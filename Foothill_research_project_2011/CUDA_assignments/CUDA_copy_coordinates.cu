#include<iostream>
#include<sstream>
#include<string>
#include<cstdlib>
#include<fstream>

__global__ void copy_coordinates(float *dev_x, float *dev_y, float *dev_z)
{
      
}


using namespace std;

int main(int argc, char **argv)
{

    float *dev_pos_x, *dev_pos_y, *dev_pos_z;
    float *pos_x, *pos_y, *pos_z;

    int NUM_PARTICLES;

    if (argc < 3)
    {
        cerr << endl;
        cerr << "Must pass in cluster_data file  on command line!" << endl;
        cerr << "Usage: " << argv[0] << " <cluster_data file> <distances file> " << endl;
        cerr << endl;
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
        cerr << "Couldn't open the file for input." << endl;
        exit(1);
    }

 
    int size = NUM_PARTICLES * sizeof(float);    
        
    pos_x = (float*)malloc(size);
    pos_y = (float*)malloc(size);
    pos_z = (float*)malloc(size);
   
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

    cudaMemcpy(dev_pos_x, pos_x, size, cudaMemcpyHostToDevice );
    cudaMemcpy(dev_pos_y, pos_y, size, cudaMemcpyHostToDevice );
    cudaMemcpy(dev_pos_z, pos_z, size, cudaMemcpyHostToDevice );
    
for(int k=0; k< NUM_PARTICLES; k++)
   cerr << pos_x[k];    


free(pos_x);
    free(pos_y);
    free(pos_z);
    
    cudaFree(dev_pos_x);
    cudaFree(dev_pos_y);  
    cudaFree(dev_pos_z);

   return 0;
}  
