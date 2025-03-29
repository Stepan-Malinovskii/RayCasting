#pragma once
#ifndef RANDOM
#define RANDOM

#include <random>

class Random
{
public:
	static float bitRandom();
	static int intRandom(int, int);
};
#endif // !RANDOM