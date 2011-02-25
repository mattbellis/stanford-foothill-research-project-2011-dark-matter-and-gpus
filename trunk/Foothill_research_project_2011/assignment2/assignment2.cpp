#include<iostream>
#include<cmath>
#include<fstream>
#include<sstream>
#include<cstdlib>

#define DAY_HOUR 24
#define HOUR_SEC 3600    

using namespace std;    

double const PI = 3.14;  
static int const MAX_PARTICLES = 1000;

//methods definitions
long double GravForce(long double grav, long double mass1,
        long double mass2, long double dist);  
double Acceleration(long double mass, long double force); 
double Velocity(double accel, double vel_init, double time);
long double Distance(long double x1, long double y1, long double z1,
        long double x2, long double y2, long double z2);
long double DistancePart(double vel_fin, double vel_init, double time);
double ChangeSecToDays(double time_sec);   
void DisplayMassConst(long double mass1, long double mass2, 
        long double grav_const);
void DisplayCoordinates(long double x, long double y, long double z, double vel, double accel); 


int main(int argc, char **argv)  
{
    float GRAV_CONST;
    long double x = 0, y= 0,z = 0, force=0, force_part=0; 
    int NUM_PARTICLES;

    if (argc < 2)     
    {
        cerr << endl;
        cerr << "Must pass in config file on command line!" << endl;
        cerr << "Usage: " << argv[0] << " <config file> " << endl;
        cerr << endl; 
        exit(1); 
    }

    ifstream infile(argv[1]); 

    infile >> GRAV_CONST;
    infile >> NUM_PARTICLES;

    long double mass[NUM_PARTICLES], force_comp[3], force_total[NUM_PARTICLES][3];
    double pos[NUM_PARTICLES][3], vel[NUM_PARTICLES][3], acc[NUM_PARTICLES][3];

    if(infile.good()) 
    {
        for(int i=0; i<NUM_PARTICLES; i++)
        {
            infile >> mass[i] >> pos[i][0] >> pos[i][1] >> pos[i][2] >> vel[i][0] >> vel[i][1] >> vel[i][2];
        }

    } 
    else
    {
        cerr << "Couldn't open the file for input." << endl;
        exit(1);
    }
    long double dist[NUM_PARTICLES][NUM_PARTICLES];

    double time = 1000, time_to_hit = 0; 

    int num_time_steps = 0;

    // Zero out the values
    for (int i=0; i<NUM_PARTICLES; i++) 
    {
        for (int j=0; j<3; j++) 
        {
            force_comp[j] = 0.0;
            force_total[i][j] = 0.0;
            acc[i][j] = 0.0;
            dist[i][j] = 0.0;
        }
    }

    cout.setf(ios_base::scientific,ios::fixed);  
    cout.precision(4); 

    double total_distance;
    double original_distance;
    double cos_phi, sin_phi, cos_theta, sin_theta;
    double vel_temp[3];
    double pos_temp[3];
    double absx,absy,absz;

    double acc_cutoff = 0.01, unit_vector_x=0, unit_vector_y=0, unit_vector_z=0;
    double k;

    cout << NUM_PARTICLES << endl;

    double local_dist = 0.0;
    double local_dist_sq = 0.0;
    double local_dist_cubed = 0.0;
    double Gm1m2 = 0.0;

    while(true)
    {
        
        ///////////////////////////////////////////////////////////////////////
        // Write out how many steps we've taken
        ///////////////////////////////////////////////////////////////////////
        if (num_time_steps%10000==0)
        {
            cerr << num_time_steps << endl;
        }
        ///////////////////////////////////////////////////////////////////////
            
        for(int i=0; i<NUM_PARTICLES; i++)
        {
            for(int j=0; j<NUM_PARTICLES; j++)
            {
                if(i != j)
                {

                    dist[i][j] = Distance(pos[i][0], pos[i][1], pos[i][2], 
                            pos[j][0], pos[j][1], pos[j][2]);

                    local_dist = dist[i][j];
                    local_dist_cubed = local_dist * local_dist * local_dist;

                    //force = GravForce(GRAV_CONST, mass[i], mass[j], local_dist); 
                    Gm1m2 = GRAV_CONST* mass[i]* mass[j];
                    force_part = Gm1m2/local_dist_cubed;

                    x = pos[j][0] - pos[i][0];
                    y = pos[j][1] - pos[i][1];
                    z = pos[j][2] - pos[i][2];
                    //absx = abs(x);
                    //absy = abs(y);
                    //absz = abs(z);

                    /*
                    if(x != 0)
                        unit_vector_x = x /absx;
                    else
                        unit_vector_x = 1;
                    if(y != 0)
                        unit_vector_y = y /absy;
                    else
                        unit_vector_y = 1;
                    if(z != 0)		         
                        unit_vector_z = z /absz;
                    else
                        unit_vector_z =1;

                    k = sqrt(local_dist_sq - z * z);

                    cos_phi = absx / k;
                    sin_phi = absy / k;
                    cos_theta = absz / local_dist; 
                    sin_theta = k / local_dist;
                    */

                    //force_comp[0] = force * sin_theta * cos_phi * unit_vector_x;
                    //force_comp[1] = force * sin_theta * sin_phi * unit_vector_y; 
                    //force_comp[2] = force * cos_theta * unit_vector_z;

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
                acc[i][k] = Acceleration(mass[i], force_total[i][k]);
                vel_temp[k] = vel[i][k];
                vel[i][k] = Velocity(acc[i][k], vel_temp[k], time);

                pos[i][k] += DistancePart(vel[i][k], vel_temp[k], time); 
                force_total[i][k] = 0.0;
            }
        }

        ////////////////////////////////////////////////////////////////////////
        // Need to check to see if acceleration is too big!
        ////////////////////////////////////////////////////////////////////////
        bool acceleration_is_too_big = false;
        bool too_close = false;
        for (int i=0; i<NUM_PARTICLES; i++) 
        {
            for (int j=0; j<3; j++) 
            {
                if(abs(acc[i][j]) > acc_cutoff && acceleration_is_too_big==false)
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
                    if (i!=j && dist[i][j] < 1e4)
                    {
                        cerr << dist[i][j] << endl;
                        too_close = true;
                    }
                }
            }
        }

        if (too_close)
            exit(0);
        ////////////////////////////////////////////////////////////////////////

        time_to_hit += time;

        cout << time_to_hit << ",";
            
        for (int i=0;i<NUM_PARTICLES;i++) 
        {
            cout << pos[i][0] << "," << pos[i][1] << "," << pos[i][2] << ",";
        }

        cout << endl;
        num_time_steps++;
    }
    cerr << "time in seconds: " << time_to_hit<< endl;    
    cerr << ChangeSecToDays(time_to_hit) << " days.\n\n";    
    return 0;
}
long double GravForce(long double GRAV, long double mass1,   
        long double mass2, long double dist) 
{ 
    return (GRAV * mass1 * mass2) / (dist * dist);
}
double Acceleration(long double mass, long double force)
{
    return force / mass;
}
double  Velocity(double accel, double vel_init, double time)  
{
    double velocity_final = vel_init + accel * time;
    return velocity_final;
}
double ChangeSecToDays(double time_sec)
{
    return time_sec / HOUR_SEC / DAY_HOUR;  
}
long double Distance(long double x1, long double y1, long double z1,
        long double x2, long double y2, long double z2)  
{
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));  
}
long double DistancePart(double vel_fin, double vel_init, double time)
{
    long double distance = (vel_fin + vel_init) /2 * time;
    return distance;
}
void DisplayMassConst(long double mass1, long double mass2, 
        long double grav_const)
{
    cout <<  grav_const / pow(10.0, -11) << "e8   " << mass1 / pow(10.0, 24)
        << "e24   " <<  mass2 / pow(10.0, 20) << "e20  " << endl; 
}
void DisplayCoordinates(long double x, long double y, long double z,
        double vel, double accel)
{
    cout <<  x << "," << y << "," << z << "," << vel << "," << accel << ",";
}
