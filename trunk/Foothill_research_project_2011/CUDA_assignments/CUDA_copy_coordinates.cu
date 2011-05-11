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
    infile >> NUM_PARTICLES;

    string axis_titles;
    for(int m=0; m <3; m++)
       infile >> axis_titles >> axis_titles;

    float pos[NUM_PARTICLES][3];

    string dummy;

    if(infile.good())
    {
        for(int i=0; i<NUM_PARTICLES; i++)
        {

            infile >> pos[i][0] >> dummy;
            infile >> pos[i][1] >> dummy;
            infile >> pos[i][2] >> dummy;
        }

    }
    else
     {
        cerr << "Couldn't open the file for input." << endl;
        exit(1);
    }
    ////////////////////////////////////////////////////////////////////////////
    
    int size = NUM_PARTICLES * sizeof(float);    

    cudaMalloc((void **) &dev_pos_x, size );
    cudaMalloc((void **) &dev_pos_y, size );
    cudaMalloc((void **) &dev_pos_z, size );

    cudaMemcpy(dev_pos_x, pos[NUM_PARTICLES][0], size, cudaMemcpyHostToDevice );
    cudaMemcpy(dev_pos_y, pos[NUM_PARTICLES][1], size, cudaMemcpyHostToDevice );
    cudaMemcpy(dev_pos_z, pos[NUM_PARTICLES][2], size, cudaMemcpyHostToDevice );
    
    
    cudaFree(dev_pos_x);
    cudaFree(dev_pos_y);  
    cudaFree(dev_pos_z);

   return 0;
}  
