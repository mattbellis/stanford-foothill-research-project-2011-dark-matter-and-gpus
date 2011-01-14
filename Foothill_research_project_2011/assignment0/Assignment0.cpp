#include<iostream>
#include<cmath>

using namespace std;

//double const GRAV_CONST = 6.6742 * pow(10.0, -11);
double const GRAV_CONST = 6.6742e-11;

long double GravForce(long double mass1, long double mass2, long double dist);
double Acceleration(long double mass, long double force);

int main()
{
   long double mass_E, mass_M, dist, force;
   double accel_E, accel_M; 

   
   mass_E = 5.974 * pow(10.0, 24);
   mass_M = 7.349 * pow(10.0, 22);
   dist = 3.84403 * pow(10.0, 8);

   force = GravForce(mass_E, mass_M, dist);
   accel_E = Acceleration(mass_E, force);
   accel_M = Acceleration(mass_M, force);

   cout.setf(ios::fixed);
   cout.precision(5);

   cout << "The Gravitational Force between Earth and Moon is:\n"
        << force / pow(10.0, 20) << " * 10^20 Newton" << endl <<endl;

   cout << "The acceleration of Earth is: " << accel_E << " m/s/s\n\n";

   cout << "The acceleration of Moon is: " << accel_M << " m/s/s\n\n";

   return 0;
}

long double GravForce(long double mass1, long double mass2, long double dist)
{
   double gForce;

   gForce = (GRAV_CONST * mass1 * mass2) / (dist * dist);

   return gForce;
}
double Acceleration(long double mass, long double force)
{
   double acceleration;

   acceleration = force / mass;

   return acceleration;
}
