#include<iostream>
#include<sstream>
#include<string>
#include<cstdlib>
#include<fstream>

__global__ void distance(float *x, float *y, float *z, int NUM_PART, float *dist)
{
   float posx, posy, posz;
   
   
   for(int i=0; i<NUM_PART-1; i++)
   {
      posx = x[i+1] - x[i];
      posy = y[i+1] - y[i];
      posz = z[i+1] - z[i];
      dist[i] = sqrt(posx * posx + posy * posy + posz * posz); 
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

    ifstream infile(argv[1]);

    //////////////////////////////////////////////////////////////////////
    // Read in the cluster_data file
    ////////////////////////////////////////////////////////////////////////////

    string axis_titles;
  
    
    string dummy;

    if(infile.good())
    {
        infile >> NUM_PARTICLES;
    
        for(int m=0; m <3; m++)
           infile >> axis_titles >> axis_titles;


    }
    else
     {
        printf("Couldn't open the file for input.\n");
        exit(1);
    }

 
    int size = NUM_PARTICLES * sizeof(float);    
        
    pos_x = (float*)malloc(size);
    pos_y = (float*)malloc(size);
    pos_z = (float*)malloc(size);
    h_dist = (float *)malloc(size);
   
    for(int i=0; i<NUM_PARTICLES; i++)
    {

       infile >> pos_x[i] >> dummy;
       infile >> pos_y[i] >> dummy;
       infile >> pos_z[i] >> dummy;
    }

    ////////////////////////////////////////////////////////////////////////////
    

    cudaMalloc((void **) &dev_pos_x, size );
    cudaMalloc((void **) &dev_pos_y, size );
    cudaMalloc((void **) &dev_pos_z, size );
    cudaMalloc((void **) &dev_dist, size);

    cudaMemcpy(dev_pos_x, pos_x, size, cudaMemcpyHostToDevice );
    cudaMemcpy(dev_pos_y, pos_y, size, cudaMemcpyHostToDevice );
    cudaMemcpy(dev_pos_z, pos_z, size, cudaMemcpyHostToDevice );
    
    printf("%i\n", NUM_PARTICLES);
    for(int k=0; k< NUM_PARTICLES; k++)
       printf("%e ", pos_x[k]);    

    distance<<<NUM_PARTICLES, 1 >>>(dev_pos_x, dev_pos_y, dev_pos_z, NUM_PARTICLES, dev_dist);

    
    cudaMemcpy(h_dist, dev_dist, size, cudaMemcpyDeviceToHost );
    
    free(pos_x);
    free(pos_y);
    free(pos_z);
    
    cudaFree(dev_pos_x);
    cudaFree(dev_pos_y);  
    cudaFree(dev_pos_z);

   return 0;
}  