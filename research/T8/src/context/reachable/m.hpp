

#include "../properties.hpp"
#include "../representation.hpp"
#include "../relative/permutative.hpp"
#include "../descriptor/sum.hpp"
#include "../priority/assigner.hpp"

#ifndef _T6_CONTEXT_REACHABLE
#define _T6_CONTEXT_REACHABLE

class Reachable
{
private:
	const Properties& p;
	const Representation& r;
	const Permutative_Mark& pm;
	const Assigner& a;
	const Sum& sum;
	uint8_t * space;

	size_t *ops_conf, *ops_conf_start;
	size_t *ops_chosen_machines, *ops_chosen_machines_start;
	size_t *ops_g, *ops_g_start;
	size_t *ops_dimensional_sum, *ops_dimensional_sum_start;

public:
	Reachable(const Properties& p, const Representation& r, uint8_t * space, const Assigner& a) : p(p), r(r), space(space),
		pm(*new Permutative_Mark(p, r, space)), a(a), sum(*new Sum(p))
	{
		this->ops_conf_start = new size_t[p.degrees * (p.dimensions * p.max * (p.degrees + 2))];
		this->ops_chosen_machines_start = new size_t[p.machines * (p.dimensions * p.max * (p.degrees + 2))];
		this->ops_g_start = new size_t[p.dimensions * (p.dimensions * p.max * (p.degrees + 2))];
		this->ops_dimensional_sum_start = new size_t[p.dimensions * (p.dimensions * p.max * (p.degrees + 2))];

		this->ops_conf = this->ops_conf_start;
		this->ops_chosen_machines = this->ops_chosen_machines_start;
		this->ops_g = this->ops_g_start;
		this->ops_dimensional_sum = this->ops_dimensional_sum_start;
	}

	~Reachable()
	{
		delete[] ops_conf_start;
		delete[] ops_chosen_machines_start;
		delete[] ops_g_start;
		delete[] ops_dimensional_sum_start;
	}

private:
	bool set_reachable(size_t* configuration, size_t index) const;
	void recursive_g(size_t* configuration, size_t index, size_t* dimensional_sum, size_t* g,
		size_t* mod_configuration, size_t layer, size_t* return_machines, bool stacked_zero);
	void fill_space(size_t* configuration, size_t index);

public:
	void full_fill_space();

};

bool Reachable::set_reachable(size_t* configuration, size_t index) const
{
	if (space[index] & p.REACHABLE_BM) return false;

	space[index] = p.REACHABLE_BM | p.INITIAL_BM;
	pm.mark(configuration, index, p.REACHABLE_BM);
	space[index] = p.REACHABLE_BM | p.PREV_INITIAL_BM;

	return true;
}

void Reachable::recursive_g(size_t* configuration, size_t index, size_t* dimensional_sum, size_t* g,
	size_t* mod_configuration, size_t layer, size_t* return_machines, bool stacked_zero)
{
	size_t upper_g = (p.max - dimensional_sum[layer]) < p.discretizations ? p.max - dimensional_sum[layer] : p.discretizations;

	if (layer == p.dimensions - 1)
	{
		for (size_t l = 0; l <= upper_g; ++l)
		{
			if (stacked_zero) { stacked_zero = false; continue; }

			g[layer] = l;

			size_t selected_machines_count = a.pick(configuration, g, return_machines);
			for (size_t i = 0; i < selected_machines_count; ++i)
			{
				size_t selected_machine = return_machines[i];

				size_t mc_i = selected_machine * p.dimensions;
				for (size_t d = 0; d < p.dimensions; ++d)
				{
					mod_configuration[mc_i] = configuration[mc_i] + g[d];
					mc_i++;
				}

				size_t index_g = r.index_combine_configuration_g(index, g, selected_machine);
				fill_space(mod_configuration, index_g);

				mc_i = selected_machine * p.dimensions;
				for (size_t d = 0; d < p.dimensions; ++d)
				{
					mod_configuration[mc_i] = configuration[mc_i];
					mc_i++;
				}

			}
		}
	}
	else
	{
		for (size_t l = 0; l <= upper_g; ++l)
		{
			g[layer] = l;
			recursive_g(configuration, index, dimensional_sum, g, mod_configuration, layer + 1, return_machines, stacked_zero);
			stacked_zero = false;
		}
	}
}

void Reachable::fill_space(size_t* configuration, size_t index)
{
	if (!set_reachable(configuration, index)) return;

	ops_conf += p.degrees;
	std::copy(configuration, configuration + p.degrees, ops_conf);
	ops_g += p.dimensions;
	ops_dimensional_sum += p.dimensions;
	sum.dimensions(configuration, ops_dimensional_sum);
	ops_chosen_machines += p.machines;

	recursive_g(configuration, index, ops_dimensional_sum, ops_g, ops_conf, 0, ops_chosen_machines, true);

	ops_conf -= p.degrees;
	ops_g -= p.dimensions;
	ops_dimensional_sum -= p.dimensions;
	ops_chosen_machines -= p.machines;

}

void Reachable::full_fill_space()
{
	size_t index = 0;
	ops_conf += p.degrees;
	std::fill(ops_conf, ops_conf + p.degrees, 0);
	fill_space(ops_conf, index);
	ops_conf -= p.degrees;
}

#endif