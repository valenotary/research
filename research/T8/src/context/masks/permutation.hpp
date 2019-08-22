
#include <vector>
#include <algorithm>

#include "../properties.hpp"

#ifndef _T6_CONTEXT_MASKS_PERMUTATION
#define _T6_CONTEXT_MASKS_PERMUTATION

// generate all permutations of machine & dimension order
void permutation_gen(const Properties& p, std::vector<size_t*>& return_mask)
{
	// permutations of machines
	size_t * order_machines = new size_t[p.machines];
	for (size_t i = 0; i < p.machines; ++i) order_machines[i] = i + 1;
	std::vector<size_t*> perm_machines;
	do {
		size_t * single_perm_machines = new size_t[p.machines];
		std::copy(order_machines, order_machines + p.machines, single_perm_machines);
		perm_machines.push_back(single_perm_machines);
	} while (std::next_permutation(order_machines, order_machines + p.machines));
	delete[] order_machines;

	// permutations of dimensions
	size_t * order_dimensions = new size_t[p.dimensions];
	for (size_t i = 0; i < p.dimensions; ++i) order_dimensions[i] = i + 1;
	std::vector<size_t*> perm_dimensions;
	do {
		size_t * single_perm_dimensions = new size_t[p.dimensions];
		std::copy(order_dimensions, order_dimensions + p.dimensions, single_perm_dimensions);
		perm_dimensions.push_back(single_perm_dimensions);
	} while (std::next_permutation(order_dimensions, order_dimensions + p.dimensions));
	delete[] order_dimensions;

	// create all m! * d! permutations
	for (auto it_M : perm_machines)
	{
		for (auto it_D : perm_dimensions)
		{
			size_t * single_perm_mask = new size_t[p.machines * p.dimensions];
			for (size_t i_m = 0; i_m < p.machines; ++i_m)
			{
				for (size_t i_d = 0; i_d < p.dimensions; ++i_d)
				{
					single_perm_mask[i_m * p.dimensions + i_d] = p.dimensions * (it_M[i_m] - 1) + it_D[i_d] - 1;
				}
			}
			return_mask.push_back(single_perm_mask);
		}
	}

	perm_machines.clear(); perm_dimensions.clear();
}

void test_perm_1()
{
	std::vector<size_t*> holder;
	for (size_t m = 1; m <= 3; ++m)
	{
		for (size_t d = 1; d <= 3; ++d)
		{
			Properties p(m, d, 1);
			permutation_gen(p, holder);
			std::cout << "(M = " << m << ", D = " << d << "):\n";
			for (size_t it = 0; it < holder.size(); ++it)
			{
				std::cout << "\t" << it + 1 << "\t| ";
				for (size_t i = 0; i < p.degrees; ++i) std::cout << holder[it][i] << '\t';
				std::cout << '\n';
			}
			std::cout << '\n';
			holder.clear();
		}
	}
}

#endif