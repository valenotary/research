
#include <vector>
#include <cstdint>

#include "../properties.hpp"
#include "../representation.hpp"
#include "../masks/permutation.hpp"

#ifndef _T6_CONTEXT_RELATIVE_PERMUTATIVE
#define _T6_CONTEXT_RELATIVE_PERMUTATIVE

class Permutative_Mark
{
private:
	const Properties& p;
	const Representation& r;
	uint8_t * space;
	std::vector<size_t*> mask;
	size_t * holder;

public:
	Permutative_Mark(const Properties& p, const Representation& r, uint8_t* space) : p(p), r(r), space(space), holder(new size_t[p.degrees])
	{
		permutation_gen(p, mask);
		mask.erase(mask.begin(), mask.begin() + 1);
	}
	~Permutative_Mark()
	{
		mask.clear();
		delete[] holder;
	}

	void mark(size_t* configuration, size_t index, uint8_t bitMask) const;
};

void Permutative_Mark::mark(size_t* configuration, size_t index, uint8_t bitMask) const
{
	if (space[index] & p.INITIAL_BM)
	{
		for (auto it : mask)
		{
			for (size_t i = 0; i < p.degrees; ++i) holder[i] = configuration[it[i]];
			size_t index_p = r.index_configuration(holder);
			space[index_p] = space[index] | bitMask | p.PERM_BM;
		}
	}
}

#endif