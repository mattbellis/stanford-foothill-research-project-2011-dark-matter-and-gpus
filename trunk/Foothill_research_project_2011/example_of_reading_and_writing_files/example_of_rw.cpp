// These are standard libraries I always include.
#include<cstdlib>
#include<cstdio>

// This is for reading and writing to standard output or standard error.
#include<iostream>

// This is for reading and writing to files.
#include<fstream>

#define MAX_PARTICLES 100

using namespace std;

int main()
{

    // Declare the input file
    ifstream infile("input.dat");

    // Declare the output file.
    // Note that this will overwrite the file if it already exists!
    ofstream outfile("output.dat");

    // Declare some variables to hold the information.
    float a;
    int nlines;
    // This 2D array will hold much of the information.
    // We're using the MAX_PARTICLES variable to allocate the memory. If we
    // only read in a few particles, this wastes some memory, but for now,
    // it lets us keep the code very general.
    float x[MAX_PARTICLES][3];

    // Note that ``a" is an integer and ``nlines" is a float, but the fstream
    // is smart enough to know how to put the information from the file into 
    // the variables.
    infile >> a;
    infile >> nlines;

    // Read in the rest of the info.
    for (int i=0;i<nlines;i++)
    {
        infile >> x[i][0] >> x[i][1] >> x[i][2];
    }

    // Now write some variation on these numbers to the outfile.
    // Note that the ``>>" has changed to the ``<<".
    for (int i=0;i<nlines;i++)
    {
        for (int j=0;j<3;j++)
        {
            outfile << x[i][j]*x[i][j] << "\t";
        }
        outfile << endl;
    }

    outfile.close();

    return 0;
}
