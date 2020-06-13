#ifndef ENV_H
#define ENV_H

#include <stdlib.h>
#include <stdio.h>

#include <chrono>
#include <iostream>

using namespace std;

class Env
{
public:
	Env();
	virtual ~Env();

	void step();
	void multiStep(int max_steps);
	void setFrequency(int n_frequency);
	void render();

protected:
	int count;
	int frequency;
};

#endif
