#include<stdio.h>
#include<iostream>
#include<fstream>
#include<bits/stdc++.h>
using namespace std;

int main(){

    ifstream fin("any.ppm");
    int i=100;
    string formate;
    fin>>formate;
    // formate.erase(remove(formate.begin(), formate.end(), 'A'), formate.end());
    if(formate!="P3"){
        cout<<"Wrong file formte";
        return 0;
    }
    int width,height;
    fin>>width>>height;
    int **arr=new int*[height];
    for(int i=0;i<height;i++){
        arr[i]=new int[width*3];
    }
    int pix1,pix2,pix3,avg,place;
    for(int i=0;i<height;i++){
        place=0;
        for(int j=0;j<width;j++){
            fin>>pix1>>pix2>>pix3;
            avg=(pix1+pix2+pix3)/3;
            for(int k=0;k<3;k++){
                arr[i][place]=avg;
                place++;
            }
        }
    }
    // while(i--){
    //     fin>>j;
    //     cout<<" "<<j;
    // }
    cout<<endl;
    ofstream fout("gray.ppm");
    fout<<"P3\n";
    fout<<width<<" "<<height<<"\n";
    for(int i=0;i<height;i++){
        for(int j=0;j<width*3;j++){
            fout<<arr[i][j]<<" ";
        }
        fout<<endl;
    }
    fout.close();
}