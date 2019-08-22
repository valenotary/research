#include <cstdint>
#include <math.h>
#include <algorithm>
#include <vector>
#include <bitset>
#include <iostream>

//#include <chrono>

#include "misc.hpp"
#include "context.hpp"

#ifndef _H_CONTEXT_FILL
#define _H_CONTEXT_FILL

#define UNREACHABLE 0b00000000
#define REACHABLE 0b10000000

#define PERM 0b00000001

#define INITIAL 0b00000100
#define PREV_INITIAL 0b00001000

#define REACHABLE_PERM REACHABLE | PERM

#define OVERLOADED 0b00010000
#define OVERFLOWN 0b00100000

// for comparison of w/ or w/o perm
bool use_perm = true;
size_t pc = 0;
//size_t runC = 0;
//size_t runCH = 0;

// ================= CONTEXT =================

inline Context::Context(size_t machines, size_t dimensions, size_t discretizations, PriorityAssigner* pa)
{
	this->machines = machines; this->dimensions = dimensions; this->discretizations = discretizations;

	this->degrees = machines * dimensions;
	this->max = machines * discretizations;

	this->num_discret_conf = pa->worst_case_limit(this) + 1;
	this->volume = ipow(num_discret_conf, degrees);

	this->ops_conf = new size_t[degrees * (dimensions * max * (degrees + 2))];
	this->ops_choosen_machines = new size_t[machines * (dimensions * max * (degrees + 2))];
	this->ops_g = new size_t[dimensions * (dimensions * max * (degrees + 2))];
	this->ops_dimensional_sum = new size_t[dimensions * (dimensions * max * (degrees + 2))];
	this->ops_permutation = new size_t[degrees]; // this one doesn't need to be shifted, just one set
	this->ops_conf_printer = new size_t[degrees];

	this->ops_conf_p = ops_conf;
	this->ops_choosen_machines_p = ops_choosen_machines;
	this->ops_g_p = ops_g;
	this->ops_dimensional_sum_p = ops_dimensional_sum;
	this->ops_permutation_p = ops_permutation;
	this->ops_conf_printer_p = ops_conf_printer;

	this->paf = pa->factory(this);

	this->space = mk_space(this->volume);

	permutations_mask_gen(this->machines, this->dimensions, this->permuations_mask);
	permuations_mask.erase(permuations_mask.begin(), permuations_mask.begin() + 1); // delete initial order

	this->position_scales = new size_t[degrees];
	for (size_t i = 0; i < degrees; ++i) this->position_scales[i] = ipow(num_discret_conf, degrees - i - 1);
}


inline Context::~Context()
{
	permuations_mask.clear();
	delete[] space, position_scales;
	delete[] ops_conf, ops_choosen_machines, ops_g, ops_dimensional_sum, ops_permutation, ops_conf_printer;
}

// ================= REPRESENTATION CONVERSION =================


inline size_t Context::index_configuration(size_t* configuration)
{
	size_t index = 0;
	for (size_t i = 0; i < degrees; ++i) index = index + configuration[i] * position_scales[i];
	return index;
}


inline void Context::reverse_index(size_t index, size_t * return_conf)
{
	for (size_t i = 0; i < degrees; ++i)
	{
		size_t remainder = index % num_discret_conf;
		return_conf[degrees - i - 1] = remainder;
		index = index / num_discret_conf;
	}
}

// ================= PRINTING =================


inline void Context::print_conf(size_t * conf)
{
	print(conf, index_configuration(conf));
}


inline void Context::print_index(size_t index)
{
	if (index < volume)
	{
		reverse_index(index, ops_conf_printer_p);
		print(ops_conf_printer_p, index);
	}
	else std::cout << "ERROR: index (" << index << ") out of bounds";
}


inline void Context::print(size_t * conf, size_t index)
{
	std::cout << '[';
	for (size_t m = 0; m < machines - 1; ++m)
	{
		std::cout << '(';
		for (size_t d = 0; d < dimensions - 1; ++d)
		{
			std::cout << conf[m * dimensions + d] << ", ";
		}
		std::cout << conf[m * dimensions + dimensions - 1] << "); ";
	}
	std::cout << '(';
	for (size_t d = 0; d < dimensions - 1; ++d)
	{
		std::cout << conf[(machines - 1) * dimensions + d] << ", ";
	}
	std::cout << conf[(machines - 1) * dimensions + dimensions - 1] << ")] | ";

	std::cout << index << " : " << std::bitset<8>(space[index]);
}

// ================= PERMUTATIVE MARKING =================


inline void Context::mark_permutative_configurations(size_t* configuration, size_t index)
{
	if (space[index] & INITIAL)
	{
		for (auto it : permuations_mask)
		{
			for (size_t i = 0; i < degrees; ++i) ops_permutation_p[i] = configuration[it[i]];
			size_t index_p = index_configuration(ops_permutation_p);
			//if (index_p >= volume)
			//{
			//	//std::cout << ops_g_p[0] << '\t' << ops_g_p[1] << '\t' << ops_g_p[2] << '\n';
			//	//print_conf(configuration);
			//	//print_conf(ops_permutation_p);
			//	size_t * ig = ops_g + dimensions;
			//	for (size_t* ip = ops_conf + degrees; ip <= ops_conf_p; ip += degrees)
			//	{
			//		print_conf(ip);
			//		std::cout << "\t||\t" << ig[0] << '\t' << ig[1] << '\t' << ig[2] << '\n';
			//		//std::cout << '\n';
			//		ig += dimensions;
			//	}
			//	print_conf(ops_permutation_p);
			//}
			space[index_p] = REACHABLE_PERM;
		}
	}
}

// ================= EVALUATE =================

inline bool Context::set_configuration(size_t* configuration, size_t index)
{
	if (space[index]) return false;

	pc++; // count calculations

	space[index] = REACHABLE | INITIAL;
	if (use_perm) mark_permutative_configurations(configuration, index); // if(use_perm) => perm testing, delete normally
	space[index] = REACHABLE | PREV_INITIAL;

	return true;
}


inline void Context::recursive_g(size_t* configuration, size_t index, size_t* dimensional_sum, size_t* g,
	size_t* mod_configuration, size_t layer, size_t* return_machines, bool stacked_zero)
{
	size_t upper_g = (max - dimensional_sum[layer]) < discretizations ? max - dimensional_sum[layer] : discretizations;

	if (layer == dimensions - 1)
	{
		for (size_t l = 0; l <= upper_g; ++l)
		{
			if (stacked_zero) { stacked_zero = false; continue; }

			g[layer] = l;

			size_t selected_machines_count = paf(configuration, g, return_machines);
			for (size_t i = 0; i < selected_machines_count; ++i)
			{
				size_t selected_machine = return_machines[i];

				size_t mc_i = selected_machine * dimensions;
				for (size_t d = 0; d < dimensions; ++d)
				{
					mod_configuration[mc_i] = configuration[mc_i] + g[d];
					mc_i++;
				}

				size_t index_g = index_combine_conf_g(index, g, selected_machine);
				fill_space(mod_configuration, index_g);

				mc_i = selected_machine * dimensions;
				for (size_t d = 0; d < dimensions; ++d)
				{
					mod_configuration[mc_i] = configuration[mc_i];
					mc_i++;
				}

			}
		}
	}
	else
	{
		for (size_t l = 0; l <= upper_g; ++l) // CHANGE TO 0
		{
			g[layer] = l;
			recursive_g(configuration, index, dimensional_sum, g, mod_configuration, layer + 1, return_machines, stacked_zero);
			stacked_zero = false;
		}
	}
}

inline void Context::fill_space(size_t* configuration, size_t index)
{
	if (!set_configuration(configuration, index)) return;

	//std::cout << ++runC << '\t';
	//if (runC > runCH) runCH = runC;
	//std::cout << runCH << '\n';

	ops_conf_p += degrees;
	std::copy(configuration, configuration + degrees, ops_conf_p);
	ops_g_p += dimensions;
	ops_dimensional_sum_p += dimensions;
	sum_dimensions(configuration, ops_dimensional_sum_p);
	ops_choosen_machines_p += machines;

	recursive_g(configuration, index, ops_dimensional_sum_p, ops_g_p, ops_conf_p, 0, ops_choosen_machines_p, true); // CHANGE TO TRUE

	ops_conf_p -= degrees;
	ops_g_p -= dimensions;
	ops_dimensional_sum_p -= dimensions;
	ops_choosen_machines_p -= machines;
	
	//--runC;
}

inline void Context::full_fill_space()
{
	size_t index = 0;
	ops_conf_p += degrees;
	std::fill(ops_conf_p, ops_conf_p + degrees, 0);
	fill_space(ops_conf_p, index);
	ops_conf_p -= degrees;
}

/*
std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	for (size_t r = 0; r < 100000; ++r)
	{
	}
std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
std::cout << duration << '\n';
*/


#endif
