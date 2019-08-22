
#include "../properties.hpp"
#include "assigner.hpp"
#include "../descriptor/norm_inf.hpp"

#ifndef _T6_CONTEXT_PRIORITY_GREEDYMAKESPAN
#define _T6_CONTEXT_PRIORITY_GREEDYMAKESPAN

namespace Greedy_Makespace
{
	class A : public Assigner
	{
	private:
		size_t * holder;
		const Norm_Inf& ni;

	public:
		A(const Properties& p) : Assigner(p), ni(*new Norm_Inf(p))
		{
			this->holder = new size_t[p.machines];
		}

		~A()
		{
			delete[] holder;
			delete &ni;
		}

		size_t pick(size_t * configuration, size_t * g, size_t * return_machines) const
		{
			ni.machines_g(configuration, g, holder);

			size_t machines_picked = 1;
			size_t min_makespan = holder[0];
			return_machines[0] = 0;

			for (size_t m = 1; m < p.machines; ++m)
			{
				if (holder[m] < min_makespan)
				{
					machines_picked = 0;
					min_makespan = holder[m];
					return_machines[machines_picked++] = m;
				}
				else if (holder[m] == min_makespan)
				{
					return_machines[machines_picked++] = m;
				}
			}

			return machines_picked;
		}

		void printDescription() const
		{
			std::cout << "Greedy Makespan / Priority(max)";
		}

	};

	class F : public Factory
	{
	public:
		Assigner& make(const Properties& p) const
		{
			return *(new A(p));
		}
	};
}

#endif