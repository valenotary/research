#define LD_DIMENSIONS 2
#define MACHINES 2
#define OPT 100 // scaled OPT, e.g. smallest load scalar is 20 for OPT = 100, INV_EPS = 5; 100 / 5
#define INV_EPS 5 // 1/eps

#include "load_st.h"
#include "group_conf.h"
//#include "tests.h"
#include <unordered_set>
#include <iterator>
#include <math.h>

Load_ST* generate_g_space() // all G s.t. g1, g2 <= OPT
{
	size_t atom_load = OPT / INV_EPS; // smallest scalar unit of work
	unsigned long g_space_crdn = std::pow(INV_EPS + 1, LD_DIMENSIONS); // cardinality of g-space
	Load_ST* g_space = new Load_ST[g_space_crdn]; // g-space
	size_t lastIndex = 1; // pos of last (g1,g2) created, 1 because we consider zero vector our first element

	// all this is just to create g-space for an arbitary LD_DIMENSIONS >= 1
	for (size_t i = LD_DIMENSIONS; i > 0; --i)
	{

		Load_ST* i_dimn_axis = new Load_ST[INV_EPS];
		for (size_t dimn_axis_v = 1; dimn_axis_v <= INV_EPS; ++dimn_axis_v)
		{
			i_dimn_axis[dimn_axis_v - 1].d[i - 1] = dimn_axis_v * atom_load;
		}

		size_t old_lastIndex = lastIndex;
		for (size_t dimn_axis_v = 1; dimn_axis_v <= INV_EPS; ++dimn_axis_v)
		{
			for (size_t permut_i = 0; permut_i < old_lastIndex; ++permut_i)
			{
				g_space[lastIndex++] = g_space[permut_i] + i_dimn_axis[dimn_axis_v - 1];
			}
		}

	}

	return g_space;
}

std::unordered_set<Machine_Group, Machine_Group_Hasher> generate_RN()
{
	unsigned long volume = MACHINES * LD_DIMENSIONS * INV_EPS;
	Load_ST* g_space = generate_g_space();
	unsigned long g_space_crdn = std::pow(INV_EPS + 1, LD_DIMENSIONS);

	// seperation of unordered sets to allow loop to skip reprocessing same configurations
	std::unordered_set<Machine_Group, Machine_Group_Hasher> RN; // reachable spaces
	std::unordered_set<Machine_Group, Machine_Group_Hasher> prevStage; // previously generated sub-RN
	std::unordered_set<Machine_Group, Machine_Group_Hasher> stage; // for newly generated sub-RN
	prevStage.insert(Machine_Group()); // initialize with zero configuration when starting

	for (size_t i = 0; i < volume; ++i)
	{
		for (auto it : prevStage) // previous generated (A, B) in RN
		{
			for (size_t g_i = 1; g_i < g_space_crdn; ++g_i) // all G load vectors, start at 1 to skip zero vector
			{
				Machine_Group* new_mg = it.assignLoad_greedy(g_space[g_i]); // check and assign load to configuration (machine group)
				if (new_mg) stage.insert(*new_mg); // only add if not NULL
			}
		}
		for (auto it : prevStage) RN.insert(it); // push previously generated sub-RN (prevStage) to RN (RN)
		prevStage = std::unordered_set<Machine_Group, Machine_Group_Hasher>(); // reset previous generated sub-RN (prevStage)
		for (auto it : stage) prevStage.insert(it); // push newly generated sub-RN (stage) to previously generated sub-RN (prevStage)
		stage = std::unordered_set<Machine_Group, Machine_Group_Hasher>(); // reset newly generated sub-RN (stage)
	}
	for (auto it : prevStage) RN.insert(it); // final push of last set of newly generated sub-RN (stage)

	delete[] g_space;

	return RN;
}

int main(int argc, char **argv)
{

	//run_test();
	std::unordered_set<Machine_Group, Machine_Group_Hasher> RN = generate_RN();
	for (auto it : RN) // print out configurations
	{
		std::cout << it << std::endl; 
	}
	std::cout << RN.size(); // print out size
}