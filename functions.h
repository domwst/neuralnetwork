#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <cmath>

using ld = long double;

struct ActivationFunc
{
	ld (*func)(ld), (*der_f)(ld);

	ActivationFunc(ld (*)(ld), ld (*)(ld));
};

ActivationFunc::ActivationFunc(ld (*_func)(ld), ld (*_der_f)(ld)) : 
func(_func), der_f(_der_f) {}

namespace FunctionLib
{
	ld tanh(ld v)
	{
		return ((exp(v) - exp(-v)) / (exp(v) + exp(-v)));
	}

	ld der_tanh(ld v)
	{
		return pow((2 * exp(v)) / (exp(2 * v) + 1), 2);
	}

	ld lin_func(ld v)
	{
		return v;
	}

	ld der_lin(ld v)
	{
		return 1;
	}

	ld sigmoid(ld v)
	{
		return 1 / (1 + exp(-v));
	}

	ld der_sigmoid(ld v)
	{
		return exp(v) / pow(exp(v) + 1, 2);
	}
}

ActivationFunc tg(FunctionLib::tanh, FunctionLib::der_tanh),
lin(FunctionLib::lin_func, FunctionLib::der_lin), sigm(FunctionLib::sigmoid, FunctionLib::der_sigmoid);

#endif // FUNTIONS_H