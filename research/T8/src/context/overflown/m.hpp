
#include <vector>
#include <cstdint>

#include "../../../Eigen/Dense"

#include "../properties.hpp"
#include "../representation.hpp"
#include "../masks/combination.hpp"
#include "../masks/permutation.hpp"
#include "../relative/heavier.hpp"
#include "../relative/lighter.hpp"


#ifndef _T6_CONTEXT_OVERFLOWN
#define _T6_CONTEXT_OVERFLOWN

class Overflown
{
private:
	const Properties& p;
	const Representation& r;
	uint8_t * space;
	const Heavier_Mark* h;
	const Lighter_Mark* l;

	std::vector<size_t*> perm_mask;
	size_t * perm_holder;

	// combinations_mask = pregenerated machine combinations, kernel_mask = pregenerated !overflown bound combinations
	std::vector<size_t*> machine_mask;
	std::vector<size_t*> kernel_mask;
	size_t machines_len;
	size_t kernel_len;

	const uint8_t UNDERFLOWN_S = 0b00000001;
	const uint8_t OVERFLOWN_S = 0b10000000;
	const uint8_t UNDETERMINED_S = 0b00000000;

public:
	Overflown(const Properties& p, uint8_t * space, const Representation& r) : p(p), space(space), r(r)
	{
		this->h = new Heavier_Mark(p, space);
		this->l = new Lighter_Mark(p, space);

		// sub-kernel init

		if (p.machines >= p.dimensions + 2)
		{
			this->machines_len = p.dimensions + 1;
			std::vector<size_t> orgComb;
			for (size_t i = 0; i < p.machines; ++i) orgComb.push_back(i);
			combination_gen(this->machines_len, &orgComb, &this->machine_mask);
		}
		else
		{
			this->machines_len = p.machines;
			size_t* normalOrder = new size_t[p.machines];
			for (size_t i = 0; i < p.machines; ++i) normalOrder[i] = i;
			machine_mask.push_back(normalOrder);
		}

		this->kernel_len = 0;
		if (p.machines > 1)
		{
			this->kernel_len = p.machines >= p.dimensions + 2 ? p.dimensions : p.machines - 1;
			std::vector<size_t> orgKernel;
			for (size_t i = 0; i < p.dimensions; ++i) orgKernel.push_back(i);
			combination_gen(this->kernel_len, &orgKernel, &this->kernel_mask);
		}

		// perm
		this->perm_holder = new size_t[p.degrees];
		permutation_gen(p, perm_mask);
		//((std::vector<size_t*>&) perm_mask).erase(perm_mask.begin(), perm_mask.begin() + 1);
	}

	~Overflown()
	{
		delete h, l;
		machine_mask.clear();
		kernel_mask.clear();
		delete[] perm_holder;
		perm_mask.clear();
	}

private:
	void fill_h(size_t * configuration, size_t index, size_t layer) const;
	void opt_ruleOutTypical() const;

public:
	bool checkMachineUnderflow(size_t* configuration) const;
	bool checkDimensionOverflow(size_t* configuration) const;
	bool checkIntersectionUnderflow(size_t* configuration) const;
	uint8_t checkStatus(size_t* configuration) const;

	void fill() const;

};

// check if any machines is within bounding box
bool Overflown::checkMachineUnderflow(size_t* configuration) const
{
	for (size_t m = 0; m < p.machines; ++m)
	{
		bool underflown = true;
		for (size_t d = 0; d < p.dimensions; ++d)
		{
			if (configuration[m * p.dimensions + d] > p.discretizations)
			{
				underflown = false;
				break;
			}
		}
		if (underflown)
		{
			return true;
		}
	}
	return false;
}

// check if any dimensions is outside bounding box
bool Overflown::checkDimensionOverflow(size_t* configuration) const
{
	for (size_t d = 0; d < p.dimensions; ++d)
	{
		bool overflown = true;
		for (size_t m = 0; m < p.machines; ++m)
		{
			if (configuration[m * p.dimensions + d] <= p.discretizations)
			{
				overflown = false;
				break;
			}
		}
		if (overflown)
		{
			return true;
		}
	}
	return false;
}

// check if linear combination intersects any sub-kernels
bool Overflown::checkIntersectionUnderflow(size_t* configuration) const
{
	//size_t index = r.index_configuration(configuration);

	Eigen::MatrixXd A(machines_len, machines_len);
	A.setZero();
	A.row(machines_len - 1).setOnes();
	Eigen::VectorXd b(machines_len);
	b.setZero();
	b.row(machines_len - 1).setOnes();

	//std::cout << A << '\n';
	//std::cout << b << "\n\n";

	for (auto it_m : machine_mask)
	{
		Eigen::MatrixXd conf_M(machines_len, p.dimensions);
		for (size_t i_m = 0; i_m < machines_len; ++i_m)
		{
			for (size_t d = 0; d < p.dimensions; ++d)
			{
				conf_M(i_m, d) = (double)configuration[it_m[i_m] * p.dimensions + d] - p.discretizations;
			}
		}
		//std::cout << conf_M << '\n';

		for (auto it_k : kernel_mask)
		{
			//for (size_t i_m = 0; i_m < machine_comb_len; ++i_m) std::cout << it_m[i_m] << '\t';
			//std::cout << '\n';
			//for (size_t i_k = 0; i_k < kernel_comb_len; ++i_k) std::cout << it_k[i_k] << '\t';
			//std::cout << '\n';

			for (size_t i_k = 0; i_k < kernel_len; ++i_k)
			{
				A.row(i_k) = conf_M.col(it_k[i_k]);
			}

			Eigen::VectorXd lambda = A.lu().solve(b);
			//std::cout << A << '\n';
			//if(index == 113369288) std::cout << lambda << "\n";
			//std::cout << lambda.hasNaN() << "\n";
			//std::cout << (lambda.array() >= 0).all() << '\n';

			if (!lambda.hasNaN() && (lambda.array() + 1000000 >= 1000000).all())
			{
				Eigen::VectorXd intersection = lambda.transpose() * conf_M;
				intersection.array() += 1000000; // shift out floating point imprecision
				//std::cout << intersection << '\n';
				//std::cout << (intersection.array() <= 1000).all() << '\n';

				if ((intersection.array() <= 1000000).all())
				{
					return true;
				}
			}
		}
	}
	return false;
}

uint8_t Overflown::checkStatus(size_t* configuration) const
{
	//if (checkMachineUnderflow(configuration)) {
	//	//std::cout << "U" << '\t';
	//	return UNDERFLOWN_S;
	//}
	//if (checkDimensionOverflow(configuration)) {
	//	//std::cout << "O" << '\t';
	//	return OVERFLOWN_S;
	//}
	if (checkIntersectionUnderflow(configuration)) {
		//std::cout << "U" << '\t';
		return UNDERFLOWN_S; 
	}
	//std::cout << "UNDER" << '\t';
	return UNDETERMINED_S;
}

void Overflown::fill_h(size_t * configuration, size_t index, size_t layer) const
{
	if (layer == p.degrees - 1)
	{
		for (size_t i = 0; i <= p.max; ++i)
		{
			configuration[layer] = i;

			if ((space[index] & p.OVERFLOWN_BM) || (space[index] & p.UNDERFLOWN_BM) || (space[index] & p.UNDETERMINED_BM))
			{
				index++;
				continue;
			}
			uint8_t status = checkStatus(configuration);

			for (auto it : perm_mask)
			{
				for (size_t i = 0; i < p.degrees; ++i) perm_holder[i] = configuration[it[i]];
				size_t index_p = r.index_configuration(perm_holder);

				if (status == UNDERFLOWN_S)
				{
					l->mark(perm_holder, index_p, p.UNDERFLOWN_BM);
				}
				else if (status == OVERFLOWN_S)
				{
					h->mark(perm_holder, index_p, p.OVERFLOWN_BM);
				}
				else if (status == UNDETERMINED_S)
				{
					space[index_p] = space[index_p] | p.UNDETERMINED_BM;
				}
			}

			index++;
		}
	}
	else
	{
		for (size_t i = 0; i <= p.max; ++i)
		{
			configuration[layer] = i;
			fill_h(configuration, index, layer + 1);
			index = index + p.position_scales[layer];
		}
	}
}

void Overflown::opt_ruleOutTypical() const
{
	size_t * configuration = new size_t[p.degrees];

	// single machine under
	for (size_t m = 0; m < p.machines; ++m)
	{
		std::fill(configuration, configuration + p.degrees, p.max);
		for (size_t d = 0; d < p.dimensions; ++d)
		{
			configuration[m * p.dimensions + d] = p.discretizations;
		}
		size_t index = r.index_configuration(configuration);
		l->mark(configuration, index, p.UNDERFLOWN_BM);
	}

	// single dimension over
	if (p.max > p.discretizations)
	{
		for (size_t d = 0; d < p.dimensions; ++d)
		{
			std::fill(configuration, configuration + p.degrees, 0);
			for (size_t m = 0; m < p.machines; ++m)
			{
				configuration[m * p.dimensions + d] = p.discretizations + 1;
			}
			size_t index = r.index_configuration(configuration);
			h->mark(configuration, index, p.OVERFLOWN_BM);
		}
	}
	
}

void Overflown::fill() const
{
	size_t * configuration = new size_t[p.degrees];
	std::fill(configuration, configuration + p.degrees, 0);
	size_t index = 0;
	opt_ruleOutTypical();
	fill_h(configuration, index, 0);

	for (size_t i = 0; i < p.volume; ++i)
	{
		if ((space[i] & p.OVERFLOWN_BM) || (space[i] & p.UNDERFLOWN_BM)) continue;
		space[i] = space[i] | p.OVERFLOWN_BM;
	}

	delete[] configuration;
}

#endif