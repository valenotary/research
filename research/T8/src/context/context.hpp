
#include <cstdint>

#include "properties.hpp"
#include "representation.hpp"
#include "printer/m.hpp"
#include "descriptor/norm_inf.hpp"
#include "priority/assigner.hpp"
#include "reachable/m.hpp"
#include "overflown/m.hpp"

#ifndef _T6_CONTEXT_CONTEXT
#define _T6_CONTEXT_CONTEXT

class Context
{
public: //private
	const Properties& p;
	const Representation& r;
	const Printer& printer;

private:
	const Norm_Inf& ni;
	Reachable* reachable;
	Overflown* overflown;

	const Assigner& a;
	uint8_t * space;

	bool analyzed = false;
	size_t count_reachable = 0;
	size_t count_underflown = 0;
	size_t count_overflown = 0;
	double alphaComp = 0;

public:
	Context(const size_t machines, const size_t dimensions, const size_t discretization, const Factory& f) :
		p(*new Properties(machines, dimensions, discretization)),
		a(f.make(p)),
		r(*new Representation(p)),
		space(new uint8_t[p.volume]),
		printer(*new Printer(p, r, space)),
		ni(*new Norm_Inf(p))
	{
		std::fill(space, space + p.volume, 0);

		this->reachable = new Reachable(p, r, space, a);
		this->overflown = new Overflown(p, space, r);
	}

private:
	// find alpha competitive ratio
	void findAlphaComp()
	{
		size_t * conf = new size_t[p.degrees];
		size_t * norm_inf = new size_t[p.machines];
		size_t ac = p.discretizations;
		for (size_t i = 0; i < p.volume; ++i)
		{
			if ((space[i] & p.REACHABLE_BM) && !(space[i] & p.OVERFLOWN_BM))
			{
				((Representation&) r).reverse_index(i, conf);
				ni.machines(conf, norm_inf);
				size_t min_loaded = norm_inf[0];
				for (size_t m = 1; m < p.machines; ++m) if (norm_inf[m] < min_loaded) min_loaded = norm_inf[m];
				if (min_loaded + p.discretizations > ac)
				{
					ac = min_loaded + p.discretizations;
				}
			}
		}
		alphaComp = (double)ac / p.discretizations;
	}

public:
	bool analyze()
	{
		if (analyzed) return false;
		
		reachable->full_fill_space();
		overflown->fill();
		findAlphaComp();

		for (size_t i = 0; i < p.volume; ++i)
		{
			if (space[i] & p.REACHABLE_BM) count_reachable++;
			if (space[i] & p.UNDERFLOWN_BM) count_underflown++;
			if (space[i] & p.OVERFLOWN_BM) count_overflown++;
		}

		analyzed = true;
		return true;
	}

	const uint8_t * getSpace()
	{
		return (const uint8_t*)space;
	}

	size_t getReachableCount() { return analyzed ? count_reachable : -1; }
	size_t getUnderflownCount() { return analyzed ? count_underflown : -1; }
	size_t getOverflownCount() { return analyzed ? count_overflown : -1; }
	double getAlphaComp() { return analyzed ? alphaComp : -1; }
	void printCount()
	{
		std::cout << "Context Properties ID: " << p.prop_id << '\n'; 
		std::cout << "\tDisutility Function: "; a.printDescription(); std::cout << '\n';
		std::cout << "\tReachable: " << count_reachable << '\n';
		std::cout << "\tUnderflown: " << count_underflown << '\n';
		std::cout << "\tOverflown: " << count_overflown << '\n';
		std::cout << "\tAlpha Competitive Ratio: " << alphaComp;
	}

};

#endif
