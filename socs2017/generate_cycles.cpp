#include <stdio.h>
#include "PancakePuzzle.h"
#include "IDA.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <cstdio>

using namespace std;

/**
 * File for generating self inverse states and states with short cycles.
 */
int main(int argc, char **argv) {

    int pancakes = 10; // pancake stack size
    int problems = 10; // number of problems to generate
    bool consecutive = false; // Are the cycles in order
    int max_cycle_size = 2; // Maximum cycle size. 2 means a self-inverse state

    string output_filename = "10_self_inverse_problems";

	ofstream prob_stream;
    int count = 0;

    srand(time(NULL));

    prob_stream.open(output_filename.c_str(), std::ofstream::out | std::ofstream::trunc);

    PancakeState rand_pancakes;
    PancakeState new_stack = PancakeState(pancakes);
    int rand_in = 0;

    while(count < problems)
    {
        new_stack = PancakeState(pancakes);
        if(!consecutive)
            rand_pancakes = PancakePuzzle::GetRandomState(pancakes);

    	for(int i = 0; i < pancakes - 1; i+=(rand_in+1))
    	{
    		rand_in = rand() % min(max_cycle_size, pancakes - i);

    		if(consecutive) {
    		    rand_pancakes = PancakePuzzle::GetRandomState(rand_in + 1);
    		    for(unsigned k = 0; k < rand_pancakes.puzzle.size(); k++)
    		    {
    		        rand_pancakes.puzzle[k] += i;
    		    }
    		}
    		for(int j = 0; j < rand_in; j++)
    		{
    		    if(consecutive)
    		        new_stack.puzzle[rand_pancakes.puzzle[j]] = rand_pancakes.puzzle[j + 1];
    		    else
    		        new_stack.puzzle[rand_pancakes.puzzle[j + i]] = rand_pancakes.puzzle[j + i + 1];
    		}

    		if(consecutive)
    		    new_stack.puzzle[rand_pancakes.puzzle[rand_in]] = rand_pancakes.puzzle[0];
    		else
    		    new_stack.puzzle[rand_pancakes.puzzle[rand_in + i]] = rand_pancakes.puzzle[i];

    	}

    	cout << new_stack << endl;
    	for(unsigned i = 0; i < new_stack.puzzle.size(); i++)
    	{
    		prob_stream << new_stack.puzzle[i];
    		if(i != new_stack.puzzle.size() - 1)
    		{
    			 prob_stream << ' ';
    		}
    	}
    	if(count != problems - 1)
    	{
    		prob_stream << "\n";
    	}
    	count++;
    }
    prob_stream.close();
    prob_stream.clear();
}
