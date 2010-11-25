#define MyBot2_CC
#include <iostream>
#include "PlanetWars.h"

#include <math.h>
#include "MyBot2.h"

double dist(const Planet &dest,const Planet &src)
{
  int x1 = src.X();
  int y1 = src.Y();
  int x2 = dest.X();
  int y2 = dest.Y();
  int xdiff = x1-x2;
  int ydiff = y1-y2;
  return sqrt(xdiff*xdiff+ydiff*ydiff);
}

int NeedToConquerFrom(const Planet &dest,const Planet &src)
{
  int time = (int)ceil(dist(dest,src));
  if(dest.Owner() != 0)
    return dest.NumShips()+dest.GrowthRate()*time+2;
  return dest.NumShips()+2;
}

int fleetStrengthPlanet(const Planet &dest,std::vector<Fleet> efleets)
{
  std::vector<Fleet>::iterator fleet = efleets.begin();
  int total = 0;
  for(;fleet != efleets.end();++fleet)
  {
    if(dest.PlanetID() == fleet->DestinationPlanet())
    {
        total += fleet->NumShips();
    }
  }
  return total;
}

// The DoTurn function is where your code goes. The PlanetWars object contains
// the state of the game, including information about all planets and fleets
// that currently exist. Inside this function, you issue orders using the
// pw.IssueOrder() function. For example, to send 10 ships from planet 3 to
// planet 8, you would say pw.IssueOrder(3, 8, 10).
//
// There is already a basic strategy in place here. You can use it as a
// starting point, or you can throw it out entirely and replace it with your
// own. Check out the tutorials and articles on the contest website at
// http://www.ai-contest.com/resources.
void DoTurn(const PlanetWars& pw) {
  // (1) If we currently have a fleet in flight, just do nothing.
  logger << ">New Turn"<<std::endl;

  //Mark all planets who have a fleet heading to them
  attacked_planets.clear();//reset
  std::vector<Fleet> my_fleets = pw.MyFleets();//inter
  std::vector<Fleet> enemy_fleets = pw.EnemyFleets();//inter
  for(int i = 0; i < my_fleets.size();i++)
  {
    attacked_planets.insert(my_fleets[i].DestinationPlanet());
  }
  // (2) Find my strongest planet.
  int source = -1;
  double source_score = -999999.0;
  int source_num_ships = 0;
  std::vector<Planet> my_planets = pw.MyPlanets();
  Planet& my_p = my_planets[0];
  for (int i = 0; i < my_planets.size(); ++i) {
    const Planet& p = my_planets[i];
    double score = (double)p.NumShips();
    if (score > source_score) {
      my_p = p;
      source_score = score;
      source = p.PlanetID();
      source_num_ships = p.NumShips();
    }
  }
  // (3) Find the weakest enemy or neutral planet.
  int dest = -1;
  double dest_score = -999999.0;
  std::vector<Planet> not_my_planets = pw.NotMyPlanets();
  Planet &dest_planet = not_my_planets[0];
  for (int i = 0; i < not_my_planets.size(); ++i) {
    const Planet& p = not_my_planets[i];
    double score = 1.0 / (1 + p.NumShips());
    bool found = attacked_planets.find(p.PlanetID()) == attacked_planets.end();
    logger << "Planet: " << p.PlanetID() << " found: "<<found << std::endl;
    if(attacked_planets.find(p.PlanetID()) == attacked_planets.end() )
    {
      logger << "noticed:"<<p.PlanetID() << std::endl;
      if (score > dest_score &&   
          (NeedToConquerFrom(p,my_p) + fleetStrengthPlanet(p,enemy_fleets))
          < my_p.NumShips()) {
        logger << NeedToConquerFrom(p,my_p) << "<" << my_p.NumShips()
               << std::endl;;
	dest_score = score;
	dest = p.PlanetID();
        dest_planet=p;
      }
    }
  }
  attacked_planets.insert(dest);
  // (4) Send half the ships from my strongest planet to the weakest
  // planet that I do not own.
  if (source >= 0 && dest >= 0) {
    int num_ships = NeedToConquerFrom(dest_planet,my_p);
    logger << "sending: " <<num_ships << " have: "<<my_p.NumShips()<<std::endl;
    pw.IssueOrder(source, dest, num_ships);
  }
}

// This is just the main game loop that takes care of communicating with the
// game engine for you. You don't have to understand or change the code below.
int main(int argc, char *argv[]) {
  std::string current_line;
  std::string map_data;
  
  logger.open("mylog",std::ios::out);
 
  while (true) {
    int c = std::cin.get();
    current_line += (char)c;
    if (c == '\n') {
      if (current_line.length() >= 2 && current_line.substr(0, 2) == "go") {
        PlanetWars pw(map_data);
        map_data = "";
        logger << "Start"<<std::endl;
        DoTurn(pw);
	pw.FinishTurn();
        logger << "End" << std::endl;
      } else {
        map_data += current_line;
      }
      current_line = "";
    }
  }
  return 0;
}
