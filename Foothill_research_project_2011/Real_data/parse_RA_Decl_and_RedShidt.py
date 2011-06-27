#!/usr/bin/env python

import sys
from numpy import*
from math import pi
infilename = sys.argv[1]
num_lines=40012
Radian = pi / 180
infile = open(infilename,'r')

###outfilename = "parsed_file_%s_line%d.csv" % (infilename.split('.')[0], line_to_parse)
outfilename = "parsed_file_A_Decl_Red_shift.csv"
outfile = open(outfilename,'w+')

output = ""
#output = "RA, Declination , Red_shift\n"
#outfile.write(output)

for i, line in enumerate(infile):
    if i > 11 and i < num_lines:
       line_num = line.split()      
    
       for j, col in enumerate(line_num):
           if j == 1 or j == 2: 
              RA_Decl = float(col) * Radian
              output += "%.4f , " % (RA_Decl)
           if j == 24:
              output += " %s\n" % (col)
        #  print col
        #  if j == 1:
        #     RA = float(col) * Radian
        #     output += "%.4f " % (RA)
        #  if j == 2:
        #     decl = float(col) 
        #     output += ", %.4f" % (col)
           outfile.write(output)
  
           output = ""

outfile.close()
    

