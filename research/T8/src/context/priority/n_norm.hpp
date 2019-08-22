#include <iostream>

#include "../properties.hpp"
#include "assigner.hpp"

#ifndef _T6_CONTEXT_PRIORITY_NNORM
#define _T6_CONTEXT_PRIORITY_NNORM

namespace N_Norm
{

	class A : public Assigner
	{
	private:
		const double q;
		double * pre_pow;
		double * holder;

	public:
		A(const Properties& p, const double q) : Assigner(p), q(q)
		{
			this->pre_pow = new double[p.max + 1];
			this->holder = new double[p.machines];
			for (size_t i = 0; i <= p.max; ++i)
			{
				pre_pow[i] = std::pow(i, q);
			}
		}

		~A()
		{
			delete[] pre_pow;
			delete[] holder;
		}

		size_t pick(size_t * configuration, size_t * g, size_t * return_machines) const
		{

			for (size_t m = 0; m < p.machines; ++m)
			{
				double sum = 0;
				for (size_t d = 0; d < p.dimensions; ++d)
				{
					sum += pre_pow[configuration[m * p.dimensions + d] + g[d]];
				}
				holder[m] = sum;
			}

			size_t machines_picked = 1;
			double min_disutil = holder[0];
			return_machines[0] = 0;

			for (size_t m = 1; m < p.machines; ++m)
			{
				if (holder[m] < min_disutil)
				{
					machines_picked = 0;
					min_disutil = holder[m];
					return_machines[machines_picked++] = m;
				}
				else if (holder[m] == min_disutil)
				{
					return_machines[machines_picked++] = m;
				}
			}

			return machines_picked;
		}

		void printDescription() const
		{
			std::cout << "Vector " << q << "-Norm";
		}

	};

	class F : public Factory
	{
	private:
		const double q;
	public:
		F(const double q) : q(q)
		{}
		Assigner& make(const Properties& p) const
		{
			return *(new A(p, q));
		}
	};

}

#endif