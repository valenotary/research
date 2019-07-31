/*
Construction of R2, using Professor Sungjin's pseudocode

Initially R2 only has (0, 0) (0, 0)
for each (A, B) in R2,
   for every G = (g1, g2) where g1, g2 \leq 1 and G is not zero vector
      if A+B+G <= (2,2)
        if greedy assigns G to A, meaning ||A+G||_infinity <= ||B+G||_infinity,
          then add (A + G, B) to G.
        if greedy assigns G to B, meaning ||A+G||_infinity >= ||B+G||_infinity,
          then add (A, B + G) to G.
*/

#include <utility>
#include <iostream>
#include <string>
#include <chrono>
//#include <thread>
//#include <ctime>
#include <math.h>
#include <set>
#include <iterator>
#include <algorithm>


typedef std::chrono::high_resolution_clock Clock;

struct Configuration {
  std::pair <std::pair<double, double>, std::pair<double, double>> config;
  Configuration(double w, double x, double y, double z) {
    config = std::make_pair(std::make_pair(w, x), std::make_pair(y, z));
  }
  //bool operator()(const Configuration &lhs, const Configuration &rhs) const;

};

bool configFind (std::set<Configuration*> *R2, Configuration *c) {
  for (std::set<Configuration*>::iterator it = R2->begin(); it != R2->end(); it++) {
    double a1 = (*it)->config.first.first;
    double a2 = (*it)->config.first.second;
    double b1 = (*it)->config.second.first;
    double b2 = (*it)->config.second.second;

    double c1 = c->config.first.first;
    double c2 = c->config.first.second;
    double d1 = c->config.second.first;
    double d2 = c->config.second.second;

    if ((a1 == c1 && a2 == c2 && b1 == d1 && b2 == d2) || (a1 == d1 && a2 == d2 && b1 == c1 && c2 == b2)) return true;
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

// bool Configuration::operator()(const Configuration &lhs, const Configuration &rhs) const{
//   std::pair<double, double> A1 = lhs.config.first;
//   std::pair<double, double> B1 = lhs.config.second;
//   std::pair<double, double> A2 = rhs.config.first;
//   std::pair<double, double> B2 = rhs.config.second;
//   if (A1.first == A2.first && B1.first == A2.first && A1.second == A2.second && B1.second == B2.second) return true;
// }


bool R2CONSTRUCTION(double eps, std::set<Configuration*> *R2, std::set<std::pair<double, double>> G) {
  int count, added, ignored;
  std::cout <<"RUNNING R2CONSTRUCTION!" << std::endl;
  //for (int i = 0; i < eps*4; i++) {
    //std::cout << "On the " << i << "th loop..." << std::endl;
    for(std::set<Configuration*>::iterator it = R2->begin(); it != R2->end(); it++) {
      count++;
      //std::cout << "Configuration loop" << std::endl;
      double a1 = (*it)->config.first.first;
      double a2 = (*it)->config.first.second;
      double b1 = (*it)->config.second.first;
      double b2 = (*it)->config.second.second;
      //std::cout << "("<< a1 << ", " << a2 << ") (" << b1 << ", " << b2 << ") "<< std::endl;
      if (a1 + b1 > 2.01 || a2 + b2 > 2.01) {
        std::cout << "Config error: Overloaded config in R2 with: " << a1 << " " << a2 << "; " << b1 << " " << b2 << std::endl;
        return false;
      }
      for(std::set<std::pair<double, double>>::iterator g = G.begin(); g != G.end(); g++) {

        double g1 = (*g).first;
        double g2 = (*g).second;
        //std::cout << "("<< a1 << ", " << a2 << ") (" << b1 << ", " << b2 << ") (" << g1 << ", " << g2 << ")" << std::endl;
        //checking if A+B current config is > (2, 2)
        //if (a1 + b1 > 2 || a2 + b2 > 2)  return false;
        //if G is leq 1 and not (0, 0)
        if ((g1 <= 1 && g2 <= 1) && (g1 != 0 && g2 != 0)) {
          if (
            (a1 + b1 + g1 <= 2)
            &&
            (a2 + b2 + g2 <= 2)) {
              double aMax, bMax;
              if (a1 + g1 > a2 + g2)  {
                aMax = a1 + g1;
              } else {
                aMax = a2 + g2;
              }
              if (b1 + g1 > b2 + g2)  {
                bMax = b1 + g1;
              } else {
                bMax = b2 + g2;
              }
              //assign to either A or B and add to R2
              // Configuration *n;
              // double min = std::min(aMax, bMax);
              // if (min = aMax) {
              //   n = new Configuration (a1 + g1, a2 + g2, b1, b2);
              // } else if (min = bMax) {
              //   n = new Configuration (a1, a2, b1 + g1, b2 + g2);
              // }
              // if (!configFind(R2, n)) R2->insert(n);
              if(aMax <= bMax)  {
                //std::cout << //"Config added ! aMax <= bMax" << std::endl;
                //if (R2->insert(new Configuration(a1 + g1, a2 + g2, b1, b2)).second) added++;
                Configuration *n1 = new Configuration (a1 + g1, a2 + g2, b1, b2);
                if (!configFind(R2, n1)) R2->insert(n1);
                //R2->insert(new Configuration(a1 + g1, a2 + g2, b1, b2));
                  //R2->insert(new Configuration(a1 + g1, a2 + g2, b1, b2));
              }
              if (bMax >= aMax)  {
                //std::cout << //"Config added ! aMax >= bMax" << std::endl;
                //if (R2->insert(new Configuration(a1 + g1, a2 + g2, b1, b2)).second == true) added++;
                Configuration *n2 = new Configuration (a1, a2, b1 + g1, b2 + g2);
                if (!configFind(R2, n2)) R2->insert(n2);
                //R2->insert(new Configuration(a1, a2, b1 + g1, b2 + g2));
                  //R2->insert(new Configuration(a1, a2, b1 + g1, b2 + g2));
              }
            } else {//
              //ignored++;
              //std::cout << "Config not added !" << std::endl;
          }
        }
      }
    }
  //}
  // std::cout << "Total number of iterations: " << count << std::endl;
  // std::cout << "Total number of added: " << added << std::endl;
  // std::cout << "Total number of ignored: " << ignored << std::endl;
  return true;
}

std::set<Configuration*> OVERLOADED(std::set<Configuration*> R2, double alpha) {
  std::set<Configuration*> R2_OVERLOAD;
  for (std::set<Configuration*>::iterator it = R2.begin(); it != R2.end(); it++) {
    std::pair<double, double> A = (*it)->config.first;
    std::pair<double, double> B = (*it)->config.second;

    double a1 = A.first; //pf.first
    double a2 = A.second; //pf.second
    double b1 = B.first; //ps.first
    double b2 = B.second; //ps.second

    double aMax, bMax;
    if (a1 >= a2) aMax = a1; else aMax = a2;
    if (b1 >= b2) bMax = b1; else bMax = b2;

    if (aMax + 1 > alpha && bMax + 1 > alpha) R2_OVERLOAD.insert(new Configuration(a1, a2, b1, b2)); //else std::cout << "fine" << std::endl;

  }
  return R2_OVERLOAD;
}

//need to check if config intersects with unit box; or either line segment (1, 0) or (0, 1)
bool intersect(Configuration *coord) {

  //bool crossTop, crossSide;

  double x1, y1, x2, y2;
  x1 = coord->config.first.first; // (x1, y1) (x2, y2)
  y1 = coord->config.first.second;
  x2 = coord->config.second.first;
  y2 = coord->config.second.second;

  //coords we need to pay attention to are (0, 1) (1, 0) (1, 1)

  //top side check first
  if (std::min(x1, x2) <= 1 && std::max (x1, x2) >= 1) {
    if (std::min(y1, y2) <= 1 && std::max (y2, y1) >= 1) return true;
  }

  //check side side now
  if (std::min(y1, y2) >= 0 && (std::max(y1, y2) <= 1)) {
    if (std::min(x1, x2) <= 1 && std::max(x1, x2) >= 1) return true;
  }

  return false;
}

int main() {
  int test;
  double eps = 5; //SUBJECT TO CHANGE
  std::set<Configuration*> R2;
  std::set<std::pair<double, double>> G;

  std::cout << "Epsilon used in this run: " << eps << std::endl;

  //populate G with each x,y in (x, y) ranging from [0, 2] in multiples of eps
  std::cout << "Populating G..." << std::endl;
  //std::cout << "There will be " << " entries" << std::endl;
  for (int i = 0; i < (2*eps + 1); i++) {
    //std::cout << "i: " << i << std::endl;
      for (int j = 0; j < (2*eps + 1); j++) {
        //std::cout << "j: " << j << std::endl;
        //so long as i,j != 0
        if ((i == 0 && j != 0) || (i != 0 && j == 0) || (i != 0 && j != 0)) {
          //std::cout << "Added config: " << i*eps << " " << j*eps << std::endl;
          G.insert(std::make_pair(i/eps, j/eps));
        }
      }
  }

  // for (std::set<std::pair<double, double>>::iterator it = G.begin(); it != G.end(); it++) {
  //   std::cout << "(" << (*it).first << ", " << (*it).second << ")" << std::endl;
  // }

  std::cout << "Populated G!" << std::endl;
  std::cout << "Press enter to continue" << std::endl;
  std::cin >> test;
  //First add (0, 0) (0, 0)
  R2.insert(new Configuration(0, 0, 0, 0));
  //R2.insert(new Configuration(2, 0, 0, 2));
  // R2.insert(new Configuration(1, 1, 1, 1));
  // R2.insert(new Configuration(2, 2, 2, 2));
  std::cout << "Size of R2 PRIOR to Construction: " << R2.size() << std::endl;
  auto t1 = Clock::now();
  //call R2CONSTRUCTION and see if each configuration is valid
  if (R2CONSTRUCTION(eps, &R2, G)) {
    auto t2 = Clock::now();
    std::cout << "Finished calculating in: ";
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "milliseconds" << std::endl;
    std::cout << "Number of AT MOST expected elements in R2: " << (pow((2*eps + 1), 4) - 4) << std::endl;
    std::cout << "Size of R2: " << R2.size() << std::endl;
    std::cout << "Contents of R2..." << std::endl;
    for (std::set<Configuration*>::iterator it = R2.begin(); it != R2.end(); it++) {
      double a1, a2, b1, b2;
      a1 = (*it)->config.first.first;
      a2 = (*it)->config.second.first;
      b1 = (*it)->config.first.second;
      b2 = (*it)->config.second.second;

      if (a1 + b1 > 2.01 || a2 + b2 > 2.01) R2.erase(configIterator(&R2, (*it)));
      else std::cout << "(" << a1 << ", " << a2 <<") ; (" << b1 << ", " << b2 << ")" << std::endl;
    }
  } else {
    //if for some reason it isnt, then it will run this branch
    std::cout << "Construction invalid for some configuration!" << std::endl;
  }
  // if (R2.find(new Configuration (0.2, 0.2, 0.2, 0.2)) !=R2.end()) std::cout << "WOOP" << std::endl;
  //if (configFind(&R2, new Configuration(1.8, 0, 0.2, 0))) std::cout << "MERP" << std::endl;
  if (configFind(&R2, new Configuration(2, 0, 0, 2))) std::cout << "FOUND (2,0) (0, 2) OR (0, 2) (2, 0)";
  else std::cout << "Good R2 Set!" << std::endl;

  int num, den;
  num = 8;
  den = 3;
  double alpha = num / den;
  int count = 0;
  int meep = 0;
  std::cout << "Construction R2 intersect L(" << num << "/" << den << ")..." << std::endl;
  std::set<Configuration*> R2_Overloaded = OVERLOADED(R2, alpha);
  std::cout << "Size of Overloaded Set: " << R2_Overloaded.size() << std::endl;
  for (std::set<Configuration*>::iterator it = R2_Overloaded.begin(); it != R2_Overloaded.end(); it++) {
    double a1, a2, b1, b2;
    a1 = (*it)->config.first.first;
    a2 = (*it)->config.second.first;
    b1 = (*it)->config.first.second;
    b2 = (*it)->config.second.second;
    if (intersect((*it))) count++;
    else {std::cout << "Not Overflown configuration: " <<  "(" << a1 << ", " << a2 <<") ; (" << b1 << ", " << b2 << ")" << std::endl; meep++;
  }
} std::cout << "Count of configurations that are not overflown: " << count << std::endl;
  //if (count == 0) std::cout << "Success!" << std::endl; else std::cout << "Error!" << meep << std::endl;
}
