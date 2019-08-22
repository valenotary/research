
#include <cstdint>

#include "../properties.hpp"

#ifndef _T6_CONTEXT_RELATIVE_HEAVIER
#define _T6_CONTEXT_RELATIVE_HEAVIER


// mark heavier configurations with a bitMask
class Heavier_Mark
{
private:
	const Properties& p;
	uint8_t * space;

public:
	Heavier_Mark(const Properties& p, uint8_t* space) : p(p), space(space)
	{}

private:
	bool mark_h(size_t* configuration, size_t index, size_t layer, uint8_t bitMask) const;

public:
	void mark(size_t* configuration, size_t index, uint8_t bitMask) const;

};

bool Heavier_Mark::mark_h(size_t* configuration, size_t index, size_t layer, uint8_t bitMask) const
{
	if (layer == p.degrees - 1)
	{
		size_t minimum = configuration[layer];
		for (size_t i = minimum; i <= p.max; ++i)
		{
			if (!(space[index] & bitMask))
			{
				space[index] = space[index] | bitMask;
			}
			else
			{
				return i == minimum;
			}
			++index;
		}
		return false;
	}
	else
	{
		size_t minimum = configuration[layer];
		for (size_t i = minimum; i <= p.max; ++i)
		{
			bool maxed_layer = mark_h(configuration, index, layer + 1, bitMask);
			if (maxed_layer && i == minimum)
			{
				return true;
			}
			index = index + p.position_scales[layer];
		}
		return false;
	}
}

void Heavier_Mark::mark(size_t* configuration, size_t index, uint8_t bitMask) const
{
	mark_h(configuration, index, 0, bitMask);
}


#endif