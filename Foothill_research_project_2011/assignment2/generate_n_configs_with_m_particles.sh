#!/bin/bash
#
#
###########################################################################
# generate  files with random masses, positions, and  velocities, 
# calculate total momentum 
###########################################################################
#
#
if (( $# < 3 ))
then
   echo "enter width, number of particles and number of config files"
   exit 1
fi
echo "Momenta"
for(( i=1; i<=$3; i++ ))
do 
   python generate_initial_conditions.py --pos-range 1e22 --mass-mean 1e23 --mass-width $1 --vel-range 100 -n $2 > config_temp.csv
   # new_name="config_file_"$i"_$1_for_$2_particles.csv"
   # mv config_temp.csv $new_name

   awk 'BEGIN{FS=","; for(i=0; i<3; i++){mom[i]=0.0}; momenta=0.0; momentum_sum=0.0 }
   NR>2 { 
         mom[0] += $1 * $5 
         mom[1] += $1 * $6
         mom[2] += $1 * $7
        }
         END{ for(i=0; i<3; i++) { momentum_sum+=mom[i]*mom[i] }  
             momenta=sqrt(momentum_sum)  
             printf "%.3e\n", momenta } ' config_temp.csv     

done
