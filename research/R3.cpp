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
#include <tuple>
#include <initializer_list>

typedef std::chrono::high_resolution_clock Clock;

//I think a Configuration in R3 requires 3 dimensions?
struct Configuration {
  std::tuple<std::tuple<double, double, double>,
    std::tuple<double, double, double>,
    std::tuple<double, double, double>> config;
  Configuration(std::tuple<double, double, double> a, std::tuple<double, double, double> b, std::tuple<double, double, double> c) {
    config = std::make_tuple(a, b, c);
  }

};

//Need to change for R3
bool configFind (std::set<Configuration*> *R3, Configuration *c) {
  for (std::set<Configuration*>::iterator it = R3->begin(); it != R3->end(); it++) {
    // double a1 = (*it)->config.first.first;
    // double a2 = (*it)->config.first.second;
    // double b1 = (*it)->config.second.first;
    // double b2 = (*it)->config.second.second;
    //
    // double c1 = c->config.first.first;
    // double c2 = c->config.first.second;
    // double d1 = c->config.second.first;
    // double d2 = c->config.second.second;

    //if ((a1 == c1 && a2 == c2 && b1 == d1 && b2 == d2) || (a1 == d1 && a2 == d2 && b1 == c1 && c2 == b2)) return true;

    double a1, a2, a3, b1, b2, b3, c1, c2, c3;

    //(a1, a2, a3)
    a1 = std::get<0>(std::get<0>((*it)->config));
    a2 = std::get<1>(std::get<0>((*it)->config));
    a3 = std::get<2>(std::get<0>((*it)->config));

    //(b1, b2, b3)
    b1 = std::get<0>(std::get<1>((*it)->config));
    b2 = std::get<1>(std::get<1>((*it)->config));
    b3 = std::get<2>(std::get<1>((*it)->config));

    //(c1, c2, c3)
    c1 = std::get<0>(std::get<2>((*it)->config));
    c2 = std::get<1>(std::get<2>((*it)->config));
    c3 = std::get<2>(std::get<2>((*it)->config));

    double d1, d2, d3, e1, e2, e3, f1, f2, f3;

    //(d1, d2, d3)
    d1 = std::get<0>(std::get<0>(c->config));
    d2 = std::get<1>(std::get<0>(c->config));
    d3 = std::get<2>(std::get<0>(c->config));

    //(e1, e2, e3)
    e1 = std::get<0>(std::get<1>(c->config));
    e2 = std::get<1>(std::get<1>(c->config));
    e3 = std::get<2>(std::get<1>(c->config));

    //(f1, f2, f3)
    f1 = std::get<0>(std::get<2>(c->config));
    f2 = std::get<1>(std::get<2>(c->config));
    f3 = std::get<2>(std::get<2>(c->config));

    //Need to make this condition more general
    if ((a1 == d1 && a2 == d2 && a3 == d3 && b1 == e1 && b2 == d2 && b3 == d3 && c1 == f1 && c2 == f2 && c3 == f3)) return true;

  }
  return false;
}

//Need to change for R3
std::set<Configuration*>::iterator configIterator(std::set<Configuration*> *R3, Configuration *c) {
  for (std::set<Configuration*>::iterator it = R3->begin(); it != R3->end(); it++) {
    // double a1 = (*it)->config.first.first;
    // double a2 = (*it)->config.first.second;
    // double b1 = (*it)->config.second.first;
    // double b2 = (*it)->config.second.second;
    //
    // double c1 = c->config.first.first;
    // double c2 = c->config.first.second;
    // double d1 = c->config.second.first;
    // double d2 = c->config.second.second;

    //if (a1 == c1 && a2 == c2 && b1 == d1 && b2 == d2) return it;

    double a1, a2, a3, b1, b2, b3, c1, c2, c3;

    //(a1, a2, a3)
    a1 = std::get<0>(std::get<0>((*it)->config));
    a2 = std::get<1>(std::get<0>((*it)->config));
    a3 = std::get<2>(std::get<0>((*it)->config));

    //(b1, b2, b3)
    b1 = std::get<0>(std::get<1>((*it)->config));
    b2 = std::get<1>(std::get<1>((*it)->config));
    b3 = std::get<2>(std::get<1>((*it)->config));

    //(c1, c2, c3)
    c1 = std::get<0>(std::get<2>((*it)->config));
    c2 = std::get<1>(std::get<2>((*it)->config));
    c3 = std::get<2>(std::get<2>((*it)->config));

    double d1, d2, d3, e1, e2, e3, f1, f2, f3;

    //(d1, d2, d3)
    d1 = std::get<0>(std::get<0>(c->config));
    d2 = std::get<1>(std::get<0>(c->config));
    d3 = std::get<2>(std::get<0>(c->config));

    //(e1, e2, e3)
    e1 = std::get<0>(std::get<1>(c->config));
    e2 = std::get<1>(std::get<1>(c->config));
    e3 = std::get<2>(std::get<1>(c->config));

    //(f1, f2, f3)
    f1 = std::get<0>(std::get<2>(c->config));
    f2 = std::get<1>(std::get<2>(c->config));
    f3 = std::get<2>(std::get<2>(c->config));

    //Need to make this more general
    if ((a1 == d1 && a2 == d2 && a3 == d3 && b1 == e1 && b2 == d2 && b3 == d3 && c1 == f1 && c2 == f2 && c3 == f3)) return it;
  }
  return R3->end();
}

//R3 Construction Method
bool R3Construction(double eps, std::set<Configuration*> *R3, std::set<std::tuple<double, double, double>> G) {
  std::cout << "RUNNING R3 CONSTRUCTION" << std::endl;
  for (std::set<Configuration*>::iterator it = R3->begin(); it != R3->end(); R3++) {
    //need to extrapolate a1 a2 a3 b1 b2 b3 c1 c2 c3
    double a1, a2, a3, b1, b2, b3, c1, c2, c3;

    //(a1, a2, a3)
    a1 = std::get<0>(std::get<0>((*it)->config));
    a2 = std::get<1>(std::get<0>((*it)->config));
    a3 = std::get<2>(std::get<0>((*it)->config));

    //(b1, b2, b3)
    b1 = std::get<0>(std::get<1>((*it)->config));
    b2 = std::get<1>(std::get<1>((*it)->config));
    b3 = std::get<2>(std::get<1>((*it)->config));

    //(c1, c2, c3)
    c1 = std::get<0>(std::get<2>((*it)->config));
    c2 = std::get<1>(std::get<2>((*it)->config));
    c3 = std::get<2>(std::get<2>((*it)->config));

    if (a1 + b1 + c1 > 3.01 || a2 + b2 + c2 > 2.01 || a3 + b3 + c3 > 3.01) { // if the current config in R3 > 3, it should return false (fix the algorithm)
      std::cout << "R3 Construction Error: Overloaded config!" << std::endl;
      return false;
    }
    for (std::set<std::tuple<double, double, double>>::iterator g = G.begin(); g != G.end(); g++) {
      double g1 = std::get<0>((*g));
      double g2 = std::get<1>((*g));
      double g3 = std::get<2>((*g));
      //if g is leq 1 and not (0, 0, 0)
      if ((g1 <= 1 && g2 <= 1 && g3 <= 1) && (g1 != 0 && g2 != 0 && g3 != 0)) {
        // if A + B + C + G <= (3, 3, 3)
        if ((a1 + b1 + c1 + g1 <= 3) && (a2 + b2 + c2 + g2 <= 3) && (a3 + b3 + c3 + g3 <= 3)) {
          double aMax, bMax, cMax;

          aMax = std::max({a1 + g1, a2 + g2, a3 + g3}); // ||A + G||_inf
          bMax = std::max({b1 + g1, b2 + g2, b3 + g3}); // ||B + G||_inf
          cMax = std::max({c1 + g1, c2 + g2, c3 + g3}); // ||C + G||_inf

          Configuration *n;

          double min = std::min({aMax, bMax, cMax});

          if (min = aMax) {

          } else if (min = bMax) {

          } else if (min = cMax) {
            
          }

          //Need to make this more general
          // if (aMax <= bMax && aMax <= cMax) {
          //   n = new Configuration(std::make_tuple(a1 + g1, a2 + g2, a3 + g3), std::make_tuple(b1, b2, b3), std::make_tuple(c1, c2, c3));
          // }
          // if (bMax <= aMax && bMax <= cMax) {
          //   n = new Configuration(std::make_tuple(a1, a2, a3), std::make_tuple(b1 + g1, b2 + g2, b3 + g3), std::make_tuple(c1, c2, c3));
          // }
          // if (cMax <= aMax && cMax <= bMax) {
          //   n = new Configuration(std::make_tuple(a1, a2, a3), std::make_tuple(b1, b2, b3), std::make_tuple(c1 + g1, c2 + g2, c3 + g3));
          // }

        }
      }
    }
  }
  return true;
}

//check if it is overloaded
std::set<Configuration*> OVERLOADED(std::set<Configuration*> R3, double alpha) {
  std::set<Configuration*> R3_OVERLOAD;
  for (std::set<Configuration*>::iterator it = R3.begin(); it != R3.end(); it++) {
    double a1, a2, a3, b1, b2, b3, c1, c2, c3;

    //(a1, a2, a3)
    a1 = std::get<0>(std::get<0>((*it)->config));
    a2 = std::get<1>(std::get<0>((*it)->config));
    a3 = std::get<2>(std::get<0>((*it)->config));

    //(b1, b2, b3)
    b1 = std::get<0>(std::get<1>((*it)->config));
    b2 = std::get<1>(std::get<1>((*it)->config));
    b3 = std::get<2>(std::get<1>((*it)->config));

    //(c1, c2, c3)
    c1 = std::get<0>(std::get<2>((*it)->config));
    c2 = std::get<1>(std::get<2>((*it)->config));
    c3 = std::get<2>(std::get<2>((*it)->config));



  }
  return R3_OVERLOAD;
}

//check if it is overflown
bool OVERFLOWN(Configuration *coord) {


  return false;
}

//main driver function
int main() {
  double eps = 5; // tweak this value (5 is best value)
  std::set<Configuration*> R3;
  std::set<std::tuple<double, double, double>> G;

  //Populate G with every single kind of configuration possible
  // for (int i = 0; i < (2*eps + 1); i++) {
  //     for (int j = 0; j < (2*eps + 1); j++) {
  //       //so long as i,j != 0
  //       if ((i == 0 && j != 0) || (i != 0 && j == 0) || (i != 0 && j != 0)) {
  //         //std::cout << "Added config: " << i*eps << " " << j*eps << std::endl;
  //         G.insert(std::make_pair(i/eps, j/eps));
  //       }
  //     }
  // }

  for (int i = 0; i < (2*eps + 1); i++) {
    for (int j = 0; j < (2*eps + 1); j++) {
      for (int k = 0; k < (2*eps + 1); k++) {
        //config has to be nonzero
        if (!(i == 0 && j == 0 && k == 0)) {
          G.insert(std::make_tuple(i/eps, j/eps, k/eps));
        }
      }
    }
  }

  //check the contents of G
  // for (std::set<std::tuple<double, double, double>>::iterator it = G.begin(); it != G.end(); it++) {
  //   double x1, x2, x3;
  //   x1 = std::get<0>((*it));
  //   x2 = std::get<1>((*it));
  //   x3 = std::get<2>((*it));
  //   std::cout << "(" << x1 << ", " << x2 << ", " << x3 <<")" << std::endl;
  // }

  //first insert (0, 0) (0, 0) (0, 0) first
  std::tuple<double, double, double> zeroTuple = std::make_tuple(0, 0, 0); // creating a single (0, 0) config
  R3.insert(new Configuration(zeroTuple, zeroTuple, zeroTuple)); // insert into R3

  auto t1 = Clock::now(); // start a timer for the function
  if (R3Construction(eps, &R3, G)) {
    auto t2 = Clock::now();
    std::cout << "Finished calculating in: "; // Get a timestamp of the duration of R3Construction
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "milliseconds" << std::endl;
    std::cout << "Size of R3: " << R3.size() << std::endl;
    //Now iterate through R3 one more time and check for any wrong configs
    for (std::set<Configuration*>::iterator it = R3.begin(); it != R3.end(); it++) {
      //extrapolate each dimension of each pair into double variables
      double a1, a2, a3, b1, b2, b3, c1, c2, c3;

      //(a1, a2, a3)
      a1 = std::get<0>(std::get<0>((*it)->config));
      a2 = std::get<1>(std::get<0>((*it)->config));
      a3 = std::get<2>(std::get<0>((*it)->config));

      //(b1, b2, b3)
      b1 = std::get<0>(std::get<1>((*it)->config));
      b2 = std::get<1>(std::get<1>((*it)->config));
      b3 = std::get<2>(std::get<1>((*it)->config));

      //(c1, c2, c3)
      c1 = std::get<0>(std::get<2>((*it)->config));
      c2 = std::get<1>(std::get<2>((*it)->config));
      c3 = std::get<2>(std::get<2>((*it)->config));

      if (a1 + b1 + c1 > 3.01 || a2 + b2 + c2 > 3.01 || a3 + b3 + c3 > 3.01)
        R3.erase(configIterator(&R3, (*it))); //erase if config exceeds constraints
    }
  } else {//this is if R3 fails in some way
    std::cout << "Error in R3CONSTRUCTION" << std::endl;
  }

  //simple test

  //Create R3 intersect L(alpha)
  int num, den;
  num = 2; den = 1;
  double alpha = num / den;
  std::set<Configuration*> R3_Overloaded = OVERLOADED(R3, alpha);
  std::cout << "Size of Overloaded Set: " << R3_Overloaded.size() << std::endl; // debug statement

  //Finally, check how many of these overloaded sets are just overflown.
  int count = 0;
  for (std::set<Configuration*>::iterator it = R3_Overloaded.begin(); R3_Overloaded.end(); it++) {
    if (OVERFLOWN((*it))) count++;
  }
  if (count == R3_Overloaded.size()) std::cout << "Success!" << std::endl;
}
