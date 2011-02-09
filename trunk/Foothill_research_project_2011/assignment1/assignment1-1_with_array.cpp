#include<iostream>
#include<cmath>
#include<fstream>
#include<sstream>
#include<cstdlib>

#define DAY_HOUR 24
#define HOUR_SEC 3600    

using namespace std;    

double const PI = 3.14;  
static int const NUM_PARTICLES = 2;

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
    long double GRAV_CONST, dist, force=0;
    long double x_f1 = 0, x_f2 = 0, force_comp[3];

    double a1=0, a2=0, v_i1 = 0, v_i2 = 0, v_f1=0, 
           v_f2=0, time = 1000, time_to_hit = 0;

    // Arrays to hold the position, velocity and acceleration of a given
    // particle at any time.

    long double mass[NUM_PARTICLES];
    double pos[NUM_PARTICLES][3], vel[NUM_PARTICLES][3], acc[NUM_PARTICLES][3];

    // Zero out the values
    for (int i=0; i<NUM_PARTICLES; i++) 
    {
        mass[i] = 0.0;

        for (int j=0; j<3; j++) 
        {
            force_comp[j] = 0.0;
            pos[i][j] = 0.0;
            vel[i][j] = 0.0;
            acc[i][j] = 0.0;
        }
    }

    string line;       

    if (argc < 2)     
    {
        cerr << endl;
        cerr << "Must pass in config file on command line!" << endl;
        cerr << "Usage: " << argv[0] << " <config file> " << endl;
        cerr << endl; 
        exit(1); 
    }

    ifstream infile(argv[1]); 

    if(infile.good()) 
    {
        getline(infile, line);
        istringstream(line) >> GRAV_CONST; 
        getline(infile, line); 
        istringstream(line) >> mass[0] >> pos[0][0] >> pos[0][1] >> pos[0][2];
        getline(infile, line); 
        istringstream(line) >> mass[1] >> pos[1][0] >> pos[1][1] >> pos[1][2];
    } 
    else
    {
        cerr << "Couldn't open the file for input." << endl;
        exit(1);
    }

    cout.setf(ios::fixed);  
    cout.precision(5); 

    double total_distance;
    double original_distance;
    double cos_angle, sin_angle;
    double vel_temp[3];
    double pos_temp[3];

    double acc_cutoff = 0.01, unit_vector_x=0, unit_vector_y=0, unit_vector_z=0;

    cout << "Accumulated time, force, Accumulated time, separation,";
    cout << "x1,y1,z1,vx1,ax1,";
    cout << "x2,y2,z2,vx2,ax2" << endl;
    while(true)
    {
        //calculates the distance of two particles before moving
        dist = Distance(pos[0][0], pos[0][1], pos[0][2], 
                pos[1][0], pos[1][1], pos[1][2]);

        for(int i=0; i<NUM_PARTICLES; i++)
        {
            for(int j=0; j<NUM_PARTICLES; j++)
            {
                if(i != j)
                {
                    force = GravForce(GRAV_CONST, mass[i], mass[j], dist); 

                    if((pos[j][0] - pos[i][0])!=0)
                        unit_vector_x = (pos[j][0] - pos[i][0])/abs(pos[j][0] - pos[i][0]);
                    else
                        unit_vector_x = 1;
                    if((pos[j][1] - pos[i][1])!=0)
                        unit_vector_y = (pos[j][1] - pos[i][1])/abs(pos[j][1] - pos[i][1]);
                    else
                        unit_vector_y = 1;

                    // unit_vector_z = (pos[j][2] - pos[i][2])/abs(pos[j][2] - pos[i][2]);

                    cos_angle = abs(pos[j][0] - pos[i][0]) / dist;
                    sin_angle = abs(pos[j][1] - pos[i][1]) / dist;
                    force_comp[0] = force * cos_angle * unit_vector_x;
                    force_comp[1] = force * sin_angle * unit_vector_y; 

                    for (int k=0; k<3; k++) 
                    {
                        acc[i][k] = Acceleration(mass[i], force_comp[k]);
                        vel_temp[k] = vel[i][k];
                        vel[i][k] = Velocity(acc[i][k], vel_temp[k], time);

                        pos[i][k] += DistancePart(vel[i][k], vel_temp[k], time); 
                    }
                }
            }	
        }
        if(acc[0][0] > acc_cutoff)
        {
            acc_cutoff *= 10;
            time = time / 10;
            if(time < 1)
                break;
            continue;
        }
        else
        {
            if(dist < 1e7)
                break;
        }
        time_to_hit += time;

        cout << time_to_hit << "," << force / pow(10.0, 20) << "e20" << "," 
            << time_to_hit << ",";
        cout << dist << ",";
        for (int i=0;i<NUM_PARTICLES;i++) 
        {
            DisplayCoordinates(pos[i][0], pos[i][1], pos[i][2], vel[i][0], acc[i][0]); 
        }

        cout << endl;
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
