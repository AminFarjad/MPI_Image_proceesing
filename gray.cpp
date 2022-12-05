#include <stdio.h>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <mpi.h>
#include <unistd.h>
using namespace std;

// convert -compress none any1.jpg any2.ppm
// ./a.out p3 any2.ppm any3.ppm
int main(int argc, char *argv[])
{
    int size, rank;
    int *arr, *ptrs, width, size_for_proc;
    int pix1, pix2, pix3, avg, place;
    int height, start, *pixels;
    int remaining_lines;
    int bits;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    string file="any3.ppm";
    if (rank == 0)
    {
        ifstream fin(file);
        string formate;
        fin >> formate;
        if (formate != "P3")
        {
            cout << "Wrong file formate";
            return 0;
        }

        fin >> width >> height;

        fin >> bits;
        ptrs = new int[size];
        string line;
        ptrs[0] = fin.tellg();
        int index = 1, loop = 0, j = 0;
        size_for_proc = height / size;
        while (loop < (size_for_proc * size))
        {
            getline(fin, line);
            if (j == size_for_proc)
            {
                ptrs[index] = fin.tellg();
                j = 0;
                index++;
            }
            loop++;
            j++;
        }
        remaining_lines = fin.tellg();
        fin.close();
        arr = new int[width * height];
    }
    MPI_Bcast((void *)&width, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast((void *)&size_for_proc, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter((void *)ptrs, 1, MPI_INT, (void *)&start, 1, MPI_INT, 0, MPI_COMM_WORLD);
    ifstream pread(file);
    pread.seekg(start, ios::beg);
    pixels = new int[width * size_for_proc];
    for (int i = 0; i < size_for_proc; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pread >> pix1 >> pix2 >> pix3;
            avg = (pix1 + pix2 + pix3) / 3;
            pixels[(i * width) + j] = avg;
        }
    }
    MPI_Gather((void *)pixels, width * size_for_proc, MPI_INT, (void *)arr, width * size_for_proc, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        pread.close();
        if (((size * size_for_proc) - height) != 0)
        {
            cout << "yes";
            pread.open(file);
            pread.seekg(remaining_lines, ios::beg);
            int i = 0;
            int counter = width * height - (size_for_proc * size * width);
            while (pread && (counter--))
            {
                pread >> pix1 >> pix2 >> pix3;
                avg = (pix1 + pix2 + pix3) / 3;
                arr[size * size_for_proc * width + i] = avg;
                i++;
            }
            pread.close();
        }
        ofstream fout("output.ppm");
        fout << "P3\n";
        fout << width << " " << height << "\n";
        fout << bits << "\n";
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                fout << arr[i * width + j] << " " << arr[i * width + j] << " " << arr[i * width + j] << " ";
            }
            fout << endl;
        }
    }
    MPI_Finalize();
    return 0;
}