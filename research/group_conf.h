#ifndef MACHINES
#define MACHINES 2
#endif // !MACHINES

#ifndef OPT
#define OPT 10
#endif // !OPT

#include "load_st.h"

#ifndef GROUP_CONF
#define GROUP_CONF

struct Machine_Group {
	Load_ST* m;

	Machine_Group()
	{
		m = new Load_ST[MACHINES];
		for (size_t i = 0; i < MACHINES; ++i)
		{
			m[i].isMachineConfg = true;
		}
	}

	Machine_Group(const Machine_Group& mg)
	{
		new (this) Machine_Group();
		for (size_t i = 0; i < MACHINES; ++i)
		{
			m[i] += mg.m[i];
		}
	}

	// makespan
	long makespan()
	{
		long max_mkspan = m[0].norm_inf();
		for (size_t i = 1; i < MACHINES; ++i)
		{
			long mi_ninf = m[i].norm_inf();
			if (mi_ninf > max_mkspan) max_mkspan = mi_ninf;
		}
		return max_mkspan;
	}

	// combine machines into one equivalent
	Load_ST* singleMachineEq()
	{
		Load_ST* single_m = new Load_ST();
		for (size_t i = 0; i < MACHINES; ++i)
		{
			*single_m += m[i];
		}
		return single_m;
	}

	// inf norm of single machine
	long dimn_makespan()
	{
		Load_ST* sme = singleMachineEq();
		long ni = sme->norm_inf();
		delete sme;
		return ni;
	}

	// is configuration w/ new job going to valid or not
	bool is_loadAssignable(Load_ST load)
	{
		Load_ST* sme = &(*singleMachineEq() + load);
		long ni = sme->norm_inf();
		delete sme;
		return ni <= MACHINES * OPT;
	}

	// chooses machine w/ greedy priority of makespan
	int bestMachineForLoad_greedy(Load_ST load)
	{
		if (!is_loadAssignable(load)) return -1;
		int bestMachine = 0;
		long min_mkspan = (m[0] + load).norm_inf();
		for (size_t i = 1; i < MACHINES; ++i)
		{
			long mkspan = (m[i] + load).norm_inf();
			if (mkspan < min_mkspan)
			{
				bestMachine = i;
				min_mkspan = mkspan;
			}
		}
		return bestMachine;
	}

	// greedy allocation: null -> invalid conf
	Machine_Group* assignLoad_greedy(Load_ST load)
	{
		int bestMachine = bestMachineForLoad_greedy(load);
		if (bestMachine == -1) return NULL;
		Machine_Group* mg_wLoad = new Machine_Group (*this);
		mg_wLoad->m[bestMachine] += load;
		return mg_wLoad;
	}

	~Machine_Group()
	{
		delete[] m;
	}

	// some hash function just for unordered set to work
	size_t hash() const
	{
		size_t h = 0;
		for (size_t i = 0; i < MACHINES; ++i)
		{
			h ^= std::hash<long>{}(m[i].hash()) + 0x9e3779b9 + (h << 6) + (h >> 2);
		}
		return h;
	}

};

// hasher struct
struct Machine_Group_Hasher {
	std::size_t operator()(const Machine_Group& mg) const {
		return mg.hash();
	}
};

bool operator==(const Machine_Group& lhs, const Machine_Group& rhs)
{
	for (size_t i = 0; i < MACHINES; ++i)
	{
		if (lhs.m[i] != rhs.m[i]) return false;
	}
	return true;
}

bool operator!=(const Machine_Group& lhs, const Machine_Group& rhs)
{
	return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const Machine_Group& m_g)
{
	os << '[';
	for (size_t i = 0; i < MACHINES - 1; ++i)
	{
		os << m_g.m[i] << ';' << ' ';
	}
	os << m_g.m[MACHINES - 1];
	os << ']';
	return os;
}

#endif // !GROUP_CONF