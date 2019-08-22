
#include <cstdint>
#include <vector>
#include <functional>


#ifndef _H_CONTEXT
#define _H_CONTEXT

struct PriorityAssigner;

struct Context {

	Context(size_t machines, size_t dimensions, size_t discretizations, PriorityAssigner* pa);
	~Context();

	// discretizations = 1/eps
	size_t machines, dimensions, discretizations;
	// degrees = machines * dimensions, max = machines * discretizations | upper limit before configuration becomes invalid
	size_t degrees, max;
	// num_discret_conf = # of possible load values (including 0) in searchable space, volume = size of searchable space
	size_t num_discret_conf, volume;
	// pre-allocated filling operations stacked space
	size_t *ops_conf, *ops_choosen_machines, *ops_g, *ops_permutation, *ops_dimensional_sum, *ops_conf_printer;
	size_t *ops_conf_p, *ops_choosen_machines_p, *ops_g_p, *ops_permutation_p, *ops_dimensional_sum_p, *ops_conf_printer_p;

	// priority assignment function
	std::function<size_t (size_t* configuration, size_t* g, size_t* return_machines)> paf;

	// searchable space
	uint8_t* space;

	// permuations_mask = pregenerated machine & dimension permutations, position_scales = pregenerated index offsets of different configurations
	std::vector<size_t*> permuations_mask;
	size_t* position_scales;

public:

	// ................ representation conversion ................

	size_t index_configuration(size_t* configuration);
	void reverse_index(size_t index, size_t* return_conf);

	// ................ printing ................

	void print_conf(size_t* conf);
	void print_index(size_t index);
	void print(size_t * conf, size_t index);

	// ................ permutative marking ................

	void mark_permutative_configurations(size_t* configuration, size_t index);

	// ................ desc ................

	void sum_dimensions(size_t* configuration, size_t* return_list);
	void sum_dimensions_g(size_t* configuration, size_t* g, size_t* return_list);

	void sum_machines(size_t* configuration, size_t* return_list);

	void norm_inf_machines(size_t* configuration, size_t* return_list);
	void norm_inf_machines_g(size_t* configuration, size_t* g, size_t* return_list);

	bool is_valid_conf(size_t* configuration);
	bool is_valid_conf_g(size_t* configuration, size_t* g);

	size_t index_combine_conf_g(size_t index, size_t* g, size_t chosen_machine);

	// ................ evaluate ................

	bool set_configuration(size_t* configuration, size_t index);
	void recursive_g(size_t* configuration, size_t index, size_t* dimensional_sum, size_t* g, size_t* mod_configuration, size_t layer, size_t* return_machines, bool stacked_zero);
	void fill_space(size_t* configuration, size_t index);
	void full_fill_space();

};

// ................ Priority Function Interface ................

struct PriorityAssigner
{
	virtual size_t worst_case_limit(Context* c) { return c->machines * c->discretizations; } // if lower, this will not be enforced - should be instrinsic to algorithm
	virtual std::function<size_t (size_t* configuration, size_t* g, size_t* return_machines)> factory(Context* c) = 0; // return # of selected machines
};

// ................ make searchable space ................

uint8_t * mk_space(size_t volume)
{
	uint8_t * space = new uint8_t[volume];
	std::fill(space, space + volume, 0);
	return space;
}

// ................ generate all permutations of machine & dimension order ................

void permutations_mask_gen(size_t machines, size_t dimensions, std::vector<size_t*>& return_mask)
{
	// permutations of machines
	size_t * order_machines = new size_t[machines];
	for (size_t i = 0; i < machines; ++i) order_machines[i] = i + 1;
	std::vector<size_t*> perm_machines;
	do {
		size_t * single_perm_machines = new size_t[machines];
		std::copy(order_machines, order_machines + machines, single_perm_machines);
		perm_machines.push_back(single_perm_machines);
	} while (std::next_permutation(order_machines, order_machines + machines));
	delete[] order_machines;

	// permutations of dimensions
	size_t * order_dimensions = new size_t[dimensions];
	for (size_t i = 0; i < dimensions; ++i) order_dimensions[i] = i + 1;
	std::vector<size_t*> perm_dimensions;
	do {
		size_t * single_perm_dimensions = new size_t[dimensions];
		std::copy(order_dimensions, order_dimensions + dimensions, single_perm_dimensions);
		perm_dimensions.push_back(single_perm_dimensions);
	} while (std::next_permutation(order_dimensions, order_dimensions + dimensions));
	delete[] order_dimensions;

	// create all m! * d! permutations
	for (auto it_M : perm_machines)
	{
		for (auto it_D : perm_dimensions)
		{
			size_t * single_perm_mask = new size_t[machines * dimensions];
			for (size_t i_m = 0; i_m < machines; ++i_m)
			{
				for (size_t i_d = 0; i_d < dimensions; ++i_d)
				{
					single_perm_mask[i_m * dimensions + i_d] = dimensions * (it_M[i_m] - 1) + it_D[i_d] - 1;
				}
			}
			return_mask.push_back(single_perm_mask);
		}
	}

	perm_machines.clear(); perm_dimensions.clear();
}

#endif
