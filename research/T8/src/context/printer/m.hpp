
#include <bitset>

#include "../properties.hpp"
#include "../representation.hpp"

#ifndef _T6_CONTEXT_PRINTER
#define _T6_CONTEXT_PRINTER

class Printer
{
private:
	const Properties& p;
	const Representation& r;
	uint8_t * space;
	size_t * holder;

public:
	Printer(const Properties& p, const Representation& r, uint8_t * space) : p(p), r(r), space(space)
	{
		holder = new size_t[p.degrees];
	}

	~Printer()
	{
		delete[] holder;
	}

	void configuration(size_t * configuration) const;
	void index(size_t pi) const;
	void both(size_t * configuration, size_t index) const;

};

void Printer::configuration(size_t * configuration) const
{
	size_t pi = r.index_configuration(configuration);
	both(configuration, pi);
}

void Printer::index(size_t pi) const
{
	r.reverse_index(pi, holder);
	both(holder, pi);
}

void Printer::both(size_t * configuration, size_t pi) const
{
	std::cout << '[';
	for (size_t m = 0; m < p.machines - 1; ++m)
	{
		std::cout << '(';
		for (size_t d = 0; d < p.dimensions - 1; ++d)
		{
			std::cout << configuration[m * p.dimensions + d] << ", ";
		}
		std::cout << configuration[m * p.dimensions + p.dimensions - 1] << "); ";
	}
	std::cout << '(';
	for (size_t d = 0; d < p.dimensions - 1; ++d)
	{
		std::cout << configuration[(p.machines - 1) * p.dimensions + d] << ", ";
	}
	std::cout << configuration[(p.machines - 1) * p.dimensions + p.dimensions - 1] << ")] | ";

	std::cout << pi << " : " << std::bitset<8>(space[pi]);
}

#endif