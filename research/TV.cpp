/*
Naive Implementation (with Eshant's suggestions)

Create R2 as a set with initially (0, 0) (0, 0)
for every Configuration in R2
  for every job vector in G where G <= 1 and G != (0, 0)
    if A + B + G <= 2
      add min(||A + G||_inf, ||B + G||_inf) to R2

- Remember: the maximum job in G is (1, 1)
- So, creating G doesn't involve having to rotate through a set of jobs
- Calculate maximum job given a config, then add up everything up to that point

For an eps < 10, everything works fine (all calculations like checking for overflown and overloaded configurations are correct), however
for some reason the machine hiccups and usually gets stuck trying to calculate R2 when using an epsilon >= 10 (at least, for my machines).

*/

#include <utility> // std::pair
#include <iostream> // i/o
#include <string> // writing strings
#include <chrono> // timer
#include <math.h> // pow
#include <iterator> // std::set::iterator
#include <algorithm> // std::max and std::min
#include <set> // for std::set
#include <fstream> // writing to a file

typedef std::chrono::high_resolution_clock Clock;

//config struct; objects put into R2 set
struct Configuration {
  std::pair<std::pair<double, double>, std::pair<double, double>> config;
  Configuration(std::pair<double, double> a, std::pair<double, double> b): config(std::make_pair(a, b)) {}
};

//rudimentary helper function that checks if a configuration has already been placed inside the R2 set
bool configFind(std::set<Configuration*> *R2, Configuration *c) {
  //int place = 0;
  double c1 = c->config.first.first + 1000000;
  double c2 = c->config.first.second + 1000000;
  double d1 = c->config.second.first + 1000000;
  double d2 = c->config.second.second + 1000000;
  for (std::set<Configuration*>::iterator it = R2->begin(); it != R2->end(); it++) {
    //place++;
    double a1 = (*it)->config.first.first + 1000000;
    double a2 = (*it)->config.first.second + 1000000;
    double b1 = (*it)->config.second.first + 1000000;
    double b2 = (*it)->config.second.second + 1000000;

	if ((a1 == c1 && a2 == c2 && b1 == d1 && b2 == d2)) {
      return true;
    }

  }
  return false;
}

//constructs the R2 set
bool R2_Construct (double eps, std::set<Configuration*> *R2) {
  //The maximum job G can have is (1, 1)
  std::ofstream outputFile("TV.txt");
  int count = 0;

  //first add (0, 0) ; (0, 0)
  R2->insert(new Configuration(std::make_pair(0, 0), std::make_pair(0, 0)));

  //iterate through configs in R2, simultaneously updated
  for (std::set<Configuration*>::iterator it = R2->begin(); it != R2->end(); it++) {
    //std::cout << "iterator loop" << std::endl;
    double a1, a2, b1, b2;

    //[(a1, a2) ; (b1, b2)] <- each config in the R2 set looks like this
    a1 = (*it)->config.first.first;
    a2 = (*it)->config.first.second;
    b1 = (*it)->config.second.first;
    b2 = (*it)->config.second.second;

    double l1, l2;
    l1 = 2.0 - (double)(a1 + b1);
    if (l1 > 1.0) l1 = 1.0;
    l2 = 2.0 - (double)(a2 + b2);
    if (l2 > 1.0) l2 = 1.0;

	for (double g1 = 0; g1 <= l1; g1 += 1 / eps) {
		for (double g2 = 0; g2 <= l2; g2 += 1 / eps) {
			if (g1 == 0 && g2 == 0) continue;
      if (a1 + g1 > 2.0 || a2 + g2 > 2.0) continue;

			double aMax, bMax;
			aMax = std::max(a1 + g1, a2 + g2);
			bMax = std::max(b1 + g1, b2 + g2);

      double trueMin = std::min(aMax, bMax);
      Configuration *n;
			if (trueMin == aMax) {
				//++count;
				n = new Configuration(std::make_pair(a1 + g1, a2 + g2), std::make_pair(b1, b2));
				if (!configFind(R2, n)) {outputFile << "(" << a1 + g1 << ", " << a2 + g2 <<") ; (" << b1 << ", " << b2 << ")\n"; R2->insert(n);}
			}
			if (trueMin == bMax) {
				//++count;
				n = new Configuration(std::make_pair(a1, a2), std::make_pair(b1 + g1, b2 + g2));
				if (!configFind(R2, n)) {outputFile << "(" << a1 << ", " << a2 <<") ; (" << b1 + g1 << ", " << b2 + g2 << ")\n"; R2->insert(n);}
			}
		}
	}
}
  outputFile.flush();
  outputFile.close();
  return true;
}

//Overloaded: if EVERY machine is > alpha - 1 on SOME dimension, then it is overloaded
std::set<Configuration*> OVERLOAD(std::set<Configuration*> &R2, double alpha) {
  std::set<Configuration*> Overloaded;
  int count = 0;
  for (std::set<Configuration*>::iterator it = R2.begin(); it != R2.end(); it++) {
    double a1 = (*it)->config.first.first;
    double a2 = (*it)->config.first.second;
    double b1 = (*it)->config.second.first;
    double b2 = (*it)->config.second.second;

    //comment this out
    // if (count++ % 50 == 0)
      // std::cout << "aMax = " << std::max(a1, a2) << " ; bMax = " << std::max(b1, b2) << std::endl;

    if (!(std::max(a1, a2) + 1.0 <= alpha) && !(std::max(b1, b2) + 1.0 <= alpha)) Overloaded.insert(new Configuration(std::make_pair(a1, a2), std::make_pair(b1, b2)));
  }
  return Overloaded;
}

// //Overflown: basically if the config creates a line segment that doesn't cross the unit box, then it is Overflown
std::set<Configuration*> OVERFLOWN(std::set<Configuration*> &R2_set) { // should be passing in overloaded set
    std::set<Configuration*> Overflown;
    for (std::set<Configuration*>::iterator it = R2_set.begin(); it != R2_set.end(); it++) {
      double x1, y1, x2, y2;
      x1 = (*it)->config.first.first;
      y1 = (*it)->config.first.second;
      x2 = (*it)->config.second.first;
      y2 = (*it)->config.second.second;

      //(a1, a2) (b1, b2) = (0, 0) (1, 1)

      //((y1−y2)(ax−x1)+(x2−x1)(ay−y1))((y1−y2)(bx−x1)+(x2−x1)(by−y1))<0.
      if (!((((y1-y2)*(0.0-x1)+(x2-x1)*(0.0-y1))*((y1-y2)*(1.0-x1)+(x2-x1)*(1.0-y1))) < 0.0)) {
        //std::cout << "OVERFLOWN: (" << x1 << ", " << y1 << ") ; (" << x2 << ", " << y2 << ")" << std::endl;
        Overflown.insert(new Configuration(std::make_pair(x1, y1), std::make_pair(x2, y2)));
      }
    }
    return Overflown;
}

//main driver function
int main() {
  std::set<Configuration*> R2;
  double eps;
  //no need to calculate G (?)
  std::cout << "Enter an epsilon value: ";
  std::cin >> eps;
  std::cout << "\n";

  std::cout << "Constructing R2..." << std::endl;
  std::cout << "Number of AT MOST expected elements in R2: " << (pow((2*eps + 1), 4) - 4) << std::endl;
  int count = 0;
  auto t1 = Clock::now(); //start timer
  if (R2_Construct(eps, &R2)) {
    auto t2 = Clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "milliseconds" << std::endl;
    std::cout << "Number of elements in R2: " << R2.size() << std::endl;

  } else {std::cout << "error in R2_Construct!" << std::endl;}
  //Overloaded and Overflown checks here

  //Overloaded set
  int num, den;
  std::cout << "num followed by den: ";
  std::cin >> num >> den;
  double alpha = (double) num / den;
  std::cout << "Construction R2 intersect L(" << num << "/" << den << ")..." << std::endl;
  std::cout << "Alpha: " << alpha << std::endl;
  std::set<Configuration*> R2_Overloaded = OVERLOAD(R2, alpha);
  std::cout << "Finished creating Overloaded set; Size of Overloaded set: " << R2_Overloaded.size() << std::endl;
  std::cout << "Checking Overloaded set for the Overflown values..." << std::endl;
  std::set<Configuration*> R2_Overflown = OVERFLOWN(R2_Overloaded);
  std::cout << "Overflown set size (from Overloaded): " << R2_Overflown.size() << std::endl;
  if (R2_Overflown.empty()) std::cout << "Success! Algorithm is " << num << "/" << den << "-competitive!" << std::endl;
}
