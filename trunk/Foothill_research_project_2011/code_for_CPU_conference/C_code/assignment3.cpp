#include<iostream>
#include<cmath>
#include<fstream>
#include<sstream>
#include<cstdlib>

#include<string>

using namespace std;    

int main(int argc, char **argv)  
{
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

    ifstream infile(argv[1]); 
    
    ofstream outfile_dist(argv[2]);
 
    ////////////////////////////////////////////////////////////////////////////
    // Read in the cluster_data file
    ////////////////////////////////////////////////////////////////////////////
    infile >> NUM_PARTICLES;

    float pos[NUM_PARTICLES][3];
    //string dummy;
    if(infile.good()) 
    {
        for(int i=0; i<NUM_PARTICLES; i++)
        {
            infile >> pos[i][0]; // >> dummy;
            infile >> pos[i][1]; // >> dummy;
            infile >> pos[i][2]; // >> dummy;
        }

    } 
    else
    {
        cerr << "Couldn't open the file for input." << endl;
        exit(1);
    }
    ////////////////////////////////////////////////////////////////////////////

    float dist[NUM_PARTICLES][NUM_PARTICLES];

    // Zero out the values
    for (int i=0; i<NUM_PARTICLES; i++) 
    {
       for(int j=0; j<NUM_PARTICLES; j++)
          dist[i][j] = 0.0;
    }

    cout.setf(ios_base::scientific,ios::fixed);  
    cout.precision(4); 

    int number_of_distances = 0; 
    
        for(int i=0; i<NUM_PARTICLES; i++)
        {
            for(int j=0; j<NUM_PARTICLES; j++)
            {
                if(i != j && i < j)
                {
                    x = pos[j][0] - pos[i][0];
                    y = pos[j][1] - pos[i][1];
                    z = pos[j][2] - pos[i][2];

                    dist[i][j] = sqrt( x*x + y*y + z*z);
                    
		    number_of_distances++;
                }
            }
        }
     outfile_dist  <<  number_of_distances << endl; 

     
     for(int i=0; i<NUM_PARTICLES; i++)
     {
         for(int j=0; j<NUM_PARTICLES; j++)
         {
            if(dist[i][j] != 0.0)
                outfile_dist << dist[i][j] << endl;
         }
     }
    outfile_dist.close();
    return 0;
}
