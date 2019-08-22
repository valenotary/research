#include <iostream>
#include <utility>


int main() {
  std::pair<std::pair<double, double>, std::pair<double, double>> test;
  test = std::make_pair(std::make_pair(2.5, 3.5), std::make_pair(1.5, 1.2));
  std::cout << test.first.first << std::endl;
  std::cout << test.first.second << std::endl;
  std::cout << test.second.first << std::endl;
  std::cout << test.second.second << std::endl;
}
