
#include <cstdint>

#include "../properties.hpp"


#ifndef _T6_CONTEXT_RELATIVE_LIGHTER
#define _T6_CONTEXT_RELATIVE_LIGHTER

// mark lighter configurations with a bitMask
class Lighter_Mark
{
private:
	const Properties& p;
	uint8_t * space;

public:
	Lighter_Mark(const Properties& p, uint8_t* space) : p(p), space(space)
	{}

private:
	bool mark_h(size_t* configuration, size_t index, size_t layer, uint8_t bitMask) const;

public:
	void mark(size_t* configuration, size_t index, uint8_t bitMask) const;

};

bool Lighter_Mark::mark_h(size_t* configuration, size_t index, size_t layer, uint8_t bitMask) const
{
	if (layer == p.degrees - 1)
	{
		size_t maximum = configuration[layer];
		for (int i = maximum; i >= 0; --i) // int because 0 - 1 == UINT(MAX)... and loop keeps going
		{
			if (!(space[index] & bitMask))
			{
				space[index] = space[index] | bitMask;
			}
			else
			{
				return i == maximum;
			}
			--index;
		}
		return false;
	}
	else
	{
		size_t maximum = configuration[layer];
		for (int i = maximum; i >= 0; --i) // int because 0 - 1 == UINT(MAX)... and loop keeps going
		{
			bool maxed_layer = mark_h(configuration, index, layer + 1, bitMask);
			if (maxed_layer && i == maximum)
			{
				return true;
			}
			index = index - p.position_scales[layer];
		}
		return false;
	}
}

void Lighter_Mark::mark(size_t* configuration, size_t index, uint8_t bitMask) const
{
	mark_h(configuration, index, 0, bitMask);
}

#endif