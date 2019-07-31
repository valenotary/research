/*
Practice for generating random numbers
Need to try generating a random integer multiple [0, 40]


*/

#include <iostream>
#include <random>
//#include <cstdlib> (should not use this for uniform distribution)
//#include <ctime>

int main() {
  //srand(time(NULL));
  const int a = 0;
  const int b = 40;
  std::random_device rand_dev;
  std::mt19937 generator(rand_dev());
  std::uniform_int_distribution<int> distr(a, b);

  for (int i = 0; i < 20; i++) {
    std::cout << distr(generator) << std::endl;
  }
}
