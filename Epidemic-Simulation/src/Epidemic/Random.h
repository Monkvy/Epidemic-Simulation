#pragma once
#include <iostream>
#include <time.h>


struct Random
{
	static void Init()
	{
		srand((unsigned)time(NULL));
	}

	static float Rand()
	{
		return (float)rand() / RAND_MAX;
	}
};