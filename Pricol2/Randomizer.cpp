#include "Randomizer.h"
#include <exception>

float Random::bitRandom()
{
	return intRandom(0, 10000) / 10000.0f;
}
int Random::intRandom(int a, int b)
{
	if (a > b)
		throw "logic_error";
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(a, b);
	return distr(gen);
}