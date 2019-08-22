#include <iostream>

#include "../util.hpp"

#ifndef _T6_CONTEXT_PROPERTIES
#define _T6_CONTEXT_PROPERTIES

class Properties
{
private:
	static size_t prop_count;

public:
	const size_t machines;
	const size_t dimensions;
	const size_t discretizations;

	const size_t prop_id;
	const size_t degrees; // machines * dimensions, size of a configuration
	const size_t max; // maximum possible load scalar
	const size_t volume; // # of possible configurations

	const uint8_t REACHABLE_BM = 0b10000000;

	const uint8_t PERM_BM = 0b00000001;
	const uint8_t INITIAL_BM = 0b00000100;
	const uint8_t PREV_INITIAL_BM = 0b00000010;

	const uint8_t UNDERFLOWN_BM = 0b00100000;
	const uint8_t OVERFLOWN_BM = 0b01000000;
	const uint8_t UNDETERMINED_BM = 0b00010000;

	size_t* position_scales; // index displacement of configurations

	Properties(const size_t machines, const size_t dimensions, const size_t discretizations) :
		machines(machines),
		dimensions(dimensions),
		discretizations(discretizations),
		prop_id(prop_count++),
		degrees(machines * dimensions),
		max(machines * discretizations),
		volume((size_t)std::pow(max + 1, degrees))
	{
		this->position_scales = new size_t[degrees];
		for (size_t i = 0; i < degrees; ++i) position_scales[i] = ipow(max + 1, degrees - i - 1);
	}

	~Properties()
	{
		delete[] position_scales;
	}

};

size_t Properties::prop_count = 0;

std::ostream& operator<<(std::ostream& os, const Properties& p)
{
	os << "Context Properties ID: " << p.prop_id << '\n';
	os << "\tMachines: " << p.machines << ", Dimensions: " << p.dimensions << ", Discretizations: " << p.discretizations << '\n';
	os << "\tDegrees: " << p.degrees << ", Max: " << p.max << ", Volume: " << p.volume;
	return os;
}

void test_prop_1()
{
	Properties p0(2, 2, 20);
	Properties p1(2, 2, 93);
	Properties p2(3, 4, 7);
	Properties p3(3, 3, 13);
	std::cout << p0 << '\n' << p1 << '\n' << p2 << '\n' << p3 << '\n';
}

#endif