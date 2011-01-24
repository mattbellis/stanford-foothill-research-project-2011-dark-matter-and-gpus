#include<iostream>
#include<cmath>
#include<fstream>
#include<sstream>
#include<cstdlib>

#define DAY_HOUR 24
#define HOUR_SEC 3600    

using namespace std;    

double const PI = 3.14;  
////////////////////////////////////////////////////////////////////////////////
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
void DisplayCoordinates(long double x1, long double y1, long double z1,
         long double x2, long double y2, long double z2);

void DisplayAll(long double force, double accel1, double accel2, double time,
         double vel1, double vel2, long double dist1, long double dist2);
//////////////////////////////////////////////////////////////////////////////// 

int main(int argc, char **argv)  
{
   long double mass1, mass2, GRAV_CONST, x1, x2, y1, y2, z1, z2, dist, force;
   long double dist1, dist2, dist_total1 = 0, dist_total2 = 0;

   double accel1, accel2, vel_init1 = 0, vel_init2 = 0, vel_fin1, 
          vel_fin2, time = 1000, time_to_hit = 0;
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

   DisplayMassConst(mass1, mass2, GRAV_CONST);
   DisplayCoordinates(x1, y1, z1, x2, y2, z2);

   dist = Distance(x1, y1, z1, x2, y2, z2);
   double total_distance;

   while(true)
   {
      force = GravForce(GRAV_CONST, mass1, mass2, 
                        dist - dist_total1 - dist_total2); 
      accel1 = Acceleration(mass1, force);
      accel2 = Acceleration(mass2, force);
   
      vel_fin1 = Velocity(accel1, vel_init1, time);
      vel_fin2 = Velocity(accel2, vel_init2, time);
  
      dist1 = DistancePart(vel_fin1, vel_init1, time); 
      dist2 = DistancePart(vel_fin2, vel_init2, time);
      
      total_distance = dist_total1 + dist_total2 + dist1 + dist2;
      
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
      

      dist_total1 += dist1;
      dist_total2 += dist2;
      
      vel_init1 = vel_fin1;
      vel_init2 = vel_fin2;
      
      DisplayAll(force, accel1, accel2, time, vel_fin1, 
                   vel_fin2, dist1, dist2);
   }
   cout << "The time two body can hit each other is: " 
        << time_to_hit << " sec.\n";
   
   cout << " or " << ChangeSecToDays(time_to_hit) << " days.\n\n";    
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
   cout << "Gravid. const. is: " << grav_const << " m^3 / (kg * s^2) \n"; 
   cout << "1st body's mass: " << mass1 << " kg, 2nd body's mass: "
        << mass2 << " kg\n\n";
}
void DisplayCoordinates(long double x1, long double y1, long double z1,
         long double x2, long double y2, long double z2)
{
   cout << "Coordinates, 1st body: abscissa " << x1 
        << ", ordinate " << y1 << ", and applicate " << z1 << endl << endl;
   cout << "2nd body: abscissa " << x2 << ", ordinate " << y2 
        << " and applicate " << z2 << endl << endl;
}
void DisplayAll(long double force, double accel1, double accel2, double time, 
         double vel1, double vel2, long double dist1, long double dist2)
{
   cout << "The Gravidational Force between first and second bodies is:\n\t"
        << force / pow(10.0, 20) << "e20 Newton" << endl <<endl;
   cout << "The accelereations: 1st body: " << accel1 
        << " 2nd body: " << accel2 << " m/s/s\n\n";  
   cout << "The velocities: 1st body: " << vel1 << " 2nd body " 
        << vel2 << " m/s\n\n";    
   cout << "The distance each body moved after " << time << "sec. "
        << " 1st body: " << dist1 << " 2nd body: " << dist2 << endl << endl;
}

