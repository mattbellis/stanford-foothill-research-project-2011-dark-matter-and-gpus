#include<iostream>
#include<cmath>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<stdio.h>
#include<cstring>

using namespace std;    

int main(int argc, char **argv)  
{
    
    float *pos_x, *pos_y, *pos_z;
    float x = 0.0, y= 0.0, z = 0.0; 
    int NUM_PARTICLES;

    if (argc < 3)     
    {
        cerr << endl;
        cerr << "Must pass in cluster_data file  on command line!" << endl;
        cerr << "Usage: " << argv[0] << " <cluster_data file> <distances file> " << endl;
        cerr << endl; 
        exit(1); 
    }

    
    FILE *infile;
    infile = fopen(argv[1],"r");
 
    FILE *output_dist;

    output_dist = fopen(argv[2], "w");

    if(!output_dist)
    {
       perror("Error opening file");
       return 1;
    }
     

    ////////////////////////////////////////////////////////////////////////////
    // Read in the cluster_data file
    ////////////////////////////////////////////////////////////////////////////

    // char axis_titles[256];
    char dummy[256];

//    string axis_titles;
//  for(int m=0; m <3; m++)
//   infile >> axis_titles >> axis_titles;
    
    fscanf(infile, "%d", &NUM_PARTICLES);
  //  fscanf(infile, "%s %s %s %s", &axis_titles, &dummy, &axis_titles, &dummy);

	 
    int size = NUM_PARTICLES * sizeof(float);

    pos_x = (float*)malloc(size);
    pos_y = (float*)malloc(size);
    pos_z = (float*)malloc(size);

    for(int i=0; i<NUM_PARTICLES; i++)
       fscanf(infile, "%e %s %e %s %e %s", &pos_x[i], &dummy, &pos_y[i], &dummy, &pos_z[i], &dummy);
    

    float *dist = (float*)malloc(size * size);

    // Zero out the values
    memset(dist, 0, size * size);

   // cout.setf(ios_base::scientific,ios::fixed);  
   // cout.precision(4); 

    int number_of_distances = 0; 
    
        for(int i=0; i<NUM_PARTICLES; i++)
        {
            for(int j=0; j<NUM_PARTICLES; j++)
            {
        
                if(i != j && i < j)
                {
                    x = pos_x[j] - pos_x[i];
                    y = pos_y[j] - pos_y[i];
                    z = pos_z[j] - pos_z[i];

                    dist[ i*NUM_PARTICLES + j] = sqrt( x*x + y*y + z*z);
                    
		    number_of_distances++;
                }
            }
        }
     fprintf(output_dist, "%s %i %s \n", "CPU ", NUM_PARTICLES, "particles");
     
     for(int i=0; i<NUM_PARTICLES * NUM_PARTICLES; i++)
     {
         if(dist[i] > 0.0)
            fprintf(output_dist, "%e \n", dist[i]);
     }
   
    fclose(output_dist);

    free(pos_x);
    free(pos_y);
    free(pos_z);
    free(dist);

    return 0;
}
