#include <iostream>

#include "../properties.hpp"
#include "assigner.hpp"

#ifndef _T6_CONTEXT_PRIORITY_EXPONENTIAL
#define _T6_CONTEXT_PRIORITY_EXPONENTIAL

namespace Exponential
{

	class A : public Assigner
	{
	private:
		const double a;
		double * pre_expo;
		double * holder;

	public:
		A(const Properties& p, const double a) : Assigner(p), a(a)
		{
			this->pre_expo = new double[p.max + 1];
			this->holder = new double[p.machines];
			for (size_t i = 0; i <= p.max; ++i)
			{
				pre_expo[i] = std::pow(a, i);
			}
		}

		~A()
		{
			delete[] pre_expo;
			delete[] holder;
		}

		size_t pick(size_t * configuration, size_t * g, size_t * return_machines) const
		{

			for (size_t m = 0; m < p.machines; ++m)
			{
				double sum = 0;
				for (size_t d = 0; d < p.dimensions; ++d)
				{
					sum += pre_expo[configuration[m * p.dimensions + d] + g[d]];
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
			std::cout << "Exponential a = " << a;
		}

	};

	class F : public Factory
	{
	private:
		const double a;
	public:
		F(const double a) : a(a)
		{}
		Assigner& make(const Properties& p) const
		{
			return *(new A(p, a));
		}
	};

}

#endif