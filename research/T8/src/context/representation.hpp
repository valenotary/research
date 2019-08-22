
#include <cstdint>

#include "properties.hpp"

#ifndef _T6_CONTEXT_REPRESENTATION
#define _T6_CONTEXT_REPRESENTATION


class Representation
{
private:
	const Properties& p;

public:
	Representation(const Properties& p) : p(p)
	{}

	size_t index_configuration(size_t* configuration) const;
	void reverse_index(size_t index, size_t* return_conf) const;
	size_t index_combine_configuration_g(size_t index, size_t* g, size_t chosen_machine) const;

};

// turn index into configuration
size_t Representation::index_configuration(size_t* configuration) const
{
	size_t index = 0;
	for (size_t i = 0; i < p.degrees; ++i) index = index + configuration[i] * p.position_scales[i];
	return index;
}

// reverse index into configuration
void Representation::reverse_index(size_t index, size_t* return_conf) const
{
	for (size_t i = 0; i < p.degrees; ++i)
	{
		size_t remainder = index % (p.max + 1);
		return_conf[p.degrees - i - 1] = remainder;
		index = index / (p.max + 1);
	}
}

// index of configuration after assigning load g to chosen_machine
size_t Representation::index_combine_configuration_g(size_t index, size_t* g, size_t chosen_machine) const
{
	size_t i = chosen_machine * p.dimensions;
	for (size_t j = 0; j < p.dimensions; ++j) index = index + g[j] * p.position_scales[i++];
	return index;
}

#endif