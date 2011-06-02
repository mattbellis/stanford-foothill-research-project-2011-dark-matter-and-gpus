#include <stdio.h>
#include <stdlib.h>




__global__ void distance(float *x, float *y, float *z, int NUM_PART, float *dist)
{
    float posx, posy, posz;

    int idx = blockIdx.x * blockDim.x + threadIdx.x;   
    int idx_dist = idx * (NUM_PART); 
    for(int i=0; i<NUM_PART; i++)
    {
        if(idx != i)
        {  
            posx = x[idx] - x[i];
            posy = y[idx] - y[i];
            posz = z[idx] - z[i];
            dist[idx_dist + i] = sqrt(posx * posx + posy * posy + posz * posz); 
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


    //////////////////////////////////////////////////////////////////////
    // Read in the cluster_data file
    ////////////////////////////////////////////////////////////////////////////

    char axis_titles[256];
    char dummy[256];

    /*
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
     */

    fscanf(infile, "%d", &NUM_PARTICLES);
    fscanf(infile, "%s %s %s %s", &axis_titles, &dummy, &axis_titles, &dummy);

    int size = NUM_PARTICLES * sizeof(float);    

    pos_x = (float*)malloc(size);
    pos_y = (float*)malloc(size);
    pos_z = (float*)malloc(size);
    h_dist = (float *)malloc(size * size);

    for(int i=0; i<NUM_PARTICLES; i++)
    {
        fscanf(infile, "%e %s %e %s %e %s", &pos_x[i], &dummy, &pos_y[i], &dummy, &pos_z[i], &dummy);
    }

    ////////////////////////////////////////////////////////////////////////////

    dim3 grid,block;
    block.x = 1;
    grid.x = NUM_PARTICLES/block.x;


    cudaMalloc((void **) &dev_pos_x, size );
    cudaMalloc((void **) &dev_pos_y, size );
    cudaMalloc((void **) &dev_pos_z, size );
    cudaMalloc((void **) &dev_dist, size * size);

    // Check to see if we allocated enough memory.
    if (0==dev_pos_z || 0==dev_pos_y|| 0==dev_pos_x)
    {
        printf("couldn't allocate memory\n");
        return 1;
    }


    // Initialize array to all 0's
    cudaMemset(dev_pos_x,0,size);
    cudaMemset(dev_pos_z,0,size);
    cudaMemset(dev_pos_y,0,size);
    cudaMemset(dev_dist,0,size*size);

    cudaMemcpy(dev_pos_x, pos_x, size, cudaMemcpyHostToDevice );
    cudaMemcpy(dev_pos_y, pos_y, size, cudaMemcpyHostToDevice );
    cudaMemcpy(dev_pos_z, pos_z, size, cudaMemcpyHostToDevice );

    printf("%i\n", NUM_PARTICLES);
    for(int k=0; k< NUM_PARTICLES; k++)
        printf("%e ", pos_x[k]);    

    distance<<<block, NUM_PARTICLES >>>(dev_pos_x, dev_pos_y, dev_pos_z, NUM_PARTICLES, dev_dist);

    //cudaMemset(dev_dist,1.0,size*size);
    cudaMemcpy(h_dist, dev_dist, size * size, cudaMemcpyDeviceToHost );

    printf("\n%s\n", "distances");
    for(int k=0; k< NUM_PARTICLES * NUM_PARTICLES; k++)
    {
        printf("%e\n", h_dist[k]);
    }

    free(pos_x);
    free(pos_y);
    free(pos_z);

    cudaFree(dev_pos_x);
    cudaFree(dev_pos_y);  
    cudaFree(dev_pos_z);

    return 0;
}  
