#include<stdio.h>
#include<string.h>
#include<stdlib.h>

using namespace std;

#define SUBMATRIX_SIZE 10
#define NUM_BIN 10
#define MIN 0.0
#define MAX 100.0  

////////////////////////////////////////////////////////////////////////
__global__ void distance(float *x, float *y, float *z, int xind, int yind, int *dev_hist)
{


   //int idx = xind * blockDim.x + yind;
   int idx = blockIdx.x * blockDim.x + threadIdx.x;
   idx += xind;

   float x_idx = x[idx], y_idx =y[idx], z_idx = z[idx];
   float dist_x, dist_y, dist_z, dist;
   
   //int max = SUBMATRIX_SIZE*

   int ymax = yind + SUBMATRIX_SIZE;
   int bin_index,  bin = idx * (NUM_BIN + 2); 

   for(int i=yind; i<ymax; i++)
   {
      if(idx != i)
      {
         dist_x = x_idx - x[i];
         dist_y = y_idx - y[i];
         dist_z = z_idx - z[i];
         dist = sqrt(dist_x * dist_x + dist_y * dist_y + dist_z * dist_z);

         if(dist < MIN)
            bin_index = bin; 
         else if(dist >= MAX)
            bin_index = bin + NUM_BIN + 1;
         else
            bin_index = bin + int(((dist - MIN) * NUM_BIN / MAX) +1);    
   
         dev_hist[bin_index]++;

//for(int i=0; i<100; i++)
//dev_hist[i] =1;
     }
   }
}

////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{

    float *dev_pos_x, *dev_pos_y, *dev_pos_z;
    float *pos_x, *pos_y, *pos_z;

    int NUM_PARTICLES;

    if (argc < 2)
    {

        printf("\nMust pass in cluster_data file  on command line!\n");
        printf("\nUsage: ", argv[0] );
        printf(" <cluster_data file> <distances file> \n\n");
        exit(1);
    }

    FILE *infile;
    infile = fopen(argv[1],"r");


    //////////////////////////////////////////////////////////////////////
    // Read in the cluster_data file
    ////////////////////////////////////////////////////////////////////////////

    // char axis_titles[256];
    char dummy[256];

    fscanf(infile, "%d", &NUM_PARTICLES);
    //fscanf(infile, "%s %s %s %s", &axis_titles, &dummy, &axis_titles, &dummy);

    int size = NUM_PARTICLES * sizeof(float);    
    printf("# particles: %d\n",NUM_PARTICLES);

    pos_x = (float*)malloc(size);
    pos_y = (float*)malloc(size);
    pos_z = (float*)malloc(size);

    for(int i=0; i<NUM_PARTICLES; i++)
    {
        fscanf(infile, "%e %s %e %s %e %s", &pos_x[i], &dummy, &pos_y[i], &dummy, &pos_z[i], &dummy);
        //printf("%e %s %e %s %e %s\n", pos_x[i], dummy, pos_y[i], dummy, pos_z[i], dummy);
    }
    ////////////////////////////////////////////////////////////////////////////
    //allocation of histogram
    ///////////////////////////////////////////////////////////////////////////

    int *hist, *dev_hist;
    int size_hist = SUBMATRIX_SIZE * (NUM_BIN+2);

    hist = (int*)malloc(size_hist * sizeof(int));
    memset(hist, 0, size_hist);

    cudaMalloc((void **) &dev_hist, size_hist);
    cudaMemset(dev_hist, 0, size_hist);
  
    int *hist_array;
   
    hist_array =  (int*)malloc((NUM_BIN+2) * sizeof(int));
    memset(hist_array, 0, (NUM_BIN+2)); 
    
    ////////////////////////////////////////////////////////////////////////////
    // Define the grid and block size
    ////////////////////////////////////////////////////////////////////////////
    dim3 grid, block;
    block.x = 10;
    grid.x = SUBMATRIX_SIZE/block.x; //NUM_PARTICLES/block.x;
    ////////////////////////////////////////////////////////////////////////////

    cudaMalloc((void **) &dev_pos_x, size );
    cudaMalloc((void **) &dev_pos_y, size );
    cudaMalloc((void **) &dev_pos_z, size );

    // Check to see if we allocated enough memory.
    if (0==dev_pos_z || 0==dev_pos_y|| 0==dev_pos_x || 0==dev_hist)
    {
        printf("couldn't allocate memory\n");
        return 1;
    }


    // Initialize array to all 0's
    cudaMemset(dev_pos_x,0,size);
    cudaMemset(dev_pos_z,0,size);
    cudaMemset(dev_pos_y,0,size);

    cudaMemcpy(dev_pos_x, pos_x, size, cudaMemcpyHostToDevice );
    cudaMemcpy(dev_pos_y, pos_y, size, cudaMemcpyHostToDevice );
    cudaMemcpy(dev_pos_z, pos_z, size, cudaMemcpyHostToDevice );

    int x, y;
    int num_submatrices = NUM_PARTICLES / SUBMATRIX_SIZE;


 /*   for(int k = 0; k < num_submatrices; k++)
    {
       y = k*SUBMATRIX_SIZE;
       for(int j = 0; j < num_submatrices; j++)
       {
          { 
             x = j *SUBMATRIX_SIZE; 
*/
             distance<<<grid, block >>>(dev_pos_x, dev_pos_y, dev_pos_z, 0, 0, dev_hist);
               cudaMemcpy(hist, dev_hist, size_hist, cudaMemcpyDeviceToHost);

for(int m=0; m<size_hist; m++)
{
   if((m%12) == 0)
     printf("\n");

   printf("%i ", hist[m]);
}    
printf("\n");

/*
          }
       }
    }
*/
   // cudaMemcpy(hist, dev_hist, size_hist, cudaMemcpyDeviceToHost);
    for(int j=0; j<NUM_BIN+2; j++)
      for(int i=0; i<SUBMATRIX_SIZE; i++)
          hist_array[j] += hist[i*(NUM_BIN + 2)+j];

    for(int k=0; k<NUM_BIN+2; k++)
       printf("%i \n", hist_array[k]);

    free(pos_x);
    free(pos_y);
    free(pos_z);
    free(hist);

    cudaFree(dev_pos_x);
    cudaFree(dev_pos_y);  
    cudaFree(dev_pos_z);
    cudaFree(dev_hist);

    return 0;
}  
//////////////////////////////////////////////////////////////////////
