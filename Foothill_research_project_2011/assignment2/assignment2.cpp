#include<iostream>
#include<cmath>
#include<fstream>
#include<sstream>
#include<cstdlib>

#include<string>

#define DAY_HOUR 24
#define HOUR_SEC 3600    

using namespace std;    

double const PI = 3.14;  
static int const MAX_PARTICLES = 1000;

//methods definitions
long double Distance(long double x1, long double y1, long double z1,
        long double x2, long double y2, long double z2);

int main(int argc, char **argv)  
{
    float GRAV_CONST;
    long double x = 0, y= 0,z = 0, force=0, force_part=0; 
    int NUM_PARTICLES;

    if (argc < 4)     
    {
        cerr << endl;
        cerr << "Must pass in config and diagnostic files on command line!" << endl;
        cerr << "Usage: " << argv[0] << " <config file> <diagnostic file> <output file> " << endl;
        cerr << endl; 
        exit(1); 
    }

    ifstream infile(argv[1]); 

    ofstream outfile_diag(argv[2]);

    ofstream outfile_pos(argv[3]);

    ////////////////////////////////////////////////////////////////////////////
    // Read in the config file
    ////////////////////////////////////////////////////////////////////////////
    infile >> GRAV_CONST;
    infile >> NUM_PARTICLES;

    long double mass[NUM_PARTICLES], force_comp[3], force_total[NUM_PARTICLES][3];
    double pos[NUM_PARTICLES][3], vel[NUM_PARTICLES][3], acc[NUM_PARTICLES][3];
    double mom[NUM_PARTICLES][3], mom_comp[3];
    string dummy;
    if(infile.good()) 
    {
        for(int i=0; i<NUM_PARTICLES; i++)
        {
            infile >> mass[i] >> dummy; 
            infile >> pos[i][0] >> dummy;
            infile >> pos[i][1] >> dummy;
            infile >> pos[i][2] >> dummy;
            infile >> vel[i][0] >> dummy;
            infile >> vel[i][1] >> dummy;
            infile >> vel[i][2] >> dummy;
        }

    } 
    else
    {
        cerr << "Couldn't open the file for input." << endl;
        exit(1);
    }
    ////////////////////////////////////////////////////////////////////////////

    long double dist[NUM_PARTICLES][NUM_PARTICLES];

    double time = 1000, current_time = 0; 

    int num_time_steps = 0;

    // Zero out the values
    for (int i=0; i<NUM_PARTICLES; i++) 
    {
        for (int j=0; j<3; j++) 
        {
            force_comp[j] = 0.0;
            acc[i][j] = 0.0;
            dist[i][j] = 0.0;
            mom_comp[j] = 0.0;
            mom[i][j] = 0.0;
        }
    }

    cout.setf(ios_base::scientific,ios::fixed);  
    cout.precision(4); 

    double vel_temp[3];
    double pos_temp[3];

    double acc_cutoff = 0.01;
    double k;
    double max_force = 0.0, max_vel = 0.0, max_accel=0.0;
    double vel_magn =0.0, accel_magn =0.0;

    outfile_pos << NUM_PARTICLES << endl;

    double local_dist = 0.0;
    double local_dist_sq = 0.0;
    double local_dist_cubed = 0.0;
    //double local_dist_plus_eps = 0.0;
    double Gm1m2 = 0.0, eps = 1e7;

    int prev_time_printed_out = 0.0;

    double accel_magn_part= 0.0, vel_magn_part= 0.0;
    double momentum_total = 0.0, momentum_sq; 
    double kinetik_total = 0.0, potent_total = 0.0, energy_total = 0.0;

    outfile_diag << "Time," << "Max_Force," << "Max_Velocity," << "Max_Acceleration,"
        << "Total_Momentum," << "Total_Energy," << "Total_Kinetik," << "Total_Potential\n";
    while(true)
    {

        ///////////////////////////////////////////////////////////////////////
        // Write out how many steps we've taken
        ///////////////////////////////////////////////////////////////////////
        if(num_time_steps > 250000)
            exit(1);
        if (num_time_steps%10000==0)
        {
            cerr << num_time_steps << endl;
        }
        ///////////////////////////////////////////////////////////////////////

        for(int i=0; i<NUM_PARTICLES; i++)
        {
            for(int m=0; m < 3; m++)
                force_total[i][m]=0.0;
            for(int j=0; j<NUM_PARTICLES; j++)
            {
                if(i != j)
                {
                    dist[i][j] = Distance(pos[i][0], pos[i][1], pos[i][2], 
                            pos[j][0], pos[j][1], pos[j][2]);
                    
                    local_dist = dist[i][j] + eps;
                    local_dist_sq = local_dist * local_dist;
                    local_dist_cubed = dist[i][j] * (local_dist * local_dist);

                    //force = GravForce(GRAV_CONST, mass[i], mass[j], local_dist); 
                    Gm1m2 = GRAV_CONST* mass[i]* mass[j];
                    force_part = Gm1m2/local_dist_cubed;

                    if(i < j)
                    {
                        potent_total += Gm1m2 / local_dist;
                    }

                    force = Gm1m2 / local_dist_sq;
                    if(force > max_force)
                        max_force = force;
                    x = pos[j][0] - pos[i][0];

                    y = pos[j][1] - pos[i][1];
                    z = pos[j][2] - pos[i][2];

                    force_comp[0] = force_part * x;
                    force_comp[1] = force_part * y;
                    force_comp[2] = force_part * z;

                    for(int m=0; m < 3; m++)
                        force_total[i][m]+=force_comp[m];

                }
            }
        }

        for (int i=0; i<NUM_PARTICLES; i++) 
        {
            for(int k=0; k<3; k++)
            {
                acc[i][k] = force_total[i][k] / mass[i];
                vel_temp[k] = vel[i][k];
                vel[i][k] = vel_temp[k] + acc[i][k] * time;
                mom[i][k] = mass[i] * vel[i][k];
                mom_comp[k] += mom[i][k]; 
                pos[i][k] += (vel[i][k] + vel_temp[k])/2 * time; 

                vel_magn_part += vel[i][k] * vel[i][k];
                accel_magn_part += acc[i][k] * acc[i][k];
            }
            vel_magn = sqrt(vel_magn_part);
            accel_magn = sqrt(accel_magn_part);
            if(vel_magn > max_vel)
                max_vel = vel_magn;
            if(accel_magn > max_accel)
                max_accel = accel_magn;

            kinetik_total += 0.5 * mass[i] * vel_magn_part;

            potent_total *= -1.0; 
            energy_total += kinetik_total + potent_total;

            vel_magn_part = 0.0;
            accel_magn_part = 0.0;
        }

	potent_total *= -1.0; 
	energy_total += kinetik_total + potent_total;
        
        momentum_sq = 0.0;
        for(int k= 0; k<3; k++)
        {
            momentum_sq += mom_comp[k] * mom_comp[k];
        }
        momentum_total = sqrt(momentum_sq);   
        ////////////////////////////////////////////////////////////////////////
        // Need to check to see if acceleration is too big!
        ////////////////////////////////////////////////////////////////////////
        bool acceleration_is_too_big = false;
        bool too_close = false;
        for (int i=0; i<NUM_PARTICLES; i++) 
        {
            for (int k=0; k<3; k++) 
            {
                if(abs(acc[i][k]) > acc_cutoff && acceleration_is_too_big==false)
                {
                    if(time < 10)
		       break;
                    acceleration_is_too_big = true;
                    acc_cutoff *= 2;
                    time = time / 2;
                    continue;
                }
                else
                {
                    for (int j=0; j < NUM_PARTICLES; j++)
		    {
                       if (i!=j && dist[i][j] < 1e4)
        	       {
                          cerr << "Too close! " << dist[i][j] << endl;
		          too_close = true;
                       }
                    }
                }
            }
        }

        if (too_close)
            exit(0);
        ////////////////////////////////////////////////////////////////////////
        /// Write the data into diagnostic and output files 
        ///////////////////////////////////////////////////////////////////////

        current_time += time;
	//if (num_time_steps > 4e6)
	//if (1)
	//{
		//cerr << "current_time: " << int(current_time) << endl;
	//}
        //if(int(current_time) % 1000 == 0)
        if(current_time>=prev_time_printed_out+1000)
        {
           outfile_pos << current_time << ",";
           outfile_diag << current_time << ", " << max_force << ", " << max_vel << ", " 
                   << max_accel << ", " << momentum_total << ", " << energy_total << ", "
                   << kinetik_total << ", " << potent_total; 
           for(int k=0; k<3; k++)
           {
             // outfile_max << mom_comp[k] << ", ";
              mom_comp[k] = 0.0;
           }
           outfile_diag << endl;
           
           for (int i=0;i<NUM_PARTICLES;i++) 
           {
              outfile_pos << pos[i][0] << "," << pos[i][1] << "," << pos[i][2] << ",";
           }

           outfile_pos << endl;
           prev_time_printed_out = current_time;
       }

            outfile_pos << endl;
        }

        num_time_steps++;
        max_force = 0.0;
        max_accel = 0.0; 
        max_vel = 0.0;
        momentum_total = 0.0;
        kinetik_total = 0.0; potent_total = 0.0; energy_total = 0.0;
    }
    outfile_diag.close();
    outfile_pos.close();
    return 0;
}
long double Distance(long double x1, long double y1, long double z1,
        long double x2, long double y2, long double z2)  
{
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));  
}
