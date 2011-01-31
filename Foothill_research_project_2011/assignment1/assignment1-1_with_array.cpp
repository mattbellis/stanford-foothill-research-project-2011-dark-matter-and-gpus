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

//	const int NUM_PARTICLES = 2;

	double mass[NUM_PARTICLES];
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

	cout << endl;

	double total_distance;
	double original_distance;
        double cos_angle, sin_angle;

        original_distance = Distance(pos[0][0], pos[0][1], pos[0][2],
			pos[1][0], pos[1][1], pos[1][2]);

        cos_angle = (pos[1][0] - pos[0][0]) / original_distance;
        sin_angle = (pos[1][1] - pos[0][1]) / original_distance;
  
	double vel_temp[3];
	double pos_temp[3];

	double acc_cutoff = 0.01;

	while(true)
	{
		//calculates the distance of two particles before moving
		dist = Distance(pos[0][0], pos[0][1], pos[0][2], 
				pos[1][0], pos[1][1], pos[1][2]);

		force = GravForce(GRAV_CONST, mass[0], mass[1], dist); 

		force_comp[0] = force * cos_angle;
                force_comp[1] = force * sin_angle; 

		for (int i=0; i<NUM_PARTICLES; i++) 
		{
			for (int j=0; j<3; j++) 
			{
				acc[i][j] = Acceleration(mass[i], force_comp[j]);
				vel_temp[j] = vel[i][j];
				vel[i][j] = Velocity(acc[i][j], vel_temp[j], time);

				if(i==0)
				     pos[i][j] += DistancePart(vel[i][j], vel_temp[j], time); 
				else
                                     pos[i][j] -= DistancePart(vel[i][j], vel_temp[j], time); 
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
	cout << "time in seconds" << time_to_hit<< endl;    
	cout << ChangeSecToDays(time_to_hit) << " days.\n\n";    
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
