
#include <cstdint>
#include <functional>

#include "context.hpp"

#ifndef _H_ACTIVE
#define _H_ACTIVE

struct Greedy_Makespace : public PriorityAssigner
{
	// ................ return holders ................
	size_t * r_norm_inf;

	// ................ worst case limit ................
	size_t worst_case_limit(Context* c) {
		//if (c->machines == 1) return c->discretizations;
		//else return 2 * c->discretizations;
		return c->machines * c->discretizations;
	}

	// ................ priority assignment function ................
	std::function<size_t(size_t* configuration, size_t* g, size_t* return_machines)> factory(Context* c)
	{
		// initialize return holders
		r_norm_inf = new size_t[c->machines];

		// paf factory
		return [c, this](size_t* configuration, size_t* g, size_t* return_machines) {

			c->norm_inf_machines_g(configuration, g, this->r_norm_inf);

			size_t machines_picked = 1;
			size_t min_makespan = r_norm_inf[0];
			return_machines[0] = 0;

			for (size_t m = 1; m < c->machines; ++m)
			{
				if (r_norm_inf[m] < min_makespan)
				{
					machines_picked = 0;
					min_makespan = r_norm_inf[m];
					return_machines[machines_picked++] = m;
				}
				else if (r_norm_inf[m] == min_makespan)
				{
					return_machines[machines_picked++] = m;
				}
			}

			return machines_picked;

		};

	}
};

#endif