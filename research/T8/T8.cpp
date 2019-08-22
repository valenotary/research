
#include "src/m0.hpp"

int main(int argc, char **argv)
{
	int m_d = 0; int eps;
	std::cout << "Machines/Dimensions: ";
	std::cin >> m_d;
	std::cout << "1/eps: ";
	std::cin >> eps;

	Context c1(m_d, m_d, eps, Greedy_Makespace::F());
	std::cout << c1.p << '\n';
	c1.analyze();
	c1.printCount();
	std::cout << "\n\n";

	// Context c2(2, 2, 30, N_Norm::F(3));
	// std::cout << c2.p << '\n';
	// c2.analyze();
	// c2.printCount();
	// std::cout << "\n\n";

	// Context c3(2, 2, 30, Exponential::F(1.7));
	// std::cout << c3.p << '\n';
	// c3.analyze();
	// c3.printCount();
	// std::cout << "\n\n";

}
