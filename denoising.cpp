#include <stdio.h>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <mpi.h>
#include <unistd.h>
#include <random>
using namespace std;

int main(int argc, char *argv[])
{
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int **arr, *ptrs, width, size_for_proc;
    int pix1, pix2, pix3, avg, place;
    int height, start;
    int remaining_lines;
    int *data;
    int bits;
    string file="noise.ppm";
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
        // arr = new int[height*width];
        data = new int[(height + 2) * (2 + width)];
        arr = new int *[height + 2];
        for (int i = 0; i < height + 2; i++)
        {
            arr[i] = &(data[(width + 2) * i]);
        }
        int pix1, pix2, pix3, avg, place;
        for (int i = 1; i < height + 1; i++)
        {
            for (int j = 1; j < width + 1; j++)
            {
                fin >> pix1 >> pix2 >> pix3;
                arr[i][j] = pix1;
            }
        }
        // sleep(1);
        // cout<<"donne";
        for (int i = 1; i < width; i++)
        {
            // arr[0][i]=arr[1][i];
            // arr[height+1][i]=arr[height][i];
            arr[0][i] = 0;
            arr[height + 1][i] = 0;
        }
        // cout<<"done";
        for (int i = 1; i < height; i++)
        {
            // arr[i][0]=arr[i][1];
            // arr[i][width+1]=arr[i][width];
            arr[i][0] = 0;
            arr[i][width + 1] = 0;
        }
        // sleep(2);
        // cout<<"done";
        arr[0][0] = arr[1][1];
        arr[height + 1][width + 1] = arr[height][width];
        arr[0][width + 1] = arr[1][width];
        arr[height + 1][0] = arr[height - 1][1];
        fin.close();
        size_for_proc = height / size;
        // cout<<"doneyyy";
        // sleep(2);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast((void *)&width, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast((void *)&size_for_proc, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int *data_temp = new int[(size_for_proc) * (width + 2)];
    if (data_temp == NULL)
    {
        cout << "not possible";
        exit(2);
    }
    int **temp_arr = new int *[size_for_proc];
    for (int i = 0; i < size_for_proc; i++)
    {
        temp_arr[i] = &(data_temp[(width + 2) * i]);
    }
    // int temp_arr[size_for_proc][width+2];
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Scatter((void *)&(**arr), (width + 2) * (size_for_proc), MPI_INT, (void *)&(**temp_arr), (width + 2) * (size_for_proc), MPI_INT, 0, MPI_COMM_WORLD);
    vector<int> med;
    // sleep(2);
    for (int i = 1; i < size_for_proc - 1; i++)
    {
        for (int j = 1; j <= width; j++)
        {
            med.push_back(temp_arr[i - 1][j - 1]);
            med.push_back(temp_arr[i - 1][j]);
            med.push_back(temp_arr[i - 1][j + 1]);
            med.push_back(temp_arr[i][j - 1]);
            med.push_back(temp_arr[i][j]);
            med.push_back(temp_arr[i][j + 1]);
            med.push_back(temp_arr[i + 1][j - 1]);
            med.push_back(temp_arr[i + 1][j]);
            med.push_back(temp_arr[i + 1][j + 1]);
            sort(med.begin(), med.end());
            temp_arr[i][j] = med[4];
            med.clear();
            // printf("%d",med[]);
        }
    }
    // cout<<"yes";
    MPI_Gather((void *)&(**temp_arr), (width + 2) * size_for_proc, MPI_INT, (void *)&(**arr), (width + 2) * size_for_proc, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        vector<int> rem;
        for (int i = 0; i < size; i++)
        {
            rem.push_back(size_for_proc * (i + 1) - 1);
            rem.push_back(size_for_proc * (i + 1));
        }
        for (int k = 0; k < size * 2; k++)
        {
            int i = rem[k];
            for (int j = 1; j <= width; j++)
            {
                med.push_back(arr[i - 1][j - 1]);
                med.push_back(arr[i - 1][j]);
                med.push_back(arr[i - 1][j + 1]);
                med.push_back(arr[i][j - 1]);
                med.push_back(arr[i][j]);
                med.push_back(arr[i][j + 1]);
                med.push_back(arr[i + 1][j - 1]);
                med.push_back(arr[i + 1][j]);
                med.push_back(arr[i + 1][j + 1]);
                sort(med.begin(), med.end());
                arr[i][j] = med[4];
                med.clear();
                // printf("%d",med[]);
            }
        }
        // sleep(2);
        ofstream fout("denoise.ppm");
        fout << "P3 \n";
        fout << width << " " << height << " \n";
        fout << bits << "\n";
        for (int i = 1; i < height + 1; i++)
        {
            for (int j = 1; j < width + 1; j++)
            {
                fout << " " << arr[i][j] << " " << arr[i][j] << " " << arr[i][j];
            }
            fout << endl;
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}