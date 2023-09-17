# MPI_Image_proceesing
This project was created for the course of parallel and distributed computing offered by Fast Nuces taught by Sir Danish khan
 
How to run the project

convert -compress none filename.jpg output.ppm   // linux utility  to convert jpg/jpeg/png file to ppm format it normally gives result in p6 format

gcc sample.c   // program to convert output.ppm if it is in p6 to p3 (ascii coded format) 

./a.out p3 output.ppm output_file.ppm

mpic++ gray.cpp   // parallely convert file to gray 

mpirun --oversubscribe -np 4 ./a.out   // process count can be n

mpic++ noise.cpp

mpirun --oversubscribe -np 4 ./a.out    //process count can be n

mpic++ denoising.cpp

mpirun --oversubscribe -np 4 ./a.out    //process count can be n

mpic++ filters.cpp

mpirun --oversubscribe -np 4 ./a.out     //parallely run 7 filters on an image not more than 7 filters are there but can create their own filters and apply them also



MPI_library was used for message passing between diff process
This project includes 
>parallely applying gray scale on a p3 type image 
>parallely applying noise(salt and pepper noise) on a p3 gray scale image  
>denoising the image produced by part 2 median denoising was applied on it which is good for salt and pepper noise
>parallely applying 7 types of filter on a p3 type image (includes task parallelism)
filters are: 
             gray
             gamma correction
             blur (can be increased blur by increasing the value or parameter given to blur function)
             brightness (can be increased or decreased by changing d in the function bright)
             remaning 3 filters were just applied by tweaking the value or r/g/b values
             
             
any changes or correction in the project are highly appreciated
