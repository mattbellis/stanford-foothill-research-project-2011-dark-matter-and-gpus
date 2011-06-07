#include<iostream>
#include<cmath>
#include<fstream>
#include<sstream>
#include<cstdlib>

#include<string>

#define N 10
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
    
    float bin_array[N+2], num_array[NUM_COUNT];
    int count_array[N+2];
    
    if(infile.good()) 
    {
       infile >> NUM_COUNT;
       for(int i=0; i<NUM_COUNT; i++)
           infile >> num_array[i];
    } 
    else
    {
        cerr << "Couldn't open the file for input." << endl;
        exit(1);
    }
 ////////////////////////////////////////////////////////////////////////////
    
    // Zero out the values
    for (int i=0; i<N+2; i++) 
    {
       bin_array[i] = 0.0;
       count_array[i] = 0;
    }
    
    for(int k=0; k<NUM_COUNT; k++)
    {
       if(num_array[k] >= 0 || num_array[k] < N)
       {    
          for(int j=0; j<N+1; j++)
             if(num_array[k] >= j && num_array[k] < j+1)
             {
                bin_array[j+1] = num_array[k];
                count_array[j+1]++;
                break;
             }
           continue;
       }
       else 
          if(num_array[k] >= N)
          {
             bin_array[N+1] = num_array[k];
             count_array[N+1]++;
          }
       bin_array[0] = num_array[k];
       count_array[0]++;
   }
       
  for(int i=0; i<N+2; i++)
     outfile << count_array[i] << endl;
         
    outfile.close();
    return 0;
}
