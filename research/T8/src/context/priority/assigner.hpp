

#include "../properties.hpp"

#ifndef _T6_CONTEXT_PRIORITY_ASSIGNER
#define _T6_CONTEXT_PRIORITY_ASSIGNER

class Assigner
{
protected:
	const Properties& p;
public:
	Assigner(const Properties& p) : p(p)
	{}
	virtual size_t pick(size_t * configuration, size_t * g, size_t * return_machines) const = 0;
	virtual void printDescription() const = 0;
};

class Factory
{
public:
	virtual Assigner& make(const Properties& p) const = 0;
};

#endif