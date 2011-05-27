#!/usr/bin/env python

################################################################################
# This program will generate intitial conditions for an n-body simulation.
#
# It will randomly generate the mass, initial position and intitial velocity
# for the particles, using an (x,y,z) coordinate system.
# 
# The user can specify whether or not these values are generated from a flat
# distribution or a random distribution.
#
################################################################################



import sys
from math import *
import numpy as np
from optparse import OptionParser


################################################################################
# main
################################################################################
def main():

    # Parse the command line options
    myusage = "\nusage: %prog [options]"
    parser = OptionParser(usage = myusage)

    parser.add_option("--pos-range",  dest="pos_range", default=None, 
            help="Range over which to evenly generate the positions.")
    parser.add_option("--pos-mean",  dest="pos_mean", default=None, 
            help="Mean value to use for a normal distribution over which to generate the positions.")
    parser.add_option("--pos-width",  dest="pos_width", default=None, 
            help="Width value to use for a normal distribution over which to generate the positions.")

       
    parser.add_option("-n", "--num-particles",  dest="nparticles",
        default=2, help="Number of particles")

   

    # Parse the options
    (options, args) = parser.parse_args()

    # Declare some variables for use throughout the program
    pos_flat = True
    pos_range = 1.0
    pos_mean = 0.0
    pos_width = 0.0

    
    ############################################################################
    # Check to see if the user has passed things properly on the command line.
    ############################################################################
    if options.pos_range and (options.pos_mean or options.pos_width):
        print "Cannot set positions range for both flat and Gaussian!!!"
        sys.exit(-1)
    elif (options.pos_mean and not options.pos_width) or (options.pos_width and not options.pos_mean):
        print "Must set both mean and width for positions!!!"
        sys.exit(-1)
    elif options.pos_range and not options.pos_mean and not options.pos_width:
        pos_range = float(options.pos_range)
        pos_flat = True
    elif not options.pos_range and options.pos_mean and options.pos_width:
        pos_mean = float(options.pos_mean)
        pos_width = float(options.pos_width)
        pos_flat = False


    nparticles = int(options.nparticles)
    ############################################################################
    ############################################################################

    # Generate the config file
    print nparticles

    for i in range(nparticles):
        output = ""

        # Generate the initial position either from a flat or Gaussian distribution.
        # Do this for x,y,z
        for j in range(3):
            pos = [0.0]
            if pos_flat:
                pos[0] = 2.0*pos_range*np.random.random_sample() - pos_range
                output += "%-11.3e, " % (pos[0])
            else:
                # Make sure value is positive
                while pos[0]<=0.0:
                    pos = np.random.normal(pos_mean, pos_width,1)
                output += "%-11.3e, " % (pos[0])


        print output
    
    

################################################################################
# Top-level script evironment
################################################################################
if __name__ == "__main__":
    main()
