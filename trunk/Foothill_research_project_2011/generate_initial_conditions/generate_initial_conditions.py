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

    parser.add_option("--vel-range",  dest="vel_range", default=None, 
            help="Range over which to evenly generate the velocities.")
    parser.add_option("--vel-mean",  dest="vel_mean", default=None, 
            help="Mean value to use for a normal distribution over which to generate the velocities.")
    parser.add_option("--vel-width",  dest="vel_width", default=None, 
            help="Width value to use for a normal distribution over which to generate the velocities.")

    parser.add_option("--mass-range",  dest="mass_range", default=None, 
            help="Range over which to evenly generate the masses.")
    parser.add_option("--mass-mean",  dest="mass_mean", default=None, 
            help="Mean value to use for a normal distribution over which to generate the masses.")
    parser.add_option("--mass-width",  dest="mass_width", default=None, 
            help="Width value to use for a normal distribution over which to generate the masses.")

    parser.add_option("-G", "--grav-const",  dest="G", default=6.6742e-11, 
            help="Gravitational constant (default=6.6742e-11)")
    parser.add_option("-n", "--num-particles",  dest="nparticles", 
            default=2, help="Number of particles")

    # Parse the options
    (options, args) = parser.parse_args()

    # Declare some variables for use throughout the program
    pos_flat = True
    pos_range = 1.0
    pos_mean = 0.0
    pos_width = 0.0

    vel_flat = True
    vel_range = 0.0
    vel_mean = 0.0
    vel_width = 0.0

    mass_flat = True
    mass_range = 1.0
    mass_mean = 0.0
    mass_width = 0.0
    
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

    if options.vel_range and (options.vel_mean or options.vel_width):
        print "Cannot set velocities range for both flat and Gaussian!!!"
        sys.exit(-1)
    elif (options.vel_mean and not options.vel_width) or (options.vel_width and not options.vel_mean):
        print "Must set both mean and width for velocities!!!"
        sys.exit(-1)
    elif options.vel_range and not options.vel_mean and not options.vel_width:
        vel_range = float(options.vel_range)
        vel_flat = True
    elif not options.vel_range and options.vel_mean and options.vel_width:
        vel_mean = float(options.vel_mean)
        vel_width = float(options.vel_width)
        vel_flat = False

    if options.mass_range and (options.mass_mean or options.mass_width):
        print "Cannot set masses range for both flat and Gaussian!!!"
        sys.exit(-1)
    elif (options.mass_mean and not options.mass_width) or (options.mass_width and not options.mass_mean):
        print "Must set both mean and width for masses!!!"
        sys.exit(-1)
    elif options.mass_range and not options.mass_mean and not options.mass_width:
        mass_range = float(options.mass_range)
        mass_flat = True
    elif not options.mass_range and options.mass_mean and options.mass_width:
        mass_mean = float(options.mass_mean)
        mass_width = float(options.mass_width)
        mass_flat = False

    nparticles = int(options.nparticles)
    G = float(options.G)
    ############################################################################
    ############################################################################

    # Generate the config file
    print G
    print nparticles

    for i in range(nparticles):
        output = ""
        # Generate the mass either from a flat or Gaussian distribution.
        mass = [0.0]
        if mass_flat:
            mass[0] = mass_range*np.random.random_sample()
            output += "%-11.3e " % (mass[0])
        else:
            # Make sure value is positive
            while mass[0]<=0.0:
                mass = np.random.normal(mass_mean, mass_width,1)
            output += "%-11.3e " % (mass[0])

        # Generate the initial position either from a flat or Gaussian distribution.
        # Do this for x,y,z
        for j in range(3):
            pos = [0.0]
            if pos_flat:
                pos[0] = 2.0*pos_range*np.random.random_sample() - pos_range
                output += "%-11.3e " % (pos[0])
            else:
                # Make sure value is positive
                while pos[0]<=0.0:
                    pos = np.random.normal(pos_mean, pos_width,1)
                output += "%-11.3e " % (pos[0])

        # Generate the initial velocity either from a flat or Gaussian distribution.
        # Do this for x,y,z
        for j in range(3):
            vel = [0.0]
            if vel_flat:
                vel[0] = 2.0*vel_range*np.random.random_sample() - vel_range
                output += "%-11.3e " % (vel[0])
            else:
                # Make sure value is positive
                while vel[0]<=0.0:
                    vel = np.random.normal(vel_mean, vel_width,1)
                output += "%-11.3e " % (vel[0])

        print output
    
    

################################################################################
# Top-level script evironment
################################################################################
if __name__ == "__main__":
    main()
