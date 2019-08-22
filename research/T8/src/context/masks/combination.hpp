
#include <vector>
#include <iostream>


#ifndef _T6_CONTEXT_MASKS_COMBINATION
#define _T6_CONTEXT_MASKS_COMBINATION

void combination_gen_h(size_t offset, size_t k, size_t org_k, std::vector<size_t>* original, std::vector<size_t>* combination, std::vector<size_t*>* output) {
	if (k == 0) {
		size_t* comb = new size_t[org_k];
		for (size_t i = 0; i < combination->size(); ++i) comb[i] = combination->at(i);
		output->push_back(comb);
		return;
	}
	for (int i = offset; i <= original->size() - k; ++i) {
		combination->push_back(original->at(i));
		combination_gen_h(i + 1, k - 1, org_k, original, combination, output);
		combination->pop_back();
	}
}


// generate all combinations of machines, machines >= dimensions + 2
// https://stackoverflow.com/questions/12991758/creating-all-possible-k-combinations-of-n-items-in-c
void combination_gen(size_t k, std::vector<size_t>* original, std::vector<size_t*>* output)
{
	std::vector<size_t> combination;
	combination_gen_h(0, k, k, original, &combination, output);
}

void test_comb_1()
{
	std::vector<size_t> org;
	std::vector<size_t*> out;
	for (size_t m = 1; m <= 5; ++m)
	{
		org.push_back(m);
		for (size_t k = 1; k <= m; ++k)
		{
			combination_gen(k, &org, &out);
			std::cout << "(M = " << m << ", K = " << k << "):\n";
			for (size_t it = 0; it < out.size(); ++it)
			{
				std::cout << "\t" << it + 1 << "\t| ";
				for (size_t i = 0; i < k; ++i) std::cout << out[it][i] << '\t';
				std::cout << '\n';
			}
			std::cout << '\n';
			out.clear();
		}
	}
}

#endif