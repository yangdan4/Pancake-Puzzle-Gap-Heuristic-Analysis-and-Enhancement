#include <stdio.h>
#include "PancakePuzzle.h"
#include "IDA.h"
#include "exhaustive_search.h"
#include <iostream>
#include <fstream>
#include "input_utilities.h"
#include <map>
#include <math.h>
#include <sstream>
#include <string>
#include <algorithm>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <ctime> // time_t
#include <cstdio>

using namespace std;

int main(int argc, char **argv) {
	ofstream solutions2;
    int count = 0;
    int pancakes = 28;
    int problems = 1000;
    srand(time(NULL));
    solutions2.open("28 pancakes random self inverse problems hash file2", std::ofstream::out | std::ofstream::trunc);
    PancakeState rand_pancakes;
    PancakeState inv_pancakes = PancakeState(pancakes);
    int rand_in = 0;
    while(count < problems)
    {
    	//comment this line for consecutive
    	rand_pancakes = PancakePuzzle::GetRandomState(pancakes);
    	for(int i = 0; i < pancakes; i+=(rand_in+1))
    	{
    		rand_in = rand() % min(2, pancakes - i);
    		if(rand_in == 0 or i >= pancakes - 1)
    		{
    			continue;
    		}
    		//uncomment this for consecutive
    		/*rand_pancakes = PancakePuzzle::GetRandomState(rand_in + 1);
    		for(unsigned k = 0; k < rand_pancakes.puzzle.size(); k++)
    		{
    			rand_pancakes.puzzle[k] += i;
    		}*/
    		for(int j = 0; j < rand_in; j++)
    		{
    			//uncomment this for consecutive
    			//inv_pancakes.puzzle[rand_pancakes.puzzle[j]] = rand_pancakes.puzzle[j + 1];
    			//comment this for consecutive
    			inv_pancakes.puzzle[rand_pancakes.puzzle[j + i]] = rand_pancakes.puzzle[j + i + 1];
    		}
    		//uncomment this for consecutive
    		//inv_pancakes.puzzle[rand_pancakes.puzzle[rand_in]] = rand_pancakes.puzzle[0];
    		//comment this for consecutive
    		inv_pancakes.puzzle[rand_pancakes.puzzle[rand_in + i]] = rand_pancakes.puzzle[i];
    		cout << "inv" << endl;
    		cout << inv_pancakes << endl;
    	}
    	cout << inv_pancakes << endl;
    	for(unsigned i = 0; i < inv_pancakes.puzzle.size(); i++)
    	{
    		solutions2 << inv_pancakes.puzzle[i];
    		if(i != inv_pancakes.puzzle.size() - 1)
    		{
    			 solutions2 << ' ';
    		}
    	}
    	if(count != problems - 1)
    	{
    		solutions2 << "\n";
    	}
    	count++;
    	rand_pancakes = PancakePuzzle::GetRandomState(pancakes);
    	inv_pancakes = PancakeState(pancakes);
    }
    solutions2.close();
    solutions2.clear();
}
