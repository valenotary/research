/*
Testing R3 stuff
m = d = 3
Similar to R2, but have to expand it to 3 dimensions / machines
*/

#include <utility> //std::pair
#include <iostream> //i/o
#include <string> // writing strings
#include <chrono> //timer
#include <math.h> //pow
#include <iterator> //std::set::iterator
#include <algorithm> //std::max and std::min
#include <set> // for std::set
#include <fstream> // writing to a file
#include <tuple> //std::tuple

typedef std::chrono::high_resolution_clock Clock;

struct Configuration {
  std::tuple<std::tuple<double, double, double>, std::tuple <double, double, double>, std::tuple <double, double, double>> config;
  Configuration(std::tuple<double, double, double> a, std::tuple<double, double, double> b, std::tuple<double, double, double> c): config(std::make_tuple(a, b, c)) {}
};

//rudimentary helper function that checks if a configuration has already been placed inside the R2 set
bool configFind(std::set<Configuration*> *R3, Configuration *c, int count) {
  double d1, d2, d3, e1, e2, e3, f1, f2, f3;

  //(d1, d2, d3)
  d1 = std::get<0>(std::get<0>(c->config)) + 1000000;
  d2 = std::get<1>(std::get<0>(c->config)) + 1000000;
  d3 = std::get<2>(std::get<0>(c->config)) + 1000000;

  //(e1, e2, e3)
  e1 = std::get<0>(std::get<1>(c->config)) + 1000000;
  e2 = std::get<1>(std::get<1>(c->config)) + 1000000;
  e3 = std::get<2>(std::get<1>(c->config)) + 1000000;

  //(f1, f2, f3)
  f1 = std::get<0>(std::get<2>(c->config)) + 1000000;
  f2 = std::get<1>(std::get<2>(c->config)) + 1000000;
  f3 = std::get<2>(std::get<2>(c->config)) + 1000000;

  for (std::set<Configuration*>::iterator it = R3->begin(); it != R3->end(); it++) {

    double a1, a2, a3, b1, b2, b3, c1, c2, c3;

    //(a1, a2, a3)
    a1 = std::get<0>(std::get<0>((*it)->config)) + 1000000;
    a2 = std::get<1>(std::get<0>((*it)->config)) + 1000000;
    a3 = std::get<2>(std::get<0>((*it)->config)) + 1000000;

    //(b1, b2, b3)
    b1 = std::get<0>(std::get<1>((*it)->config)) + 1000000;
    b2 = std::get<1>(std::get<1>((*it)->config)) + 1000000;
    b3 = std::get<2>(std::get<1>((*it)->config)) + 1000000;

    //(c1, c2, c3)
    c1 = std::get<0>(std::get<2>((*it)->config)) + 1000000;
    c2 = std::get<1>(std::get<2>((*it)->config)) + 1000000;
    c3 = std::get<2>(std::get<2>((*it)->config)) + 1000000;

    //Need to make this condition more general
    if ((a1 == d1 && a2 == d2 && a3 == d3 && b1 == e1 && b2 == e2 && b3 == e3 && c1 == f1 && c2 == f2 && c3 == f3)) {
      //std::cout << "Config found; skipping over..." << std::endl;
      return true;
    }
  }
  return false;
}

bool R3_Construct (double eps, std::set<Configuration*> *R3) {
  std::ofstream outputFile("R3Construction.txt");
  int count = 0;
  int dumb = 0;
  std::tuple<double, double, double> zero = std::make_tuple(0, 0, 0);
  R3->insert(new Configuration(zero, zero, zero));
  for(std::set<Configuration*>::iterator it = R3->begin(); it != R3->end(); it++) {
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

    //std::cout << "(" << a1 << ", " << a2 << ", " << a3 << ") ; (" << b1 << ", " << b2 << ", " << b3 << ") ; (" << c1 << ", " << c2 << ", " << c3 << ")\n";
    double l1, l2, l3;
    l1 = 3.00 - (double)(a1 + b1 + c1);
    if (l1 > 1.00) l1 = 1.0;
    l1 = 3.00 - (double)(a2 + b2 + c2);
    if (l2 > 1.00) l2 = 1.0;
    l1 = 3.00 - (double)(a3 + b3 + c3);
    if (l3 > 1.00) l3 = 1.0;

    // std::cout << "l1: " << l1 << std::endl;
    // std::cout << "l2: " << l2 << std::endl;
    // std::cout << "l3: " << l3 << std::endl;
    for (double g1 = 0; g1 <= l1; g1 += 1/eps) {
      for (double g2 = 0; g2 <= l2; g2 += 1/eps) {
        for (double g3 = 0; g3 <= l3; g3 += 1/eps) {
          if (g1 == 0 && g2 == 0 && g3 == 0) continue;
          if (a1 + b1 + c1 + g1 > 3.00 || a2 + b2 + c2 + g2 > 3.00 || a3 + b3 + c3 + g3 > 3.00) continue;
          //std::cout << "G: (" << g1 << ", " << g2 << ", " << g3 << ") " << std::endl;
          double aMax, bMax, cMax;
          aMax = std::max({a1 + g1, a2 + g2, a3 + g3});
          bMax = std::max({b1 + g1, b2 + g2, b3 + g3});
          cMax = std::max({c1 + g1, c2 + g2, c3 + g3});

          // std::cout << "aMax: " << aMax << std::endl;
          // std::cout << "bMax: " << bMax << std::endl;
          // std::cout << "cMax: " << cMax << std::endl;

          double minMax = std::min ({aMax, bMax, cMax});
          if (minMax == aMax) {
            //std::cout << "aMax is a minimum" << std::endl;
            ++count;
            Configuration*n = new Configuration(std::make_tuple(a1 + g1, a2 + g2, a3 + g3), std::make_tuple(b1, b2, b3), std::make_tuple (c1, c2, c3));
            if(!configFind(R3, n, count)) {
              outputFile << count << ": (" << a1 + g1 << ", " << a2 + g2 << ", " << a3 + g3 << ") ; (" << b1 << ", " << b2 << ", " << b3 << ") ; (" << c1 << ", " << c2 << ", " << c3 << ")\n";
              R3->insert(n);
            } //else {std::cout << "did not add config using aMax" << std::endl;}
          }
          if (minMax == bMax) {
            //std::cout << "bMax is a minimum" << std::endl;
            ++count;
            Configuration*n = new Configuration(std::make_tuple(a1, a2, a3), std::make_tuple(b1 + g1, b2 + g2, b3 + g3), std::make_tuple (c1, c2, c3));
            if(!configFind(R3, n, count)) {
              outputFile << count << ": (" << a1 << ", " << a2 << ", " << a3 << ") ; (" << b1 + g1 << ", " << b2 + g2 << ", " << b3 + g3 << ") ; (" << c1 << ", " << c2 << ", " << c3 << ")\n";
              R3->insert(n);
            } //else {std::cout << "did not add config using bMax" << std::endl;}
          }
          if (minMax == cMax) {
            //std::cout << "cMax is a minimum" << std::endl;
            ++count;
            Configuration*n = new Configuration(std::make_tuple(a1, a2, a3), std::make_tuple(b1, b2, b3), std::make_tuple (c1 + g1, c2 + g2, c3 + g3));
            if(!configFind(R3, n, count)) {
              outputFile << count << ": (" << a1 << ", " << a2 << ", " << a3 << ") ; (" << b1 << ", " << b2 << ", " << b3 << ") ; (" << c1 + g1 << ", " << c2 + g2 << ", " << c3 + g3 << ")\n";
              R3->insert(n);
            } //else {std::cout << "did not add config using cMax" << std::endl;}
          }
          // std::cout << "End of iteration. Type in any number and press enter to continue..." << std::endl;
          // std::cin >> dumb;
        }
      }
    }
  }
  std::cout << "Inserted " << count << std::endl;
  return true;
}


int main() {
  std::set<Configuration*> R3;
  double eps;

  std::cout << "Enter an epsilon value: ";
  std::cin >> eps;

  std::cout << "R3 maximum capacity: " << R3.max_size() << std::endl;
  //int count = 0;
  auto t1 = Clock::now();
  if (R3_Construct(eps, &R3)) {
    auto t2 = Clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "milliseconds" << std::endl;
    //std::cout << "Number of AT MOST expected elements in R2: " << (pow((2*eps + 1), 4) - 4) << std::endl;
    std::cout << "Number of elements in R3: " << R3.size() << std::endl;
  } else {std::cout << "error in R3_Construct!" << std::endl;}

}
