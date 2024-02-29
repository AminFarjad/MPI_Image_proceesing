#include <stdio.h>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <mpi.h>
#include <unistd.h>
#include<random>
using namespace std;
// hello world
int main(int argc, char *argv[])
{
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int *arr, *ptrs, width, size_for_proc;
    int pix1, pix2, pix3, avg, place;
    int height, start;
    int remaining_lines;
    int bits;
    string file="output.ppm";
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
        arr = new int[height*width];
        fin>>bits;
        int pix1, pix2, pix3, avg, place;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                fin >> pix1 >> pix2 >> pix3;
                arr[i*width+j] = pix1;
            }
        }
        fin.close();
        size_for_proc=height/size;
    }

    MPI_Bcast((void *)&width, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast((void *)&size_for_proc, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int *temp_arr=new int[width*size_for_proc];
    MPI_Scatter((void *)arr, width*size_for_proc, MPI_INT, (void *)temp_arr, width*size_for_proc, MPI_INT, 0, MPI_COMM_WORLD);
    int pixels=width*size_for_proc;
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(((pixels*10)/100), ((pixels*21)/100)); // define the range
    int range=distr(gen);

    std::uniform_int_distribution<> distrx(0, size_for_proc-1);
    std::uniform_int_distribution<> distry(0, width-1);
    int x,y;
    int m=0;
    for (int i = 0; i < range; i++)
    {
        m++;
        x=distrx(gen);
        y=distry(gen);
        temp_arr[x*width+y]=255;
    }
    printf("%d\n",m);
    range=distr(gen);
    for (int i = 0; i < range; i++)
    {
        y=distry(gen);
        x=distrx(gen);
        temp_arr[x*width+y]=0;
    }
    MPI_Gather((void *)temp_arr, width * size_for_proc, MPI_INT, (void *)arr, width * size_for_proc, MPI_INT, 0, MPI_COMM_WORLD);
    if(rank==0){
        ofstream fout("noise.ppm");
        fout << "P3\n";
        fout << width << " " << height << "\n";
        fout<<bits<<"\n";
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
