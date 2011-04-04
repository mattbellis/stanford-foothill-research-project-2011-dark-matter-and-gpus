#!/bin/bash
#
#
###########################################################################
# generate 20 files with random masses, positions, and  velocities
###########################################################################
#
#
if (( $# < 2 ))
then
   echo "enter width and number of particles"
   exit 1
fi
new_total="total_momentum_$1_for_$2_particles.csv"
touch $new_total


for(( i=1; i<=$2; i++ ))
do 
   python generate_initial_conditions.py --pos-range 1e22 --mass-mean 1e23 --mass-width $1 --vel-range 100 -n $2 > config_temp.csv
   new_name="config_file_"$i"_$1_for_$2_particles.csv"
   mv config_temp.csv $new_name
   
   awk 'BEGIN{FS=","; for(i=0; i<3; i++){mom[i]=0.0}; momenta=0.0; momentum_sum=0.0} 
   NR>2 { 
         mom[0] += $1 * $5 
         mom[1] += $1 * $6
         mom[2] += $1 * $7
         }
         END{ for(i=0; i<3; i++) { momentum_sum+=mom[i]*mom[i] }  
#momenta = sqrt(mom[0]*mom[0] + mom[1]*mom[1] + mom[2] * mom[2])
             momenta=sqrt(momentum_sum)  
             printf "%.3e\n", momenta >> "$new_total"} ' $new_name     

done
