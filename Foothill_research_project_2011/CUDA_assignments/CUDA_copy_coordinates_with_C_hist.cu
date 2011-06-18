#include<stdio.h>
#include<string.h>
#include<stdlib.h>

using namespace std;

#define SUBMATRIX_SIZE 1000
#define NUM_BIN 100
#define HIST_MIN 0.0
#define HIST_MAX 3e9

////////////////////////////////////////////////////////////////////////
__global__ void distance(float *x, float *y, float *z, int xind, int yind, int *dev_hist)
{


    //int idx = xind * blockDim.x + yind;
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int thread_idx = idx;
    idx += xind;

    float x_idx = x[idx], y_idx =y[idx], z_idx = z[idx];
    float dist_x, dist_y, dist_z, dist;

    //int max = SUBMATRIX_SIZE*

    int ymax = yind + SUBMATRIX_SIZE;
    int bin_index; 
    int offset = 0;

    for(int i=yind; i<ymax; i++)
    {
        //if(idx != i)
        if(idx > i)
        {
            dist_x = x_idx - x[i];
            dist_y = y_idx - y[i];
            dist_z = z_idx - z[i];
            dist = sqrt(dist_x * dist_x + dist_y * dist_y + dist_z * dist_z);
            if(dist < HIST_MIN)
                bin_index = 0; 
            else if(dist >= HIST_MAX)
                bin_index = NUM_BIN + 1;
            else
                bin_index = int(((dist - HIST_MIN) * NUM_BIN / HIST_MAX) +1);    
                //bin_index = 5;

            //dev_hist[bin_index]++;

            offset = ((NUM_BIN+2)*thread_idx);
            bin_index += offset;

           //dev_hist[i] = xind;
           //dev_hist[i+idx] = idx;
           dev_hist[bin_index]++;

           //dev_hist[0+offset] = blockDim.x;
           //dev_hist[1+offset] = blockIdx.x;
           //dev_hist[2+offset] = threadIdx.x;
           //dev_hist[3+offset] = thread_idx;
           //dev_hist[4+offset] = idx;
           //dev_hist[5+offset] = yind;
           //dev_hist[6+offset] = ymax;
        }
    }
    //dev_hist[0 + (threadIdx.x*12)] = threadIdx.x;
    /*
    for (int i=0;i<10;i++)
    {
        offset = i*12;
        //offset = 0.0;
        dev_hist[threadIdx.x+offset] = threadIdx.x;
        //dev_hist[offset] = threadIdx.x;
        //dev_hist[offset] = 999;
    }
    */
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
    int size_hist_bytes = size_hist*sizeof(int);

    hist = (int*)malloc(size_hist_bytes);
    memset(hist, 0, size_hist_bytes);

    printf("size_hist: %d\n",size_hist_bytes);
    cudaMalloc((void **) &dev_hist, (size_hist_bytes));
    cudaMemset(dev_hist, 0, size_hist_bytes);

    int *hist_array;

    hist_array =  (int*)malloc((NUM_BIN+2) * sizeof(int));
    memset(hist_array, 0, (NUM_BIN+2)*sizeof(int)); 

    ////////////////////////////////////////////////////////////////////////////
    // Define the grid and block size
    ////////////////////////////////////////////////////////////////////////////
    dim3 grid, block;
    grid.x = 10;
    block.x = SUBMATRIX_SIZE/grid.x; //NUM_PARTICLES/block.x;
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


    int bin_index = 0;
    for(int k = 0; k < num_submatrices; k++)
    {
        y = k*SUBMATRIX_SIZE;
        printf("%d %d\n",k,y);
        for(int j = 0; j < num_submatrices; j++)
        {
                x = j *SUBMATRIX_SIZE; 

                //printf("----\n");
                //printf("%d %d\t\t%d %d\n",k,y,j,x);
                //printf("----\n");

                cudaMemset(dev_hist,0,size_hist_bytes);

                distance<<<grid,block>>>(dev_pos_x, dev_pos_y, dev_pos_z, x, y, dev_hist);
                cudaMemcpy(hist, dev_hist, size_hist_bytes, cudaMemcpyDeviceToHost);


                for(int m=0; m<size_hist; m++)
                {

                    bin_index = m%(NUM_BIN+2);
                    //if(bin_index == 0)
                        //printf("\n");

                    //printf("%3i:%3i ", m, hist[m]);
                    //printf("%3i ", hist[m]);

                    hist_array[bin_index] += hist[m];
                }    
                //printf("\n");

        }
    }

    // cudaMemcpy(hist, dev_hist, size_hist, cudaMemcpyDeviceToHost);
    /*
    for(int j=0; j<NUM_BIN+2; j++)
        for(int i=0; i<SUBMATRIX_SIZE; i++)
            hist_array[j] += hist[i*(NUM_BIN + 2)+j];
    */

    unsigned long total = 0;
    for(int k=0; k<NUM_BIN+2; k++)
    {
        printf("%i \n", hist_array[k]);
        total += hist_array[k];
    }
    printf("total: %lu \n", total);

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
