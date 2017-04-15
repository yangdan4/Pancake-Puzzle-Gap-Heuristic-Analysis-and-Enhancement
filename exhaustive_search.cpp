#include "exhaustive_search.h"
#include "PancakePuzzle.h"
#include "A_star.h"
#include <cassert>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <stdio.h>


#include "IDA.h"
#include <iostream>
#include "input_utilities.h"
#include <map>
#include <math.h>
#include <string>
#include <time.h>
#include <ctime> // time_t
#include <cstdio>

exhaustive_search::exhaustive_search(PancakePuzzle& p, unsigned short size) : puzzle(p)
{
	unsigned int array_size = factorial(size);
	hash_array.resize(array_size);
	std::cout << size << std::endl;
	pancakes = size;
	std::cout << pancakes << std::endl;
	for (unsigned int x = 0; x < array_size; x++)
	{
		hash_array[x] = 255;
	}
	
}

exhaustive_search::exhaustive_search(PancakePuzzle& p) : puzzle(p)
{
}

exhaustive_search::~exhaustive_search()
{
}

unsigned int exhaustive_search::factorial(unsigned short n) 
{
    if (n == 0)
    {
    	return 1;
    }
    
    return n * factorial(n - 1);
}

void clear(std::queue<unsigned int> &q)
{
   std::queue<unsigned int> empty;
   std::swap(q, empty);
}

void exhaustive_search::clear_hash(std::vector<unsigned short> &q)
{
   	for (unsigned int x = 0; x < q.size(); x++)
	{
		q[x] = 255;
	}
}

unsigned short exhaustive_search::GapHeur(PancakeState& state) const {
	assert(puzzle.size == state.puzzle.size());
	
	int gaps = 0;
	for(int i = 0; i < int(puzzle.size) - 1; i++) 
	{
        	if(int(abs(state.puzzle[i] - state.puzzle[i + 1])) != 1)
        	{
            		gaps++;
            	}
    	}
    	if(int(abs(state.puzzle[int(puzzle.size) - 1] - puzzle.size)) != 1)
    	{
    		gaps++;
    	}
    	return /*weight **/ gaps;
}

unsigned int exhaustive_search::hash_fn(const PancakeState &s)
{
	std::vector<int> puzzle = s.puzzle;
	unsigned int hashVal = 0;
	int numEntriesLeft = s.puzzle.size();
	for (unsigned int x = 0; x < s.puzzle.size(); x++)
	{
		hashVal += puzzle[x]*factorial(numEntriesLeft-1);
		numEntriesLeft--;
		for (unsigned y = x; y < puzzle.size(); y++)
		{
			if (puzzle[y] > puzzle[x])
				puzzle[y]--;
		}
	}
	return hashVal;
}

void exhaustive_search::reverse_hash(PancakeState &s, unsigned int hash)
{
	std::vector<int> puzzle = s.puzzle;
	unsigned int hashVal = hash;

	int numEntriesLeft = 1;
	for (int x = s.puzzle.size()-1; x >= 0; x--)
	{
		puzzle[x] = hashVal%numEntriesLeft;
		hashVal /= numEntriesLeft;
		numEntriesLeft++;
		for (int y = x+1; y < (int) s.puzzle.size(); y++)
		{
			if (puzzle[y] >= puzzle[x])
				puzzle[y]++;
		}
	}

	s.puzzle = puzzle;
}

void reverse_hash2(PancakeState &s, unsigned int hash)
{
	std::vector<int> puzzle = s.puzzle;
	unsigned int hashVal = hash;

	int numEntriesLeft = 1;
	for (unsigned x = 23; x >= 12; x--)
	{
		puzzle[x] = hashVal%numEntriesLeft;
		hashVal /= numEntriesLeft;
		numEntriesLeft++;
		for (unsigned y = x+1; y < 24; y++)
		{
			if (puzzle[y] >= puzzle[x])
				puzzle[y]++;
		}
	}
	
	for (int x = 11; x >= 0; x--)
	{
		//std::cout << x << std::endl;
		puzzle[x] = puzzle[x + 12] + 12;
	}

	s.puzzle = puzzle;
}

int exhaustive_search::gap_decreasing_moves(PancakeState& state) const {

	int top = state.puzzle[0];
	bool flip2 = false;
	bool flip1;
	int num = 0;
	if(top == 0)
	{
		flip1 = true;
	}
	else
	{
		flip1 = false;
	}
	if(top == int(state.puzzle.size()) - 1)
	{
		flip2 = true;
		num+=1;
	}
	for(unsigned i = 1; i < state.puzzle.size(); i++)
	{
		if(state.puzzle[i] == top - 1)
		{
			if(abs(state.puzzle[i - 1] - top + 1) != 1)
			{
				num+=1;
			}
			flip1 = true;
			if(flip1 and flip2)
			{
				break;
			}
		}
		else if(state.puzzle[i] == top + 1)
		{
			if(abs(state.puzzle[i - 1] - top - 1) != 1)
			{
				num+=1;
			}
			flip2 = true;
			if(flip1 and flip2)
			{
				break;
			}
		}
		
	}
	return num;
}

void increment_by(std::map<unsigned int, unsigned long int>& map, unsigned int& value, const unsigned long int& run_time)
{
	if(map.insert(std::pair<unsigned int, unsigned long int>(value,run_time)).second == false)
    {
    	
    	map[value] += run_time;
    }
}

void increment(std::map<unsigned int, unsigned long int>& map, unsigned int& value)
{
	if (map.insert(std::pair<unsigned int,unsigned long int>(value,1)).second == false)
    {
    	map[value]++;
    }
}

void increment2(std::map<unsigned int, unsigned long int>& map, int value)
{
	if (map.insert(std::pair<int,unsigned long int>(value,1)).second == false)
    {
    	map[value]++;
    }
}

int exhaustive_search::gap_decreasing_moves_no_inv(PancakeState& state) const {

	int top = state.puzzle[0];
	bool flip2 = false;
	bool flip1;
	if(top == 0)
	{
		flip1 = true;
	}
	else
	{
		flip1 = false;
	}
	if(top == int(state.puzzle.size()) - 1)
	{
		return 0;
	}
	for(unsigned i = 1; i < state.puzzle.size(); i++)
	{
		if(state.puzzle[i] == top - 1)
		{
			if(abs(state.puzzle[i - 1] - top + 1) != 1)
			{
				return 0;
			}
			flip1 = true;
			if(flip1 and flip2)
			{
				break;
			}
		}
		else if(state.puzzle[i] == top + 1)
		{
			if(abs(state.puzzle[i - 1] - top - 1) != 1)
			{
				return 0;
			}
			flip2 = true;
			if(flip1 and flip2)
			{
				break;
			}
		}
		
	}
	return 1;
}

void exhaustive_search::search()
{
	clear(open);
	open.push(0);
	clear_hash(hash_array);
	hash_array[0] = 0;
	counter = 1;
	cost = 1;
	gaps = 0;
	PancakeState state(pancakes);
	PancakePuzzle env(pancakes);
	IDA search(env, 1, false, false);
	//int g = 0;
	//int h = 0;
	unsigned int hash_value = 0;
	unsigned long int hash_value2 = 0;
	int open_size = 1;
	int pop_counter = 0;
	while(1)
	{
		std::cout << counter << std::endl;
		if(counter == hash_array.size())
		{
			std::cout << cost << std::endl;
			return;
		}
		reverse_hash(state, open.front());
		for(unsigned j = 0; j < state.puzzle.size(); j++)
    		{
    			state.inverse[state.puzzle[j]] = j;
    		}
		open.pop();
		pop_counter++;
		for(unsigned int i = 2; i <= pancakes; i++)
		{
			puzzle.ApplyAction(state, i);
			hash_value = hash_fn(state);
			if(hash_array[hash_value] == 255)
			{
				/*std::cout << state << std::endl;
				std::cout << cost << std::endl;
				std::cout << gap_decreasing_moves(state) << std::endl;*/
				gaps = search.GapHeur(state);
				hash_array[hash_value] = cost - gaps;
				hash_value2 = hash_value;
				if(cost - gaps == 0)
				{
					increment2(gaps_map, cost);
					//hash_array0.push_back(hash_value2);
					
				}
				else if(cost - gaps == 1)
				{
					increment2(top_map, cost);
					//hash_array1.push_back(hash_value2);
				}
				else if(cost - gaps == 2)
				{
					increment2(top2_map, cost);
					//hash_array2.push_back(hash_value2);
				}
				
				
				
				increment2(ltop_map, cost);
				increment2(ltopfg2_map, cost - gaps - search.TopHeurWithInverseAndLFG(state, gaps));
				increment2(ltopfg_map, cost - gaps - search.gap_decreasing_moves_no_inv_l_FG2(state, gaps));
				//increment2(gaps_map, cost - gaps);
				/*hash_array[hash_value] = cost - gaps;
				increment2(top_map, cost - gaps - search.TopHeur(state, gaps));
				increment2(top2_map, cost - gaps - search.TopHeurWithInverse(state, gaps));
				increment2(ltop_map, cost - gaps - search.gap_decreasing_moves_no_inv_l(state, gaps));
				increment2(ltop2_map, cost - gaps - search.TopHeurWithInverseAndL(state, gaps));
				increment2(ltopfg_map, cost - gaps - search.gap_decreasing_moves_no_inv_l_FG(state, gaps));
				increment2(ltopfg2_map, cost - gaps - search.gap_decreasing_moves_no_inv_l_FG2(state, gaps));*/
				//increment_by(gaps_map, cost, gap_decreasing_moves_no_inv(state));
				//increment(num_map, cost);
				open.push(hash_value);
				counter++;
			}
			puzzle.ApplyAction(state, i);
		}
		if(pop_counter == open_size)
		{
			cost++;
			open_size = open.size();
			pop_counter = 0;
		}
	}
}

void exhaustive_search::search1000()
{
}

void exhaustive_search::write_to_file()
{
	/*std::ofstream hash_file;
	hash_file.open("hash_file12heurerror0allprobs", std::ofstream::out | std::ofstream::trunc);
	std::ofstream hash_file6;
	hash_file6.open("hash_file12heurerror1allprobs", std::ofstream::out | std::ofstream::trunc);
	std::ofstream hash_file7;
	hash_file7.open("hash_file12heurerror2allprobs", std::ofstream::out | std::ofstream::trunc);*/
	std::ofstream hash_file2;
	hash_file2.open("hash_file12pancakesheurerror0.gnumeric", std::ofstream::out | std::ofstream::trunc);
	std::ofstream hash_file3;
	hash_file3.open("hash_file12pancakesheurerror1.gnumeric", std::ofstream::out | std::ofstream::trunc);
	std::ofstream hash_file4;
	hash_file4.open("hash_file12pancakesheurerror2.gnumeric", std::ofstream::out | std::ofstream::trunc);
	std::ofstream hash_file5;
	hash_file5.open("hash_file12pancakescostdist.gnumeric", std::ofstream::out | std::ofstream::trunc);
	/*std::ofstream hash_file6;
	hash_file6.open("hash_file12pancakesltop2heur.gnumeric", std::ofstream::out | std::ofstream::trunc);*/
	std::ofstream hash_file9;
	hash_file9.open("hash_file12pancakesltopfgheur.gnumeric", std::ofstream::out | std::ofstream::trunc);
	std::ofstream hash_file8;
	hash_file8.open("hash_file12pancakesltopfg2heur.gnumeric", std::ofstream::out | std::ofstream::trunc);
	/*std::ofstream hash_file4;
	hash_file4.open("hash_file8pancakesnewbithard", std::ofstream::out | std::ofstream::trunc);*/
	/*for(unsigned long int i = 0; i < hash_array0.size(); i++)
	{
		hash_file << hash_array0[i] << "\n";
	}
	hash_file.close();
	hash_file.clear();
	
	for(unsigned long int i = 0; i < hash_array1.size(); i++)
	{
		hash_file6 << hash_array1[i] << "\n";
	}
	hash_file6.close();
	hash_file6.clear();
	
	for(unsigned long int i = 0; i < hash_array2.size(); i++)
	{
		hash_file7 << hash_array2[i] << "\n";
	}
	hash_file7.close();
	hash_file7.clear();*/
	
	for(std::map<unsigned int, unsigned long int>::iterator iter = gaps_map.begin(); iter != gaps_map.end(); ++iter)
    	{
    		hash_file2 << iter->first << ";" << iter->second << "\n";
   	}
	hash_file2.close();
	hash_file2.clear();
	
	for(std::map<unsigned int, unsigned long int>::iterator iter = top_map.begin(); iter != top_map.end(); ++iter)
    	{
    		hash_file3 << iter->first << ";" << iter->second << "\n";
   	}
	hash_file3.close();
	hash_file3.clear();
	
	for(std::map<unsigned int, unsigned long int>::iterator iter = top2_map.begin(); iter != top2_map.end(); ++iter)
    	{
    		hash_file4 << iter->first << ";" << iter->second << "\n";
   	}
	hash_file4.close();
	hash_file4.clear();
	
	for(std::map<unsigned int, unsigned long int>::iterator iter = ltop_map.begin(); iter != ltop_map.end(); ++iter)
    	{
    		hash_file5 << iter->first << ";" << iter->second << "\n";
   	}
	hash_file5.close();
	hash_file5.clear();
	
	/*for(std::map<unsigned int, unsigned long int>::iterator iter = ltop2_map.begin(); iter != ltop2_map.end(); ++iter)
    	{
    		hash_file6 << iter->first << ";" << iter->second << "\n";
   	}
	hash_file6.close();
	hash_file6.clear();*/
	
	for(std::map<unsigned int, unsigned long int>::iterator iter = ltopfg_map.begin(); iter != ltopfg_map.end(); ++iter)
    	{
    		hash_file9 << iter->first << ";" << iter->second << "\n";
   	}
	hash_file9.close();
	hash_file9.clear();
	
	for(std::map<unsigned int, unsigned long int>::iterator iter = ltopfg2_map.begin(); iter != ltopfg2_map.end(); ++iter)
    	{
    		hash_file8 << iter->first << ";" << iter->second << "\n";
   	}
	hash_file8.close();
	hash_file8.clear();
	
	/*for(std::map<unsigned int, unsigned long int>::iterator iter = num_map.begin(); iter != num_map.end(); ++iter)
    	{
    		hash_file3 << iter->first << ";" << iter->second << "\n";
   	}
	hash_file3.close();
	hash_file3.clear();*/
}

void increment(std::map<unsigned short, unsigned int>& map, unsigned short& value)
{
	if (map.insert(std::pair<unsigned short, unsigned int>(value,1)).second == false)
    {
    	map[value]++;
    }
}

void update_max(std::map<unsigned short, unsigned short>& map, unsigned short& value, unsigned short& run_time)
{
	if(map.insert(std::pair<unsigned short, unsigned short>(value,run_time)).second == false)
    {
    	if(map[value] < run_time)
    	{
    		map[value] = run_time;
    	}
    }
}

void update_min(std::map<unsigned short, unsigned short>& map, unsigned short& value, unsigned short& run_time)
{
	if(map.insert(std::pair<unsigned short, unsigned short>(value,run_time)).second == false)
    {
    	if(map[value] > run_time)
    	{
    		map[value] = run_time;
    	}
    }
}

void update_average(std::map<unsigned short, double>& map, std::map<unsigned short, unsigned int>& map2, unsigned short& value, double& run_time)
{
	if(map.insert(std::pair<unsigned short, double>(value,run_time)).second == false)
    {
    	map[value] = (map[value] * map2[value] + run_time) / (map2[value] + 1);
    }
}

void exhaustive_search::read_from_file()
{
	std::fstream myfile("hash_file2.csv", std::ios_base::in);
	unsigned int count = 0;
	unsigned short path_cost = 0;
	PancakeState s(pancakes);
	unsigned short difference = 0;
	while(myfile >> path_cost)
	{
		reverse_hash(s, count);
		difference = path_cost - GapHeur(s);
		/*std::cout << "state" << std::endl;
		std::cout << s << std::endl;
		std::cout << "difference" << std::endl;
		std::cout << difference << std::endl;*/
		increment(size_map, difference);
		count++;
	}
	myfile.close();
	myfile.clear();
}

void exhaustive_search::read_from_file2()
{
	std::fstream myfile("hash_file.csv", std::ios_base::in);
	unsigned int count = 0;
	unsigned short path_cost = 0;
	double path_cost2 = 0;
	PancakeState s(pancakes);
	unsigned short difference = 0;
	while(myfile >> path_cost)
	{
		reverse_hash(s, count);
		difference = path_cost - GapHeur(s);
		path_cost2 = path_cost;
		/*std::cout << "state" << std::endl;
		std::cout << s << std::endl;
		std::cout << "difference" << std::endl;
		std::cout << difference << std::endl;*/
		update_max(max_map, difference, path_cost);
		update_min(min_map, difference, path_cost);
		update_average(average_map, size_map, difference, path_cost2);
		increment(size_map, difference);
		count++;
		std::cout << count << std::endl;
	}
	myfile.close();
	myfile.clear();
}

/*std::vector<unsigned> GetRandomState(unsigned size, unsigned max) {

	std::vector<unsigned> permutation;

    for(unsigned i = 0; i < size; i++)
    {
    	//std::cout << max << std::endl;
        permutation.push_back((unsigned int)(rand() % max));
    }
    
    int index = 0;
    int temp;
    
    unsigned last = size;
    
    // randomizes elements in the permutation
    while(last > 1) {
        index = rand() % last;
        temp = permutation[last - 1];
        permutation[last - 1] = permutation[index];
        permutation[index] = temp;
        
        last--;
    }
    
    return permutation;
}*/

void exhaustive_search::read_from_file3()
{
	std::fstream myfile("hash_file.csv", std::ios_base::in);
	/*test_file[4].open("test_file4.csv", std::ofstream::out | std::ofstream::trunc);
	test_file[5].open("test_file5.csv", std::ofstream::out | std::ofstream::trunc);*/
	for(int i = 0; i < 6; i++)
	{
		difference_maps.push_back(std::map<unsigned int, unsigned short>());
	}
	unsigned int count = 0;
	unsigned total_count = 0;
	unsigned short path_cost = 0;
	PancakeState s(pancakes);
	unsigned short difference = 0;
	std::vector<std::vector<unsigned> > randoms;
	std::vector<unsigned int> sizes;
	sizes.push_back(205330494);
	sizes.push_back(246800263);
	sizes.push_back(26213570);
	sizes.push_back(648977);
	sizes.push_back(8216);
	sizes.push_back(80);
	std::vector<unsigned int> counters(6, 0);
	for(unsigned i = 0; i < 6; i++)
	{
		size_map.insert(std::pair<unsigned short, unsigned int>(i,0));
	}
	for(unsigned i = 0; i < 6; i++)
	{
		randoms.push_back(std::vector<unsigned>());
		//std::cout << sizes[i] << std::endl;

    		for(unsigned j = 0; j < 50; j++)
    		{
    	//std::cout << max << std::endl;
        		randoms[i].push_back((unsigned int)(rand() % sizes[i]));
    		}
    
    		int index = 0;
    		int temp;
    
    		unsigned last = 50;
    
    // randomizes elements in the permutation
    		while(last > 1) {
        		index = rand() % last;
        		temp = randoms[i][last - 1];
        		randoms[i][last - 1] = randoms[i][index];
        		randoms[i][index] = temp;
        
        		last--;
    		}
		while(1)
		{
			std::sort(randoms[i].begin(), randoms[i].end());
			randoms[i].erase(std::unique( randoms[i].begin(), randoms[i].end() ), randoms[i].end());
			if(randoms[i].size() == 50)
			{
				break;
			}
			else
			{
				randoms[i].push_back((unsigned int)(rand() % sizes[i]));
			}
		}
	}
	/*for(unsigned j = 0; j < randoms[5].size(); j++)
	{
		std::cout << randoms[5][j] << std::endl;
	}*/
	while(myfile >> path_cost)
	{
		reverse_hash(s, count);
		difference = path_cost - GapHeur(s);
		/*std::cout << "state" << std::endl;
		std::cout << s << std::endl;*/
		/*std::cout << "difference" << std::endl;
		std::cout << difference << std::endl;*/
		if(counters[difference] < 50 && size_map[difference] == randoms[difference][counters[difference]])
		{
			difference_maps[difference].insert(std::pair<unsigned int, unsigned short>(count, path_cost));
			counters[difference]++;
			total_count++;
		}
		increment(size_map, difference);
		count++;
		if(total_count == 300)
		{
			break;
		}
		std::cout << count << std::endl;
	}
	myfile.close();
	myfile.clear();
}

void exhaustive_search::read_from_file4(std::vector<PancakeState>& states)
{
	std::fstream myfile("exhaustive search 8 pancakes sample06.csv", std::ios_base::in);
	unsigned int path_cost = 0;
	unsigned int count = 0;
	for(int i = 0; i < 50; i++)
	{
		std::cout << pancakes << std::endl;
		states.push_back(PancakeState(12));
	}
	while(myfile >> path_cost)
	{
		reverse_hash(states[count], path_cost);
		/*std::cout << "state" << std::endl;
		std::cout << s << std::endl;
		std::cout << "difference" << std::endl;
		std::cout << difference << std::endl;*/
		count++;
	}
	myfile.close();
	myfile.clear();
}

void exhaustive_search::read_from_file7(std::vector<PancakeState>& states)
{
	std::fstream myfile("1000 problems of heuristic difference 4.csv", std::ios_base::in);
	unsigned int path_cost = 0;
	unsigned int count = 0;
	for(int i = 0; i < 1000; i++)
	{
		states.push_back(PancakeState(pancakes));
	}
	while(myfile >> path_cost)
	{
		reverse_hash(states[count], path_cost);
		/*std::cout << "state" << std::endl;
		std::cout << s << std::endl;
		std::cout << "difference" << std::endl;
		std::cout << difference << std::endl;*/
		count++;
	}
	myfile.close();
	myfile.clear();
}

void exhaustive_search::read_from_file6(std::vector<PancakeState>& states)
{
	std::fstream myfile("exhaustive search 8 pancakes sample06.csv", std::ios_base::in);
	unsigned int path_cost = 0;
	unsigned int count = 0;
	for(int i = 0; i < 50; i++)
	{
		states.push_back(PancakeState(24));
	}
	while(myfile >> path_cost)
	{
		reverse_hash2(states[count], path_cost);
		//std::cout << "state" << std::endl;
		//std::cout << states[count] << std::endl;
		/*std::cout << "difference" << std::endl;
		std::cout << difference << std::endl;*/
		count++;
	}
	myfile.close();
	myfile.clear();
}

void exhaustive_search::read_from_file5()
{
	std::fstream myfile("hash_file.csv", std::ios_base::in);
	/*test_file[4].open("test_file4.csv", std::ofstream::out | std::ofstream::trunc);
	test_file[5].open("test_file5.csv", std::ofstream::out | std::ofstream::trunc);*/
	unsigned int count = 0;
	unsigned total_count = 0;
	unsigned short path_cost = 0;
	PancakeState s(pancakes);
	unsigned short difference = 0;
	std::vector<unsigned> randoms;
	unsigned int size = 8216;
	unsigned int counter = 0;
		//std::cout << sizes[i] << std::endl;

    		for(unsigned j = 0; j < 1000; j++)
    		{
    	//std::cout << max << std::endl;
        		randoms.push_back((unsigned int)(rand() % size));
    		}
    
    		int index = 0;
    		int temp;
    
    		unsigned last = 1000;
    
    // randomizes elements in the permutation
    		while(last > 1) { 
        		index = rand() % last;
        		temp = randoms[last - 1];
        		randoms[last - 1] = randoms[index];
        		randoms[index] = temp;
        
        		last--;
    		}
		while(1)
		{
			std::sort(randoms.begin(), randoms.end());
			randoms.erase(std::unique( randoms.begin(), randoms.end() ), randoms.end());
			if(randoms.size() == 1000)
			{
				break;
			}
			else
			{
				randoms.push_back((unsigned int)(rand() % size));
			}
		}
	for(unsigned j = 0; j < randoms.size(); j++)
	{
		std::cout << randoms[j] << std::endl;
	}
	while(myfile >> path_cost)
	{
		reverse_hash(s, count);
		difference = path_cost - GapHeur(s);
		/*std::cout << "state" << std::endl;
		std::cout << s << std::endl;*/
		/*std::cout << "difference" << std::endl;
		std::cout << difference << std::endl;*/
		if(difference == 4 && counter < 1000 && total_count == randoms[counter])
		{
			four_map.insert(std::pair<unsigned int, unsigned short>(count, path_cost));
			counter++;
		}
		if(difference == 4)
		{
			total_count++;
		}
		count++;
		if(counter == 1000)
		{
			break;
		}
		std::cout << count << std::endl;
	}
	myfile.close();
	myfile.clear();
}






