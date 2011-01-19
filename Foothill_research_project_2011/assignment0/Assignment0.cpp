#include<iostream>
#include<cmath>

#define DAY_HOUR 24
#define HOUR_SEC 3600

using namespace std;

double const GRAV_CONST = 6.6742e-11;
double const PI = 3.14;

long double GravForce(long double mass1, long double mass2, long double dist);
double Acceleration(long double mass, long double force);
double Velocity(double accel, long double dist);
double Revolution(double dist);
double Time(double velocity, long double revolution);
double ChangeSecToDays(double time_sec);

int main()
{
   long double mass_E, mass_M, dist, force, velocity, revolution;
   double accel_E, accel_M, time; 
   
   mass_E = 5.974e24;
   mass_M = 7.349e22;
   dist = 3.84403e8;
   
   force = GravForce(mass_E, mass_M, dist);
   accel_E = Acceleration(mass_E, force);
   accel_M = Acceleration(mass_M, force);
   velocity = Velocity(accel_M, dist);
   revolution = Revolution(dist);
   time = Time(velocity, revolution);
   
   cout.setf(ios::fixed);
   cout.precision(5);

   cout << "The Gravidational Force between Earth and Moon is:\n"
        << force / pow(10.0, 20) << "e20 Newton" << endl <<endl;

   cout << "The accelereation of Earth is: " << accel_E << " m/s/s\n\n";

   cout << "The acceleration of Moon is: " << accel_M << " m/s/s\n\n";

   cout << "The velocity at which Moon moves\n around the Earth is: " 
        << velocity << " m/s\n\n";

   cout << "The distance the Moon travels in one\n"
        << "revolution around the Earth is: " << revolution / pow(10.0, 6) 
        << "e6 m\n\n";

   cout << "The time Moon travels around\n"
        << "the Earth is: " << time << " sec ";

   time = ChangeSecToDays(time);

   cout << "or " << time << " days.\n\n";

   return 0;
}

long double GravForce(long double mass1, long double mass2, long double dist)
{
   return (GRAV_CONST * mass1 * mass2) / (dist * dist);
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
