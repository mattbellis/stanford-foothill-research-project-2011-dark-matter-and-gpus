#include<iostream>
#include<cmath>
#include<fstream>
#include<sstream>
#include<cstdlib>

#include<string>

#define N 100
#define MIN 0.0
#define MAX 10.0

using namespace std;    

int main(int argc, char **argv)  
{
    int NUM_COUNT;

    if (argc < 3)     
    {
        cerr << endl;
        cerr << "Must pass a file with numbers!" << endl;
        cerr << "Usage: " << argv[0] << " <numbers_N> and <hist_numbers> " << endl;
        cerr << endl; 
        exit(1); 
    }

    ifstream infile(argv[1]); 

    ofstream outfile(argv[2]);

    ////////////////////////////////////////////////////////////////////////////
    // Read in the numbers file
    ////////////////////////////////////////////////////////////////////////////

    float number;
    //float bin_array[N+2];
    //float num_array[1000000];
    int count_array[N+2];
    float bin_limit_lo = 0.0;
    float bin_limit_hi = 1.0;
    float bin_width = (MAX-MIN)/N;

    // Zero out the values
    for (int i=0; i<N+2; i++) 
    {
        //bin_array[i] = 0.0;
        count_array[i] = 0;
    }

    if(infile.good()) 
    {
        infile >> NUM_COUNT;

        for(int i=0; i<NUM_COUNT; i++)
        {
            infile >> number;
            if(number >= MIN || number < MAX)
            {    
                for(int j=0; j<N+1; j++)
                {
                bin_limit_lo = j*bin_width;
                bin_limit_hi = (j+1)*bin_width;
                    if(number >= bin_limit_lo && number < bin_limit_hi)
                    {
                        //bin_array[j+1] = number;
                        count_array[j+1]++;
                        break;
                    }
                }
                continue;
            }
            else 
                if(number >= MAX)
                {
                    //bin_array[N+1] = number;
                    count_array[N+1]++;
                }
            //bin_array[0] = number;
            count_array[0]++;

        }
    } 
    else
    {
        cerr << "Couldn't open the file for input." << endl;
        exit(1);
    }
    ////////////////////////////////////////////////////////////////////////////

        for(int i=0; i<N+2; i++)
            outfile << count_array[i] << endl;

    outfile.close();
    return 0;
}
