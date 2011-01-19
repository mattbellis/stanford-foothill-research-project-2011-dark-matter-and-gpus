#include<iostream>
#include<cmath>
#include<fstream>
#include<sstream>

#define DAY_HOUR 24
#define HOUR_SEC 3600

using namespace std;

//#include "config_file_0.dat"
//#include "config_file_1.dat"
//#include "config_file_2.dat"
//#include "sun_and_earth_config_file.dat"

//double const GRAV_CONST = 6.6742e-11;
double const PI = 3.14;

long double GravForce(long double grav, long double mass1, 
                      long double mass2, long double dist);
double Acceleration(long double mass, long double force);
double Velocity(double accel, long double dist);
double Revolution(double dist);
double Time(double velocity, long double revolution);
double ChangeSecToDays(double time_sec);
long double Distance(long double x1, long double y1, long double z1, 
                     long double x2, long double y2, long double z2);
int main()
{
   long double mass1, mass2, GRAV_CONST, x1, x2, y1, y2, z1, z2, 
               dist, force, velocity, revolution;
   double accel1, accel2, time; 
   string line;
   
   ifstream infile("Assignment0/config_file_0.dat");
   
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
   cout << "Gravid. const. is: " << GRAV_CONST << 
         " m^3 / (kg * s^2) " <<endl;

   cout << "First body has mass " << mass1 << " kg, abscissa " << x1
      << " \n\tordinate " << y1 << " and applicate " << z1 << endl << endl;
   
   cout << "Second body has mass " << mass2 << " kg, abscissa " << x2
      << " \n\tordinate " << y2 << " and applicate " << z2 << endl << endl;
  
   //dist = 3.84403e8;
   
   dist = Distance(x1, y1, z1, x2, y2, z2);

   cout << "The distance between two bodies is: " << dist << endl << endl;

   force = GravForce(GRAV_CONST, mass1, mass2, dist);
   accel1 = Acceleration(mass1, force);
   accel2 = Acceleration(mass2, force);
   velocity = Velocity(accel2, dist);
   revolution = Revolution(dist);
   time = Time(velocity, revolution);
   
   cout.setf(ios::fixed);
   cout.precision(5);

   cout << "The Gravidational Force between first and second bodies is:\n\t"
        << force / pow(10.0, 20) << "e20 Newton" << endl <<endl;

   cout << "The accelereation of first body is: " << accel1 
         << " m/s/s\n\n";

   cout << "The acceleration of second body is: " << accel2 
         << " m/s/s\n\n";

   cout << "The velocity at which second body moves\n around the first "
         << "body is: " << velocity << " m/s\n\n";

   cout << "The distance the first body travels in one\n"
        << "revolution around the second body is: " 
        << revolution / pow(10.0, 6) << "e6 m\n\n";

   cout << "The time the first body travels around\n"
        << "the second body is: " << time << " sec ";

   time = ChangeSecToDays(time);

   cout << "or " << time << " days.\n\n";

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
double  Velocity(double accel, long double dist)
{
   return sqrt(accel * dist);
}
double Revolution(double dist)
{
   return 2 * PI * dist;
}
double Time(double vel, long double rev)
{
   return rev / vel;
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
