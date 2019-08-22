
#include "../properties.hpp"

#ifndef _T6_CONTEXT_DESCRIPTOR_NORMINF
#define _T6_CONTEXT_DESCRIPTOR_NORMINF

class Norm_Inf
{
public:
	const Properties& p;

	Norm_Inf(const Properties& p) : p(p)
	{}

	void machines(size_t* configuration, size_t* return_list) const;
	void machines_g(size_t* configuration, size_t* g, size_t* return_list) const;

};

void Norm_Inf::machines(size_t* configuration, size_t* return_list) const
{
	// norm infinity of each machine
	// size(configuration) == degrees
	// size(return_list) == machines
	size_t i = 0;
	for (size_t m = 0; m < p.machines; ++m)
	{
		size_t biggest = configuration[i++];
		for (size_t d = 1; d < p.dimensions; ++d)
		{
			size_t current_i = configuration[i++];
			if (current_i > biggest) biggest = current_i;
		}
		return_list[m] = biggest;
	}
}

void Norm_Inf::machines_g(size_t* configuration, size_t* g, size_t* return_list) const
{
	// norm infinity of each machine w/ job g
	// size(configuration) == degrees
	// size(g) == dimensions
	// size(return_list) == machines
	size_t i = 0;
	for (size_t m = 0; m < p.machines; ++m)
	{
		size_t biggest = configuration[i++] + g[0];
		for (size_t d = 1; d < p.dimensions; ++d)
		{
			size_t current_i = configuration[i++] + g[d];
			if (current_i > biggest) biggest = current_i;
		}
		return_list[m] = biggest;
	}
}


#endif