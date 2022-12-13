#include <stdio.h>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <mpi.h>
#include <unistd.h>
#include <random>
using namespace std;
int **arr, width, height, *Data, bits;
void gray()
{
    int pix, place;
    for (int i = 0; i < height; i++)
    {
        place = 0;
        for (int j = 0; j < width; j++)
        {
            pix = (arr[i][j + place++] + arr[i][j + place++] + arr[i][j + place]) / 3;
            arr[i][j + place - 1] = pix;
            arr[i][j + place - 2] = pix;
            arr[i][j + place] = pix;
        }
    }
    ofstream fout("1.ppm");
    fout << "P3\n";
    fout << width << " " << height << "\n"
         << bits << "\n";
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width * 3; j++)
        {
            fout << arr[i][j] << " ";
        }
        fout << endl;
    }
}
void bright()
{
    int pix, place;
    int d = 64;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width * 3; j++)
        {
            arr[i][j] = min(255, max(0, arr[i][j] + d)); // min so pixel doesn't exceed 255. max so negative d doesn't bring pixel < 0.
        }
    }
    // cout << endl
    //      << "doneyy" << endl;
    // sleep(5);
    // cout << endl
    //      << "doneyy" << endl;

    ofstream fout("2.ppm");
    fout << "P3\n";
    fout << width << " " << height << "\n";
    fout << "255\n";
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width * 3; j++)
        {
            fout << arr[i][j] << " ";
        }
        fout << endl;
    }
}
void gmm()
{
    int pix, place;
    float d = 2;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width * 3; j++)
        {
            arr[i][j] = int(255.0 * pow((arr[i][j] / 255.0), d));
        }
    }
    // cout << endl
    //      << "doneyy" << endl;
    // sleep(5);
    // cout << endl
    //      << "doneyy" << endl;

    ofstream fout("3.ppm");
    fout << "P3\n";
    fout << width << " " << height << "\n";
    fout << "255\n";
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width * 3; j++)
        {
            fout << arr[i][j] << " ";
        }
        fout << endl;
    }
}
void blur(int n)
{
    vector<int> med;
    // sleep(2);
    int **temp_arr = new int *[height + 2];
    for (int i = 0; i < height + 2; i++)
    {
        temp_arr[i] = new int[width + 2]{0};
    }
    int pix, place;
    for (int i = 0; i < height; i++)
    {
        place = 0;
        for (int j = 0; j < width; j++)
        {
            pix = (arr[i][j + place++] + arr[i][j + place++] + arr[i][j + place]) / 3;
            temp_arr[i][j] = pix;
        }
    }
    for (int x = 0; x < n; x++)
    {
        int sum, kenrel = 16;
        for (int i = 1; i <= height; i++)
        {
            for (int j = 1; j <= width; j++)
            {
                med.push_back(temp_arr[i - 1][j - 1] * 1);
                med.push_back(temp_arr[i - 1][j] * 2);
                med.push_back(temp_arr[i - 1][j + 1] * 1);
                med.push_back(temp_arr[i][j - 1] * 2);
                med.push_back(temp_arr[i][j] * 4);
                med.push_back(temp_arr[i][j + 1] * 2);
                med.push_back(temp_arr[i + 1][j - 1] * 1);
                med.push_back(temp_arr[i + 1][j] * 2);
                med.push_back(temp_arr[i + 1][j + 1] * 1);
                sum = 0;
                for (int i = 0; i < 9; i++)
                {
                    sum += med[i];
                }
                temp_arr[i][j] = sum / kenrel;
                med.clear();
                // printf("%d",med[]);
            }
        }
    }
    ofstream fout("4.ppm");
    fout << "P3\n";
    fout << width << " " << height << "\n";
    fout << "255\n";
    for (int i = 1; i <= height; i++)
    {
        for (int j = 1; j <= width; j++)
        {
            fout << temp_arr[i][j] << " " << temp_arr[i][j] << " " << temp_arr[i][j] << " ";
        }
        fout << endl;
    }
}
void blue()
{
    int pix, place;
    for (int i = 0; i < height; i++)
    {
        place = 0;
        for (int j = 0; j < width * 3; j++)
        {
            j++;
            arr[i][++j] = arr[i][j] * 0.99;
        }
    }
    ofstream fout("5.ppm");
    fout << "P3\n";
    fout << width << " " << height << "\n"
         << "255\n";
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width * 3; j++)
        {
            fout << arr[i][j] << " ";
        }
        fout << endl;
    }
}
void red()
{
    int pix, place;
    for (int i = 0; i < height; i++)
    {
        place = 0;
        for (int j = 0; j < width * 3; j++)
        {
            arr[i][j] = arr[i][j] * 0.49;
            j++;
            j++;
        }
    }
    ofstream fout("6.ppm");
    fout << "P3\n";
    fout << width << " " << height << "\n"
         << "255\n";
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width * 3; j++)
        {
            fout << arr[i][j] << " ";
        }
        fout << endl;
    }
}
void green()
{
    int pix, place;
    for (int i = 0; i < height; i++)
    {
        place = 0;
        for (int j = 0; j < width * 3; j++)
        {
            arr[i][++j] = arr[i][j] * 0.99;
            j++;
        }
    }
    ofstream fout("7.ppm");
    fout << "P3\n";
    fout << width << " " << height << "\n"
         << "255\n";
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width * 3; j++)
        {
            fout << arr[i][j] << " ";
        }
        fout << endl;
    }
}

int main(int argc, char *argv[])
{
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int *ptrs, size_for_proc;
    int pix1, pix2, pix3, avg, place;
    int start;
    int remaining_lines;
    if (rank == 0)
    {
        ifstream fin("img.ppm");
        string formate;
        fin >> formate;
        if (formate != "P3")
        {
            cout << "Wrong file formate";
            return 0;
        }
        fin >> width >> height >> bits;
        Data = new int[height * width * 3];
        arr = new int *[height];
        for (int i = 0; i < height; i++)
        {
            arr[i] = &(Data[width * 3 * i]);
        }
        // fin >> bits;
        int pix1, pix2, pix3, avg, place;
        for (int i = 0; i < height; i++)
        {
            place = 0;
            for (int j = 0; j < width * 3; j++)
            {
                fin >> pix1;
                arr[i][j] = pix1;
                // arr[i][j+place++]=pix1;
                // arr[i][j+place]=pix1;
            }
            // cout<<place<<endl;
        }
        fin.close();
        cout << "close";
    }
    // sleep(5);
    MPI_Bcast((void *)&width, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast((void *)&height, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank != 0)
    {
        Data = new int[height * width * 3];
        arr = new int *[height];
        for (int i = 0; i < height; i++)
        {
            arr[i] = &(Data[width * 3 * i]);
        }
    }
    MPI_Bcast((void *)&(**arr), height * width * 3, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)
    {
        gray();
    }
    else if (rank == 1)
    {
        bright();
    }
    else if (rank == 2)
    {
        gmm();
    }
    else if (rank == 3)
    {
        green();
    }
    else if (rank == 4)
    {
        red();
    }
    else if (rank == 5)
    {
        blur(6);
    }
    else if (rank == 6)
    {
        blue();
    }

    MPI_Finalize();
    return 0;
}