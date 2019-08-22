
#include "../properties.hpp"

#ifndef _T6_CONTEXT_DESCRIPTOR_VALID
#define _T6_CONTEXT_DESCRIPTOR_VALID

class Valid
{
public:
	const Properties& p;

	Valid(const Properties& p) : p(p)
	{}

	bool conf(size_t* configuration) const;
	bool conf_g(size_t* configuration, size_t* g) const;

};

bool Valid::conf(size_t* configuration) const
{
	// determine if configuration is valid
	// size(configuration) == degrees
	for (size_t d = 0; d < p.dimensions; ++d)
	{
		size_t i = d;
		size_t s = 0;
		for (size_t m = 0; m < p.machines; ++m)
		{
			s = s + configuration[i];
			i = i + p.dimensions;
		}
		if (s > p.max) return false;
	}
	return true;
}

bool Valid::conf_g(size_t* configuration, size_t* g) const
{
	// determine if configuration is valid w/ job g
	// size(configuration) == degrees
	// size(g) == dimensions
	for (size_t d = 0; d < p.dimensions; ++d)
	{
		size_t i = d;
		size_t s = g[d];
		for (size_t m = 0; m < p.machines; ++m)
		{
			s = s + configuration[i];
			i = i + p.dimensions;
		}
		if (s > p.max) return false;
	}
	return true;
}

#endif