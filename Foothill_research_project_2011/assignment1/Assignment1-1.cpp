#include<iostream>
#include<cmath>
#include<fstream>
#include<sstream>
#include<cstdlib>

#define DAY_HOUR 24
#define HOUR_SEC 3600    

using namespace std;    

double const PI = 3.14;  

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
   long double mass1, mass2, GRAV_CONST, x1, x2, y1, y2, z1, z2, dist, force=0;
   long double dist1, dist2, x_f1 = 0, x_f2 = 0;

   double a1=0, a2=0, v_i1 = 0, v_i2 = 0, v_f1=0, 
          v_f2=0, time = 1f0, time_to_hit = 0;
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
      istringstream(line) >> mass1 >> x1 >> y1 >> z1;
      getline(infile, line); 
      istringstream(line) >> mass2 >> x2 >> y2 >> z2;   
   } 
   else
   {
      cerr << "Couldn't open the file for input." << endl;
      exit(1);
   }
   
   cout.setf(ios::fixed);  
   cout.precision(5); 

   //display the gravitational cont, and mass of two particles
   //DisplayMassConst(mass1, mass2, GRAV_CONST);
   
   //display all parameters bevore mooving
   //cout << time_to_hit << " " << force; 

   //DisplayCoordinates(x1, y1, z1, vel_init1, accel1);
   //DisplayCoordinates(x2, y2, z2, vel_init2, accel2);
   cout << endl;

   //calculates the distance of two particles before moving
   dist = Distance(x1, y1, z1, x2, y2, z2);

   double total_distance;

   while(true)
   {
      force = GravForce(GRAV_CONST, mass1, mass2, 
                       dist - x_f1 - x_f2); 
      a1 = Acceleration(mass1, force);
      a2 = Acceleration(mass2, force);
   
      v_f1 = Velocity(a1, v_i1, time);
      v_f2 = Velocity(a2, v_i2, time);
  
      dist1 = DistancePart(v_f1, v_i1, time); 
      dist2 = DistancePart(v_f2, v_i2, time);
      
      //the distance they moved all together within time step
      total_distance = x_f1 + x_f2 + dist1 + dist2;
      
      if(total_distance > dist)
      {
         time = time / 10;
         if(time < 1)
            break;
         continue;
      }
      else
         if((total_distance + dist1 + dist2) == dist)
            break;
      
      time_to_hit += time;
 
      //total distances each moves in time step
      x_f1 += dist1;
      x_f2 += dist2;
      
      //initial velocities for the next time step
      v_i1 = v_f1;
      v_i2 = v_f2;
      
      cout << time_to_hit << "," << force / pow(10.0, 20) << "e20" << "," 
		<< time_to_hit << ",";
      DisplayCoordinates(x_f1, y1, z1, v_f1, a1); 
      DisplayCoordinates(dist - x_f2, y2, z2, v_f2, a2); 

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
   return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));  
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

