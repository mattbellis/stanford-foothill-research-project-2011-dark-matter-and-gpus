#!/bin/tcsh 

set tag = $1
../code_for_CPU_conference/CUDA_code/Calculate_arc_length_two_datasets RA_and_Declination_TEMP_1.csv random_RA_and_Declination.csv test_DR_$tag.dat
../code_for_CPU_conference/CUDA_code/Calculate_arc_length RA_and_Declination_TEMP_1.csv test_RR_$tag.dat
../code_for_CPU_conference/CUDA_code/Calculate_arc_length random_RA_and_Declination.csv test_DD_$tag.dat
