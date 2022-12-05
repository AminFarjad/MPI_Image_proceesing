#include <stdio.h>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <mpi.h>
#include <unistd.h>
#include<random>
using namespace std;

int main(int argc, char *argv[])
{
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int  *ptrs, width, size_for_proc;
    int pix1, pix2, pix3, avg, place;
    int height, start;
    int remaining_lines;
    int **arr;
    if(rank==0){
        int *data=new int[4*5];
        arr=new int*[4];
        for(int i=0;i<4;i++){
            arr[i]=&(data[5*i]);
        }
        // int *data=(int *)malloc(4*5*sizeof(int));
        // arr=(int**)malloc(4*sizeof(int*));
        // for(int i=0;i<rows;i++)
        //     arr[i]=&(data[cols*i]);
        // arr=new int*[4];
        // for(int i=0;i<4;i++){
        //     arr[i]=new int[5];
        // }
        int count=0;
        for(int i=0;i<4;i++){
            for(int j=0;j<5;j++){
                arr[i][j]=count++;
            }
        }
    }
    int *dataq=new int[2*5];
    int **pli=new int*[2];
    for(int i=0;i<2;i++){
       pli[i]=&(dataq[5*i]);
   }
    MPI_Scatter((void *)&(**arr), 2*5, MPI_INT, (void *)&(**pli), 2*5, MPI_INT, 0, MPI_COMM_WORLD);
    sleep(rank*10);
    for(int i=0;i<2;i++){
        for(int j=0;j<5;j++){
            cout<<pli[i][j]<<" ";
        }
        cout<<endl;
    }

    // if (rank == 0)
    // {
    //     ifstream fin("any.ppm");
    //     string formate;
    //     fin >> formate;
    //     if (formate != "P3")
    //     {
    //         cout << "Wrong file formate";
    //         return 0;
    //     }
    //     fin >> width >> height;
    //     arr = new int[height*width];
        
    //     int pix1, pix2, pix3, avg, place;
    //     for (int i = 0; i < height; i++)
    //     {
    //         for (int j = 0; j < width; j++)
    //         {
    //             fin >> pix1 >> pix2 >> pix3;
    //             arr[i*width+j] = (pix1+pix2+pix3)/3;
    //         }
    //     }
    //     fin.close();
    //     size_for_proc=height/4;
    // }

    // MPI_Bcast((void *)&width, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // MPI_Bcast((void *)&size_for_proc, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // int *temp_arr=new int[width*size_for_proc];
    // MPI_Scatter((void *)arr, width*size_for_proc, MPI_INT, (void *)temp_arr, width*size_for_proc, MPI_INT, 0, MPI_COMM_WORLD);
    // int pixels=width*size_for_proc;
    // // std::random_device rd; // obtain a random number from hardware
    // // std::mt19937 gen(rd()); // seed the generator
    // // std::uniform_int_distribution<> distr(((pixels*10)/100), ((pixels*21)/100)); // define the range
    // // int range=distr(gen);

    // // std::uniform_int_distribution<> distrx(0, size_for_proc-1);
    // // std::uniform_int_distribution<> distry(0, width-1);
    // // int x,y;
    // // int m=0;
    // // for (int i = 0; i < range; i++)
    // // {
    // //     m++;
    // //     x=distrx(gen);
    // //     y=distry(gen);
    // //     temp_arr[x*width+y]=255;
    // // }
    // // printf("%d\n",m);
    // // range=distr(gen);
    // // for (int i = 0; i < range; i++)
    // // {
    // //     y=distry(gen);
    // //     x=distrx(gen);
    // //     temp_arr[x*width+y]=0;
    // // }
    // MPI_Gather((void *)temp_arr, width * size_for_proc, MPI_INT, (void *)arr, width * size_for_proc, MPI_INT, 0, MPI_COMM_WORLD);
    // if(rank==0){
    //     // int rem=height-(height/4);
    //     // if(rem!=0){
    //     //     int rem_pixels=rem*width;
    //     // }
    //     ofstream fout("amin.ppm");
    //     fout << "P3\n";
    //     fout << width << " " << height << "\n";
    //     for (int i = 0; i < height; i++)
    //     {
    //         for (int j = 0; j < width; j++)
    //         {
    //             fout << arr[i * width + j] << " " << arr[i * width + j] << " " << arr[i * width + j] << " ";
    //         }
    //         fout << endl;
    //     }
    // }
    MPI_Finalize();
    return 0;
}