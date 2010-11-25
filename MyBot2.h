#ifndef MYBOT2_H
#define MYBOT2_H

#include <vector>
#include <set>
#include <fstream>
#include <iostream>
#ifdef MyBot2_CC
//Globals
std::set<int> attacked_planets;
std::ofstream logger;

std::vector<int> frontier;
#endif

#endif
