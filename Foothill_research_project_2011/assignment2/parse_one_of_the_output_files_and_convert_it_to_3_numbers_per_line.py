#!/usr/bin/env python

import sys

infilename = sys.argv[1]
line_to_parse = int(sys.argv[2])

infile = open(infilename,'r')

outfilename = "parsed_file_%s_line%d.csv" % (infilename.split('.')[0], line_to_parse)
outfile = open(outfilename,'w+')

for nline,line in enumerate(infile):

    if nline == line_to_parse:

       # print line
        vals = line.split(',')

        nvals = len(vals)

        nparticles = int((nvals-1)/3)

        output = "%d\n" % (nparticles)
        outfile.write(output)

        output = "Coord_X, Coord_Y , Coord_Z\n"
        outfile.write(output)
 
        output = ""
        for i in range(1,nvals):

            output += "%s , " % (vals[i])

            if (i)%3==0:
                output += "\n"
   #             print output 

                outfile.write(output)

                output = ""

outfile.close()
