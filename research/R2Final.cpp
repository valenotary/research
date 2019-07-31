/*
Final Implementation of R2 Construction

Pseudocode:
Create R2 as a set with initially (0, 0) (0, 0)
for every Configuration in R2
  for every job vector in G where G <= 1 and G != (0, 0)
    if A + B + G <= 2
      add min(||A + G||_inf, ||B + G||_inf) to R2

Notes:
- Sungjin said that the R2 size should be around 1/4 of total possible values
- Need to try to get this to work with eps = 40 (!?)
  - Need to check in with Eshant to see what he's doing

*/
#include <utility>
#include <iostream>
#include <string>
#include <chrono>
//#include <thread>
//#include <ctime>
#include <math.h>
//#include <set>
#include <iterator>
#include <algorithm>
#include <set>

typedef std::chrono::high_resolution_clock Clock;

struct Configuration {
  std::pair<std::pair<double, double>, std::pair<double, double>> config;
  Configuration(std::pair<double, double> a, std::pair<double, double> b) {
    config = std::make_pair(a, b);
  }
};

//need to add configFind and configIterator
bool configFind(std::set<Configuration*> *R2, Configuration *c) {
  for (std::set<Configuration*>::iterator it = R2->begin(); it != R2->end(); it++) {
    double a1 = (*it)->config.first.first;
    double a2 = (*it)->config.first.second;
    double b1 = (*it)->config.second.first;
    double b2 = (*it)->config.second.second;

    double c1 = c->config.first.first;
    double c2 = c->config.first.second;
    double d1 = c->config.second.first;
    double d2 = c->config.second.second;

    if ((a1 == c1 && a2 == c2 && b1 == d1 && b2 == d2)) return true;
  }
  return false;
}

std::set<Configuration*>::iterator configIterator(std::set<Configuration*> *R2, Configuration *c) {
  for (std::set<Configuration*>::iterator it = R2->begin(); it != R2->end(); it++) {
    double a1 = (*it)->config.first.first;
    double a2 = (*it)->config.first.second;
    double b1 = (*it)->config.second.first;
    double b2 = (*it)->config.second.second;

    double c1 = c->config.first.first;
    double c2 = c->config.first.second;
    double d1 = c->config.second.first;
    double d2 = c->config.second.second;

    if (a1 == c1 && a2 == c2 && b1 == d1 && b2 == d2) return it;
  }
  return R2->end();
}

//R2Construction creates the R2 set, with the constraint that
//every configuration stored in it is <= (2, 2)
bool R2CONSTRUCTION(double eps, std::set<Configuration*> *R2, std::set<std::pair<double, double>> G) {
  //fill body according to pseudocode
  for (std::set<Configuration*>::iterator it = R2->begin(); it != R2->end(); it++) {
    double a1, a2, b1, b2;

    a1 = (*it)->config.first.first;
    a2 = (*it)->config.first.second;
    b1 = (*it)->config.second.first;
    b2 = (*it)->config.second.second;

    for (std::set<std::pair<double, double>>::iterator g = G.begin(); g != G.end(); g++) {
      double g1 = g->first;
      double g2 = g->second;
      if ((g1 <= 1 && g2 <= 1) && (g1 != 0 && g2 != 0)) {
        if ((a1 + b1 + g1 <= 2) && (a2 + b2 + g2 <= 2)) {
          //||C+G||_inf is the largest dimension of a configuration
          //two cases: either a and b are the same, or they aren't
          if (a1 == b1 && a2 == b2) {
            Configuration *n1 = new Configuration (std::make_pair(a1 + g1, a2 + g2), std::make_pair(b1, b2));
            if (!configFind(R2, n1)) R2->insert(n1);
            Configuration *n2 = new Configuration (std::make_pair(a1, a2), std::make_pair(b1 + g1, b2 + g2));
            if (!configFind(R2, n2)) R2->insert(n2);
          } else {
            double aMax, bMax;
            aMax = std::max(a1 + g1, a2 + g2);
            bMax = std::max(b1 + g1, b2 + g2);
            double min = std::min(aMax, bMax);
            Configuration *n;
            if (min == aMax) {
              n = new Configuration (std::make_pair(a1 + g1, a2 + g2), std::make_pair(b1, b2));
            } else if (min == bMax) {
              n = new Configuration (std::make_pair(a1, a2), std::make_pair(b1 + g1, b2 + g2));
            }
            if (!configFind(R2,n)) R2->insert(n);
          }
        }
      }
    }
  }

  return true;
}

//Need to add OVERLOADED and OVERFLOWN here

//Overloaded: if EVERY machine is > alpha - 1 on SOME dimension, then it is overloaded
std::set<Configuration*> OVERLOADED (std::set<Configuration*> R2, double alpha) {
  std::set<Configuration*> R2_OVERLOAD;
  for (std::set<Configuration*>::iterator it = R2.begin(); it != R2.end(); it++) {
    double a1, a2, b1, b2;
    a1 = (*it)->config.first.first;
    a2 = (*it)->config.first.second;
    b1 = (*it)->config.second.first;
    b2 = (*it)->config.second.second;

    double aMax, bMax;
    aMax = std::max(a1, a2);
    bMax = std::max(b1, b2);
    if (aMax > alpha - 1 && bMax > alpha - 1) R2_OVERLOAD.insert(new Configuration(std::make_pair(a1, a2), std::make_pair(b1, b2)));
  }
  return R2_OVERLOAD;
}

//Overflown: basically if the config creates a line segment that doesn't cross the unit box, then it is Overflown
std::set<Configuration*> OVERFLOWN (std::set<Configuration*> Overloaded) {
  std::set<Configuration*> OVERFLOWN;
  for (std::set<Configuration*>::iterator it = Overloaded.begin(); it != Overloaded.end(); it++) {
    double x1, y1, x2, y2;
    x1 = (*it)->config.first.first;
    y1 = (*it)->config.first.second;
    x2 = (*it)->config.second.first;
    y2 = (*it)->config.second.second;

    //(a1, a2) (b1, b2) = (0, 0) (1, 1)

    //((y1−y2)(ax−x1)+(x2−x1)(ay−y1))((y1−y2)(bx−x1)+(x2−x1)(by−y1))<0.
    if ((((y1-y2)*(0-x1)+(x2-x1)*(0-y1))*((y1-y2)*(1-x1)+(x2-x1)*(1-y1))) >= 0) OVERFLOWN.insert(new Configuration(std::make_pair(x1, y1), std::make_pair(x2, y2)));
  }
  return OVERFLOWN;
}

int main() {
  int test;
  double eps = 5; // Need to improve this to 40!
  std::set<Configuration*> R2; // unordered set we are using here
  std::set<std::pair<double, double>> G; // G set of job vectors

  for (int i = 0; i < 2*eps+1; i++) {
    for (int j = 0; j < 2*eps+1; j++) {
      if (!(i == 0 && j == 0)) {
        G.insert(std::make_pair(i/eps, j/eps));
      }
    }
  }

  for (std::set<std::pair<double, double>>::iterator it = G.begin(); it != G.end(); it++) {
    std::cout << "(" << (*it).first << ", " << (*it).second << ")" << std::endl;
  }
  std::cin >> test;

  R2.insert(new Configuration(std::make_pair(0, 0), std::make_pair(0, 0)));
  auto t1 = Clock::now();
  if (R2CONSTRUCTION(eps, &R2, G)) {
    auto t2 = Clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "milliseconds" << std::endl;
    std::cout << "Number of AT MOST expected elements in R2: " << (pow((2*eps + 1), 4) - 4) << std::endl;
    //std::cout << "Size of R2: " << R2.size() << std::endl;

    //this forloop is just for debugging purposes (delete/comment out later)
    // for (std::set<Configuration*>::iterator it = R2.begin(); it != R2.end(); it++) {
    //   double a1, a2, b1, b2;
    //   a1 = (*it)->config.first.first;
    //   a2 = (*it)->config.first.second;
    //   b1 = (*it)->config.second.first;
    //   b2 = (*it)->config.second.second;
    //
    //   //if (a1 + b1 > 2.00 || a2 + b2 > 2.00) R2.erase(configIterator(&R2, (*it)));
    //   std::cout << "(" << a1 << ", " << a2 <<") ; (" << b1 << ", " << b2 << ")" << std::endl;
    // }
    std::cout << "Size of R2: " << R2.size() << std::endl;
  } else std::cout << "Error in R2 for some config" << std::endl;

  //Overloaded and Overflown checks here

  //Overloaded set
  int num, den;
  num = 8;
  den = 3;
  double alpha = num / den;
  std::cout << "Construction R2 intersect L(" << num << "/" << den << ")..." << std::endl;
  std::set<Configuration*> R2_Overloaded = OVERLOADED(R2, alpha);
  std::cout << "Size of Overloaded set: " << R2_Overloaded.size() << std::endl;
  std::cout << "Creating Overflown set.. " << std::endl;
  std::set<Configuration*> R2Final = OVERFLOWN(R2_Overloaded);
  if (R2Final.empty()) std::cout << "algorithm is " << num << "/" << den << "competitive" << std::endl;
  else std::cout << "Error! Size of R2Final is: " << R2Final.size() << std::endl;
}
