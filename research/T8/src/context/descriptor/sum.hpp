
#include "../properties.hpp"

#ifndef _T6_CONTEXT_DESCRIPTOR_SUM
#define _T6_CONTEXT_DESCRIPTOR_SUM

class Sum
{
public:
	const Properties& p;

	Sum(const Properties& p) : p(p)
	{}

	void dimensions(size_t* configuration, size_t* return_list) const;
	void dimensions_g(size_t* configuration, size_t* g, size_t* return_list) const;
	void machines(size_t* configuration, size_t* return_list) const;

};

void Sum::dimensions(size_t* configuration, size_t* return_list) const
{
	// sum loads on each dimension
	// size(configuration) == degrees
	// size(return_list) == dimensions
	for (size_t d = 0; d < p.dimensions; ++d)
	{
		size_t i = d;
		size_t s = 0;
		for (size_t m = 0; m < p.machines; ++m)
		{
			s = s + configuration[i];
			i = i + p.dimensions;
		}
		return_list[d] = s;
	}
}

void Sum::dimensions_g(size_t* configuration, size_t* g, size_t* return_list) const
{
	// sum loads on each dimension w/ job g
	// size(configuration) == degrees
	// size(g) == dimensions
	// size(return_list) == dimensions
	for (size_t d = 0; d < p.dimensions; ++d)
	{
		size_t i = d;
		size_t s = g[d];
		for (size_t m = 0; m < p.machines; ++m)
		{
			s = s + configuration[i];
			i = i + p.dimensions;
		}
		return_list[d] = s;
	}
}

void Sum::machines(size_t* configuration, size_t* return_list) const
{
	// sum loads on each machine
	// size(configuration) == degrees
	// size(return_list) == machines
	size_t i = 0;
	for (size_t m = 0; m < p.machines; ++m)
	{
		size_t s = 0;
		for (size_t d = 0; d < p.dimensions; ++d)
		{
			s = s + configuration[i++];
		}
		return_list[m] = s;
	}
}

#endif