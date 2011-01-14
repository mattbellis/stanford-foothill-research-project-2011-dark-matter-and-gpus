#include<iostream>
#include<cmath>

using namespace std;

double const Grav_const = 6.6742 * pow(10.0, -11);

long double GravForce(long double mass1, long double mass2, long double dist);
double Acceleration(long double mass, long double force);

int main()
{
   long double mass_E, mass_M, Dist, Force;
   double Accel_E, Accel_M; 

   
   mass_E = 5.974 * pow(10.0, 24);
   mass_M = 7.349 * pow(10.0, 22);
   Dist = 3.84403 * pow(10.0, 8);

   Force = GravForce(mass_E, mass_M, Dist);
   Accel_E = Acceleration(mass_E, Force);
   Accel_M = Acceleration(mass_M, Force);

   cout.setf(ios::fixed);
   cout.precision(5);

   cout << "The Gravidational Force between Earth and Moon is:\n"
        << Force / pow(10.0, 20) << " * 10^20 Newton" << endl <<endl;

   cout << "The accelereation of Earth is: " << Accel_E << " m/s/s\n\n";

   cout << "The acceleration of Moon is: " << Accel_M << " m/s/s\n\n";

   return 0;
}

long double GravForce(long double mass1, long double mass2, long double dist)
{
   double gForce;

   gForce = (Grav_const * mass1 * mass2) / (dist * dist);

   return gForce;
}
double Acceleration(long double mass, long double force)
{
   double acceleration;

   acceleration = force / mass;

   return acceleration;
}