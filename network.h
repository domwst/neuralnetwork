#ifndef NETWORK_H
#define NETWORK_H

#include "functions.h"
#include "NeuralNetworkExceptions.h"
#include <vector>

#define DEFAULT_LEARN_RATE 0.03

class NeuralNetwork
{
private:

	std::vector<ActivationFunc> layers_func;
	std::vector<int> layers;
	std::vector<std::vector<ld>> input, output, b, delt_b, err, grad;
	std::vector<std::vector<std::vector<ld>>> weights, delt_weights;
	ld learn_rate;
	int n, amount_of_goinds_back;


public:

	NeuralNetwork(std::vector<int>, std::vector<ActivationFunc>, ld = DEFAULT_LEARN_RATE);
	std::vector<ld> getOut(std::vector<ld>);
	void goBack(std::vector<ld>);
	void learn();
};

NeuralNetwork::NeuralNetwork(std::vector<int> _layers, std::vector<ActivationFunc> _layers_func, ld _learn_rate) : 
layers(_layers), layers_func(_layers_func), learn_rate(_learn_rate), n(layers.size())
{
	std::cout << n << std::endl;
	if (layers.size() != layers_func.size())
	{
		throw InitVectorSizeError("Layers vector size doesn't match layers functoins vector size.");
		return;
	}
	amount_of_goinds_back = 0;
	input.resize(n);
	output.resize(n);
	b.resize(n);
	delt_b.resize(n);
	err.resize(n);
	grad.resize(n);
	for (int i = 0; i < n; ++i)
	{
		input[i].resize(layers[i]);
		output[i].resize(layers[i]);
		b[i].resize(layers[i]);
		delt_b[i].resize(layers[i], 0);
		err[i].resize(layers[i]);
		grad[i].resize(layers[i]);
	}
	weights.resize(n - 1);
	delt_weights.resize(n - 1);
	for (int i = 0; i < n - 1; ++i)
	{
		weights[i].resize(layers[i]);
		delt_weights[i].resize(layers[i], std::vector<ld>(layers[i + 1], 0));
		for (int j = 0; j < layers[i]; ++j)
		{
			weights[i][j].resize(layers[i + 1]);
			for (int k = 0; k < layers[i + 1]; ++k)
				weights[i][j][k] = ((rand() % 2000000) / 1000000.0 - 1);
		}
	}
	for (auto &x : b)
		for (auto &y : x)
			y = (rand() % 500) / 2000.0 - 0.125;
}

std::vector<ld> NeuralNetwork::getOut(std::vector<ld> inp)
{
	if (inp.size() != layers[0])
	{
		throw InputVectorSizeError("Your input vector size and network input vector size doesn't match");
		return {};
	}
	input[0] = inp;
	for (int i = 0; i < layers[0]; ++i)
	{
		// input[0][i] += b[0][i];
		output[0][i] = layers_func[0].func(input[0][i]);
	}
	for (int i = 1; i < n; ++i)
		for (int j = 0; j < layers[i]; ++j)
		{
			input[i][j] = b[i][j];
			for (int k = 0; k < layers[i - 1]; ++k)
				input[i][j] += output[i - 1][k] * weights[i - 1][k][j];
			output[i][j] = layers_func[i].func(input[i][j]);
		}
	return output.back();
}

void NeuralNetwork::goBack(std::vector<ld> must_be)
{
	if (must_be.size() != layers.back())
	{
		throw OutputVectorSizeError("Your output vector size and network output vector size doesn't match");
		return;
	}
	for (int i = 0; i < layers.back(); ++i)
	{
		err.back()[i] = must_be[i] - output.back()[i];
		grad.back()[i] = err.back()[i] * layers_func.back().der_f(input.back()[i]);
	}
	for (int i = n - 2; i > -1; --i)
		for (int j = 0; j < layers[i]; ++j)
		{
			err[i][j] = 0;
			for (int k = 0; k < layers[i + 1]; ++k)
				err[i][j] += weights[i][j][k] * grad[i + 1][k];
			grad[i][j] = err[i][j] * layers_func[i].der_f(input[i][j]);
		}
	for (int i = 0; i < n - 1; ++i)
		for (int j = 0; j < layers[i]; ++j)
			for (int k = 0; k < layers[i + 1]; ++k)
				delt_weights[i][j][k] += learn_rate * grad[i + 1][k] * output[i][j];
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < layers[i]; ++j)
			delt_b[i][j] += learn_rate * grad[i][j];
	++amount_of_goinds_back;
}

void NeuralNetwork::learn()
{
	if (amount_of_goinds_back <= 0)
	{
		amount_of_goinds_back = 0;
		return;
	}
	for (int i = 0; i < n - 1; ++i)
		for (int j = 0; j < layers[i]; ++j)
			for (int k = 0; k < layers[i + 1]; ++k)
			{
				weights[i][j][k] += delt_weights[i][j][k] / amount_of_goinds_back;
				delt_weights[i][j][k] = 0;
			}
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < layers[i]; ++j)
		{
			b[i][j] += delt_b[i][j] / amount_of_goinds_back;
			delt_b[i][j] = 0;
		}
	amount_of_goinds_back = 0;
}

#endif // NETWORK_H