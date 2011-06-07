#include<stdio.h>
//nclude<sstream>
#include<string.h>
#include<stdlib.h>
//nclude<fstream>

__global__ void distance(float *x, float *y, float *z, int xind, int yind)// float *dist)
{


   int idx = xind * blockDim.x + yind;
 //  int idx = blockIdx.x * blockDim.x + threadIdx.x;
//   int idx_dist = idx * NUM_PART;
   float x_idx = x[idx], y_idx =y[idx], z_idx = z[idx];
   float dist_x, dist_y, dist_z, dist;

   for(int i=idx+1; i<blockDim.x; i++)
   {
      if(idx != i)
      {
         dist_x = x_idx - x[i];
         dist_y = y_idx - y[i];
         dist_z = z_idx - z[i];
         dist = sqrt(dist_x * dist_x + dist_y * dist_y + dist_z * dist_z);
//         dist[idx_dist + i] = sqrt(dist_x * dist_x + dist_y * dist_y + dist_z * dist_z);
     }
   }
}

using namespace std;

int main(int argc, char **argv)
{

    float *dev_pos_x, *dev_pos_y, *dev_pos_z, *dev_dist;
    float *pos_x, *pos_y, *pos_z, *h_dist;

    int NUM_PARTICLES;

    if (argc < 2)
    {

        printf("\nMust pass in cluster_data file  on command line!\n");
        printf("\nUsage: ", argv[0] );
        printf(" <cluster_data file> <distances file> \n\n");
        exit(1);
    }

    //ifstream infile(argv[1]);
    FILE *infile;
    infile = fopen(argv[1],"r");

   // FILE *output_dist;

   // output_dist = fopen(argv[2], "w");

   // if(!output_dist)
   // {
   //      perror("Error opening file");
   //      return 1;
   // }

    //////////////////////////////////////////////////////////////////////
    // Read in the cluster_data file
    ////////////////////////////////////////////////////////////////////////////

   // char axis_titles[256];
    char dummy[256];

    fscanf(infile, "%d", &NUM_PARTICLES);
  //  fscanf(infile, "%s %s %s %s", &axis_titles, &dummy, &axis_titles, &dummy);

    int size = NUM_PARTICLES * sizeof(float);    

    pos_x = (float*)malloc(size);
    pos_y = (float*)malloc(size);
    pos_z = (float*)malloc(size);
    h_dist = (float*)malloc(size * size);

    for(int i=0; i<NUM_PARTICLES; i++)
    {
        fscanf(infile, "%e %s %e %s %e %s", &pos_x[i], &dummy, &pos_y[i], &dummy, &pos_z[i], &dummy);
    }

    ////////////////////////////////////////////////////////////////////////////

    dim3 grid, block;
    block.x = 512;
    grid.x = 127; //NUM_PARTICLES/block.x;


    cudaMalloc((void **) &dev_pos_x, size );
    cudaMalloc((void **) &dev_pos_y, size );
    cudaMalloc((void **) &dev_pos_z, size );
  //  cudaMalloc((void **) &dev_dist, size * size);

    // Check to see if we allocated enough memory.
    if (0==dev_pos_z || 0==dev_pos_y|| 0==dev_pos_x)// || 0==dev_dist)
    {
        printf("couldn't allocate memory\n");
        return 1;
    }


    // Initialize array to all 0's
    cudaMemset(dev_pos_x,0,size);
    cudaMemset(dev_pos_z,0,size);
    cudaMemset(dev_pos_y,0,size);
   // cudaMemset(dev_dist,0,size*size);

    cudaMemcpy(dev_pos_x, pos_x, size, cudaMemcpyHostToDevice );
    cudaMemcpy(dev_pos_y, pos_y, size, cudaMemcpyHostToDevice );
    cudaMemcpy(dev_pos_z, pos_z, size, cudaMemcpyHostToDevice );

   int x, y;
   int block_num = NUM_PARTICLES / block.x;

   for(int k = 0; k < block_num; k++)
   {
      y = k* block.x;
      for(int j = 0; j < block_num; j++)
      {
         { 
            x = j * block.x; 
            distance<<<grid, block >>>(dev_pos_x, dev_pos_y, dev_pos_z, x, y);//, dev_dist);
         }
      }
    //cudaMemset(dev_dist,1.0,size*size);
   // cudaMemcpy(h_dist, dev_dist, size * size, cudaMemcpyDeviceToHost );
    
  //  fprintf(output_dist, "%s %i \n", "GPU ", NUM_PARTICLES);

  //  for(int k=0; k< NUM_PARTICLES * NUM_PARTICLES; k++)
  //  {
  //     if(h_dist[k] > 0)
  //       fprintf(output_dist, "%e \n", h_dist[k]);
  //  }

  //  fclose(output_dist);   

    free(pos_x);
    free(pos_y);
    free(pos_z);

    cudaFree(dev_pos_x);
    cudaFree(dev_pos_y);  
    cudaFree(dev_pos_z);

    return 0;
}  
