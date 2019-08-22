
#include "context.hpp"

#ifndef _H_CONTEXT_DESC
#define _H_CONTEXT_DESC

// ................ sum loads on each dimension ................

inline void Context::sum_dimensions(size_t* configuration, size_t* return_list) // size should be == dimensions
{
	for (size_t d = 0; d < dimensions; ++d)
	{
		size_t i = d;
		size_t s = 0;
		for (size_t m = 0; m < machines; ++m)
		{
			s = s + configuration[i];
			i = i + dimensions;
		}
		return_list[d] = s;
	}
}

inline void Context::sum_dimensions_g(size_t* configuration, size_t* g, size_t* return_list) // size should be == dimensions
{
	for (size_t d = 0; d < dimensions; ++d)
	{
		size_t i = d;
		size_t s = g[d];
		for (size_t m = 0; m < machines; ++m)
		{
			s = s + configuration[i];
			i = i + dimensions;
		}
		return_list[d] = s;
	}
}

// ................ sum loads on each machine ................

inline void Context::sum_machines(size_t* configuration, size_t* return_list) // size should be == machines
{
	size_t i = 0;
	for (size_t m = 0; m < machines; ++m)
	{
		size_t s = 0;
		for (size_t d = 0; d < dimensions; ++d)
		{
			s = s + configuration[i++];
		}
		return_list[m] = s;
	}
}

// ................ norm infinity of each machine ................

inline void Context::norm_inf_machines(size_t* configuration, size_t* return_list) // size should be == machines
{
	size_t i = 0;
	for (size_t m = 0; m < machines; ++m)
	{
		size_t biggest = configuration[i++];
		for (size_t d = 1; d < dimensions; ++d)
		{
			size_t current_i = configuration[i++];
			if (current_i > biggest) biggest = current_i;
		}
		return_list[m] = biggest;
	}
}

inline void Context::norm_inf_machines_g(size_t* configuration, size_t* g, size_t* return_list) // size should be == machines
{
	size_t i = 0;
	for (size_t m = 0; m < machines; ++m)
	{
		size_t biggest = configuration[i++] + g[0];
		for (size_t d = 1; d < dimensions; ++d)
		{
			size_t current_i = configuration[i++] + g[d];
			if (current_i > biggest) biggest = current_i;
		}
		return_list[m] = biggest;
	}
}

// ................ determine if configuration is valid ................

inline bool Context::is_valid_conf(size_t* configuration)
{
	for (size_t d = 0; d < dimensions; ++d)
	{
		size_t i = d;
		size_t s = 0;
		for (size_t m = 0; m < machines; ++m)
		{
			s = s + configuration[i];
			i = i + dimensions;
		}
		if (s > max) return false;
	}
	return true;
}

inline bool Context::is_valid_conf_g(size_t* configuration, size_t* g)
{
	for (size_t d = 0; d < dimensions; ++d)
	{
		size_t i = d;
		size_t s = g[d];
		for (size_t m = 0; m < machines; ++m)
		{
			s = s + configuration[i];
			i = i + dimensions;
		}
		if (s > max) return false;
	}
	return true;
}

// ................ index of configuration after assigning load g to chosen_machine ................

inline size_t Context::index_combine_conf_g(size_t index, size_t* g, size_t chosen_machine)
{
	size_t i = chosen_machine * dimensions;
	for (size_t j = 0; j < dimensions; ++j) index = index + g[j] * position_scales[i++];
	return index;
}

#endif