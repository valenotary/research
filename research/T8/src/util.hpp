#include <cstdint>
#include <random>
#include <iostream>
#include <climits>

#ifndef _T6_UTIL
#define _T6_UTIL

/*
	fast unsigned integer power
	https://stackoverflow.com/questions/101439/the-most-efficient-way-to-implement-an-integer-based-power-function-powint-int
*/
size_t ipow(size_t base, size_t exp)
{
	size_t result = 1;
	for (;;)
	{
		if (exp & 1)
			result *= base;
		exp >>= 1;
		if (!exp)
			break;
		base *= base;
	}

	return result;
}

/*
	compounded stdlib random
	for generating larger than 32,768 integer values
	https://stackoverflow.com/questions/22883840/c-get-random-number-from-0-to-max-long-long-integer/22883968
*/
typedef unsigned long long int UINT64;
UINT64 getRandom(UINT64 const& min = 0, UINT64 const& max = 0)
{
	return (((UINT64)(unsigned int)rand() << 32) + (UINT64)(unsigned int)rand()) % (max - min) + min;
}

void test_ipow_1()
{
	for (size_t i = 0; i < 10; ++i)
	{
		size_t b = rand() % 50 + 1;
		size_t exp = rand() % 5 + 1;
		size_t result = ipow(b, exp);
		std::cout << i << " | base: " << b << ", exp: " << exp << ", base ^ exp = " << result << '\n';
	}
}

void test_getRandom_1()
{
	for (size_t i = 0; i < 10; ++i)
	{
		UINT64 r = getRandom(0, ULLONG_MAX);
		std::cout << i << " | random uint64: " << r << '\n';
	}
}

#endif