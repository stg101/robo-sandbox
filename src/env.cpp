#include "env.h"

Env::Env()
{
	count = 0;
	frequency = 0;
	printf("created\n");
}

Env::~Env()
{
}

void Env::setFrequency(int n_frequency)
{
	frequency = n_frequency;
}

void Env::step()
{
	if (count == frequency)
		count = 0;
	else
		count++;
}

void Env::multiStep(int max_steps)
{
	int current_steps = 0;

	while (current_steps < max_steps)
	{
		if (count == frequency)
			count = 0;
		else
			count++;

		current_steps++;
	}
}

void Env::render()
{
	printf("count: %d\n", count);
}