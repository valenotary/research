#ifndef LD_DIMENSIONS
#define LD_DIMENSIONS 2
#endif // !LD_DIMENSIONS

#include <iostream>
#include <iterator>

#ifndef LOAD_ST
#define LOAD_ST

struct Load_ST {
	long* d;
	bool isMachineConfg;

	Load_ST()
	{
		d = new long[LD_DIMENSIONS];
		std::fill(d, d + LD_DIMENSIONS, 0);
		isMachineConfg = false;
	}

	Load_ST(const Load_ST& l_st)
	{
		new (this) Load_ST();
		isMachineConfg = l_st.isMachineConfg;
		for (size_t i = 0; i < LD_DIMENSIONS; ++i)
		{
			d[i] = l_st.d[i];
		}
	}

	~Load_ST() { delete[] d; }

	// norm inf
	long norm_inf()
	{
		long max_ld = d[0];
		for (size_t i = 1; i < LD_DIMENSIONS; ++i)
		{
			if (d[i] > max_ld) max_ld = d[i];
		}
		return max_ld;
	}

	// some hash function just for unordered set to work
	size_t hash() const
	{
		size_t h = 0;
		for (size_t i = 0; i < LD_DIMENSIONS; ++i)
		{
			// https://stackoverflow.com/questions/42701688/using-an-unordered-map-with-arrays-as-keys/42701876
			h ^= std::hash<long>{}(d[i])+0x9e3779b9 + (h << 6) + (h >> 2);
		}
		return h;
	}

};

// hasher struct
struct Load_ST_Hasher {
	std::size_t operator()(const Load_ST& l_st) const {
		return l_st.hash();
	}
};

bool operator==(const Load_ST& lhs, const Load_ST& rhs)
{
	for (size_t i = 0; i < LD_DIMENSIONS; ++i)
	{
		if (lhs.d[i] != rhs.d[i]) return false;
	}
	return true;
}

bool operator!=(const Load_ST& lhs, const Load_ST& rhs)
{
	return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const Load_ST& l_st)
{
	os << (l_st.isMachineConfg ? '(' : '<');
	for (size_t i = 0; i < LD_DIMENSIONS - 1; ++i)
	{
		os << l_st.d[i] << ',' << ' ';
	}
	os << l_st.d[LD_DIMENSIONS - 1];
	os << (l_st.isMachineConfg ? ')' : '>');
	return os;
}

void operator+=(const Load_ST& lhs, const Load_ST& rhs)
{
	for (size_t i = 0; i < LD_DIMENSIONS; ++i)
	{
		lhs.d[i] += rhs.d[i];
	}
}

Load_ST& operator+(const Load_ST& lhs, const Load_ST& rhs)
{
	Load_ST* sum = new Load_ST();
	sum->isMachineConfg = lhs.isMachineConfg || rhs.isMachineConfg;

	*sum += lhs;
	*sum += rhs;

	return *sum;
}

#endif // !LOAD_ST