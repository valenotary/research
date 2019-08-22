
#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <string>

#include "context_fill.hpp"
#include "context_desc_fill.hpp"
#include "active.hpp"

// check representation conversion
void test_1()
{
	Context* C = new Context(3, 3, 2, new Greedy_Makespace());
	size_t index = C->index_configuration(new size_t[C->degrees]{ 4, 3, 0, 1 , 2, 3, 0, 2, 1 });
	C->space[index] = 0b00000001;
	std::cout << index << '\n';
	size_t * conf = new size_t[C->degrees];
	C->reverse_index(index, conf);
	C->print_conf(conf); std::cout << '\n';

	delete[] conf;
	delete C;
}

// check permutation mask
void test_2()
{
	Context* C = new Context(3, 3, 1, new Greedy_Makespace());
	for (auto it : C->permuations_mask)
	{
		for (size_t i = 0; i < C->degrees; ++i)
		{
			std::cout << it[i] << '\t';
		}
		std::cout << '\n';
	}
	std::cout << C->permuations_mask.size() << '\n';

	delete C;
}

// check set_configuration
void test_3()
{
	Context* C = new Context(2, 2, 3, new Greedy_Makespace());
	size_t * conf = new size_t[C->degrees]{ 1, 0, 0, 0 };
	size_t index = C->index_configuration(conf);
	C->set_configuration(conf, index);
	for (size_t i = 0; i < C->volume; ++i)
	{
		C->print_index(i);
		std::cout << '\n';
	}

	delete C;
}

// check mark_permutative_configurations, called through set_configuration
void test_4()
{
	Context* C = new Context(2, 2, 2, new Greedy_Makespace());
	size_t * conf = new size_t[C->degrees]{ 1, 0, 0, 0 };
	size_t index = C->index_configuration(conf);
	C->set_configuration(conf, index);
	for (size_t i = 0; i < C->volume; ++i)
	{
		C->print_index(i);
		std::cout << '\n';
	}

	delete[] conf;
	delete C;
}

// check paf
void test_5()
{
	Context* C = new Context(2, 2, 2, new Greedy_Makespace());
	size_t * conf = new size_t[C->degrees]{ 1, 0, 1, 0 };
	size_t * g = new size_t[C->dimensions]{ 1, 0 };
	size_t * selected_machines_holder = new size_t[C->machines];
	std::cout << C->num_discret_conf << '\n';
	for (size_t i = 0; i < C->paf(conf, g, selected_machines_holder); ++i)
	{
		std::cout << selected_machines_holder[i] << '\t';
	}

	delete[] conf, g, selected_machines_holder;
	delete C;
}

// check desc functions
void test_6()
{
	Context* C = new Context(3, 3, 3, new Greedy_Makespace());
	size_t * conf = new size_t[C->degrees]{ 0, 3, 2, 6, 3, 4, 3, 2, 2 };
	size_t * g = new size_t[C->dimensions]{ 1, 1, 1 };

	size_t * sum_d = new size_t[C->dimensions];
	size_t * sum_d_g = new size_t[C->dimensions];
	C->sum_dimensions(conf, sum_d);
	C->sum_dimensions_g(conf, g, sum_d_g);

	for (size_t i = 0; i < C->dimensions; ++i) std::cout << (int)sum_d[i] << '\t';
	std::cout << '\n';
	for (size_t i = 0; i < C->dimensions; ++i) std::cout << (int)sum_d_g[i] << '\t';
	std::cout << '\n';

	size_t * sum_m = new size_t[C->machines];
	C->sum_machines(conf, sum_m);

	for (size_t i = 0; i < C->machines; ++i) std::cout << (int)sum_m[i] << '\t';
	std::cout << '\n';

	size_t* inf_m = new size_t[C->machines];
	size_t* inf_m_g = new size_t[C->machines];
	C->norm_inf_machines(conf, inf_m);
	C->norm_inf_machines_g(conf, g, inf_m_g);

	for (size_t i = 0; i < C->machines; ++i) std::cout << (int)inf_m[i] << '\t';
	std::cout << '\n';
	for (size_t i = 0; i < C->machines; ++i) std::cout << (int)inf_m_g[i] << '\t';
	std::cout << '\n';

	std::cout << C->is_valid_conf(conf) << '\t' << C->is_valid_conf_g(conf, g);

	delete C;
}

// check index_combine_conf_g
void test_7()
{
	Context* C = new Context(3, 2, 5, new Greedy_Makespace());
	size_t * conf = new size_t[C->degrees]{ 1, 0, 0, 0, 0, 0 };
	size_t * g = new size_t[C->dimensions]{ 7, 9 };
	size_t index = C->index_configuration(conf);
	size_t index_g = C->index_combine_conf_g(index, g, 0);
	C->print_index(index);
	std::cout << '\n';
	C->print_index(index_g);
	std::cout << '\n';

	delete C;
}

// check paf
void test_8()
{
	Context* C = new Context(2, 2, 5, new Greedy_Makespace());
	size_t * conf = new size_t[C->degrees]{ 5, 0, 0, 1 };
	size_t * g = new size_t[C->dimensions]{ 5, 9 };
	size_t * selected_machines_holder = new size_t[C->machines];
	for (size_t i = 0; i < C->paf(conf, g, selected_machines_holder); ++i)
	{
		std::cout << selected_machines_holder[i] << '\t';
	}

	delete C;
}

// compare perm on/off
void test_9()
{

	use_perm = false;

	Context* C1 = new Context(2, 2, 10, new Greedy_Makespace());
	C1->full_fill_space();
	size_t count1 = 0;
	for (size_t i = 0; i < C1->volume; ++i) if (C1->space[i]) count1++;

	use_perm = true;

	Context* C2 = new Context(2, 2, 10, new Greedy_Makespace());
	C2->full_fill_space();
	size_t count2 = 0;
	for (size_t i = 0; i < C2->volume; ++i) if (C2->space[i]) count2++;

	std::cout << count1 << '\t' << count2 << '\n';

	size_t count3 = 0;
	for (size_t i = 0; i < C1->volume; ++i)
	{
		if (C1->space[i] && !C2->space[i])
		{
			std::cout << ++count3 << '\t';
			C1->print_index(i);
			std::cout << '\t';
			C2->print_index(i);
			std::cout << '\n';
		}
	}
	
	delete C1, C2;
}

// fill test
void test_10(bool printResults)
{
	use_perm = true;
	Context* C = new Context(2, 2, 5, new Greedy_Makespace());
	C->full_fill_space();
	size_t count = 0;
	for (size_t i = 0; i < C->volume; ++i)
	{
		if (C->space[i]) count++;
		if (printResults)
		{
			C->print_index(i);
			std::cout << '\n';
		}
	}
	std::cout << "|RN|: " << count << ", CALCULATED: " << pc;

	delete C;
}

bool characterizeContext2M2D(Context* C, double alphaC)
{

	if (C->machines != 2 || C->dimensions != 2) return false;

	// overloaded

	double scaledAlphaC = alphaC * C->discretizations;

	size_t * max_g = new size_t[C->degrees];
	std::fill(max_g, max_g + C->degrees, C->discretizations);

	size_t * conf_i = new size_t[C->degrees];
	size_t * norm_inf_machines_i = new size_t[C->machines];

	for (size_t i = 0; i < C->volume; ++i)
	{
		//if (!C->space[i]) continue;

		C->reverse_index(i, conf_i);
		C->norm_inf_machines_g(conf_i, max_g, norm_inf_machines_i);

		//C->print(conf_i, i); for (size_t j = 0; j < 2; ++j) { std::cout << '\n' << norm_inf_machines_i[0] << '\t' << norm_inf_machines_i[1] << '\n'; }
		
		if (norm_inf_machines_i[0] > scaledAlphaC && norm_inf_machines_i[1] > scaledAlphaC)
		{
			C->space[i] = C->space[i] | OVERLOADED;
			if (i == 0) std::cout << "ER";
		}

	}
	
	// overflown

	int * conf_si = new int[C->degrees];
	for (size_t i = 0; i < C->volume; ++i)
	{
		//if (!C->space[i]) continue;

		C->reverse_index(i, (size_t*) conf_si);
		if ((conf_si[0] > C->discretizations && conf_si[2] > C->discretizations)
			|| (conf_si[1] > C->discretizations && conf_si[3] > C->discretizations))
		{
			C->space[i] = C->space[i] | OVERFLOWN;
		}
		else if ((conf_si[0] <= C->discretizations && conf_si[1] <= C->discretizations) || (conf_si[2] <= C->discretizations && conf_si[3] <= C->discretizations))
		{
			continue;
		}
		else
		{
			for (size_t d = 0; d < C->degrees; ++d) conf_si[d] -= C->discretizations;

			double slope = conf_si[3] - conf_si[1]; slope /= (conf_si[2] - conf_si[0]);
			// + 1000 to filter out any floating point errors
			
			double yint = conf_si[1] - conf_si[0] * slope + 1000;
			//double xint = conf_si[0] - conf_si[1] / slope + 1000;
			//double yint = conf_si[1] - conf_si[0] * slope;
			//double xint = conf_si[0] - conf_si[1] / slope;

			//if(yint > 0 && xint > 0 && (yint + 1000 == 1000) && (xint + 1000 == 1000)) { C->print_index(i); std::cout << '\t' << yint << '\t' << xint << '\n'; }
			//if (yint < 0 && xint < 0 && (yint + 1000 == 1000) && (xint + 1000 == 1000)) { C->print_index(i); std::cout << '\t' << yint << '\t' << xint << '\n'; }
			//yint += 1000; xint += 1000;

			//if (yint == 1000 || xint == 1000) continue;

			if(yint > 1000) C->space[i] = C->space[i] | OVERFLOWN;
			//if(xint > 0) C->space[i] = C->space[i] | OVERFLOWN;

			//if ((yint > 1000 && xint <= 1000) || (yint <= 1000 && xint > 1000)) { C->print_index(i); std::cout << '\t' << yint << '\t' << xint << '\n'; }
			//if ((yint >= 1000 && xint < 1000) || (yint < 1000 && xint >= 1000)) { C->print_index(i); std::cout << '\t' << yint << '\t' << xint << '\n'; }

		}
		//else if (conf_i[0] > C->discretizations && conf_i[3] > C->discretizations)
		//{
		//	double m0_X = ((double)(C->discretizations - conf_i[2])) / (conf_i[0] - conf_i[2]);
		//	double m1_X = ((double)(C->discretizations - conf_i[3])) / (conf_i[1] - conf_i[3]);
		//	if (m0_X <= m1_X) C->space[i] = C->space[i] | OVERFLOWN;
		//}
		//else if (conf_i[1] > C->discretizations && conf_i[2] > C->discretizations)
		//{
		//	double m0_X = ((double)(C->discretizations - conf_i[3])) / (conf_i[1] - conf_i[3]);
		//	double m1_X = ((double)(C->discretizations - conf_i[2])) / (conf_i[0] - conf_i[2]);
		//	if (m0_X <= m1_X) C->space[i] = C->space[i] | OVERFLOWN;
		//}

	}
}

void readF(Context* C)
{
	std::string str;
	std::ifstream infile;
	infile.open("r2tabdem.txt");
	size_t * conf = new size_t[4];
	while (!infile.eof())
	{
		infile >> conf[0] >> conf[1] >> conf[2] >> conf[3];
		//std::cout << conf[0] << '\t' << conf[1] << '\t' << conf[2] << '\t' << conf[3] << '\n';
		C->space[C->index_configuration(conf)] = C->space[C->index_configuration(conf)] | 0b01000000;

		for (auto it : C->permuations_mask)
		{
			for (size_t i = 0; i < C->degrees; ++i) C->ops_permutation_p[i] = conf[it[i]];
			size_t index_p = C->index_configuration(C->ops_permutation_p);
			//if (index_p >= volume)
			//{
			//	//std::cout << ops_g_p[0] << '\t' << ops_g_p[1] << '\t' << ops_g_p[2] << '\n';
			//	//print_conf(configuration);
			//	//print_conf(ops_permutation_p);
			//	size_t * ig = ops_g + dimensions;
			//	for (size_t* ip = ops_conf + degrees; ip <= ops_conf_p; ip += degrees)
			//	{
			//		print_conf(ip);
			//		std::cout << "\t||\t" << ig[0] << '\t' << ig[1] << '\t' << ig[2] << '\n';
			//		//std::cout << '\n';
			//		ig += dimensions;
			//	}
			//	print_conf(ops_permutation_p);
			//}
			C->space[index_p] = C->space[C->index_configuration(conf)] | 0b01000000;
		}
	}
	infile.close();
}

void normPrint(Context* C, size_t index)
{
	size_t * conf = new size_t[C->degrees];
	double * normConf = new double[C->degrees];
	C->reverse_index(index, conf);
	for (size_t i = 0; i < C->degrees; ++i) normConf[i] = (double)conf[i] / C->discretizations;
	std::cout << '[';
	for (size_t m = 0; m < C->machines - 1; ++m)
	{
		std::cout << '(';
		for (size_t d = 0; d < C->dimensions - 1; ++d)
		{
			std::cout << normConf[m * C->dimensions + d] << ", ";
		}
		std::cout << normConf[m * C->dimensions + C->dimensions - 1] << "); ";
	}
	std::cout << '(';
	for (size_t d = 0; d < C->dimensions - 1; ++d)
	{
		std::cout << normConf[(C->machines - 1) * C->dimensions + d] << ", ";
	}
	std::cout << normConf[(C->machines - 1) * C->dimensions + C->dimensions - 1] << ")]";// | ";

	//std::cout << index << " : " << std::bitset<8>(C->space[index]);
}

void compareTest()
{
	//test_10(false);


	Context* C = new Context(2, 2, 5, new Greedy_Makespace());
	std::cout << "Machines: " << C->machines << ", Dimensions: " << C->dimensions << ", 1/eps: " << C->discretizations << '\n';



	/*int * conf = new int[4];
	C->reverse_index(218617, (size_t*) conf);
	for (size_t i = 0; i < 4; ++i) { conf[i] -= 4; };
	for (size_t i = 0; i < 4; ++i) { std::cout << conf[i] << '\t'; };
	std::cout << '\n';*/


	C->full_fill_space();
	readF(C);
	//characterizeContext2M2D(C, 8.0 / 3 - 0.00);//1.0 / C->discretizations + 0.04);// +0.034);
	size_t count = 0;
	size_t overflown_c = 0;
	bool isAlphaComp = true;
	for (size_t i = 0; i < C->volume; ++i)
	{
		if (C->space[i] & OVERFLOWN)
		{
			overflown_c++;
		}
		if (C->space[i] & REACHABLE) count++;
		if ((C->space[i] & REACHABLE) && (C->space[i] & OVERLOADED) && !(C->space[i] & OVERFLOWN))
		{
			isAlphaComp = false;
			//C->print_index(i);
			//std::cout << '\n';
		}
		//C->print_index(i);
		//std::cout << '\n';
	}
	size_t difc = 0;
	std::cout << "\n\nE: reachable, T: non-reachable\n\n";
	for (size_t i = 0; i < C->volume; ++i)
	{
		if ((C->space[i] & REACHABLE) && !(C->space[i] & 0b01000000))
		{
			std::cout << ++difc << ": ";
			//C->print_index(i);
			normPrint(C, i);
			std::cout << '\n';
		}
	}
	difc = 0;
	std::cout << "\n\nE: non-reachable, T: reachable\n\n";
	for (size_t i = 0; i < C->volume; ++i)
	{
		if (!(C->space[i] & REACHABLE) && (C->space[i] & 0b01000000))
		{
			std::cout << ++difc << ": ";
			//C->print_index(i);
			normPrint(C, i);
			std::cout << '\n';
		}
	}
	std::cout << "|RN|: " << count << ", CALCULATED: " << pc << ", AlphaComp: " << isAlphaComp << ", Overflown: " << overflown_c;

	delete C;
}

int main(int argc, char **argv)
{
	//compareTest();
	Context* C = new Context(2, 2, 5, new Greedy_Makespace());
	std::cout << "Machines: " << C->machines << ", Dimensions: " << C->dimensions << ", 1/eps: " << C->discretizations << '\n';
	C->full_fill_space();
	characterizeContext2M2D(C, 8.0 / 3 - 0.00);//1.0 / C->discretizations + 0.04);// +0.034);
	size_t count = 0;
	size_t overflown_c = 0;
	bool isAlphaComp = true;

	for (size_t i = 0; i < C->volume; ++i)
	{
		if (C->space[i] & OVERFLOWN)
		{
			overflown_c++;
		}
		if (C->space[i] & REACHABLE) count++;
		if ((C->space[i] & REACHABLE) && (C->space[i] & OVERLOADED) && !(C->space[i] & OVERFLOWN))
		{
			isAlphaComp = false;
			C->print_index(i);
			std::cout << '\n';
		}
		//C->print_index(i);
		//std::cout << '\n';
	}

	std::cout << "|RN|: " << count << ", CALCULATED: " << pc << ", AlphaComp: " << isAlphaComp << ", Overflown: " << overflown_c;
	delete C;
}