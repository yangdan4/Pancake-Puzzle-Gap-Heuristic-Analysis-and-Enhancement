#include "PancakePuzzle.h"
#include "IDA.h"
#include <cassert>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <ctime> // time_t

IDA::IDA(PancakePuzzle& p, double new_weight, bool new_greater_pruning, bool new_equal_pruning)
        : puzzle(p), flag(0), weight(new_weight), greater_pruning(new_greater_pruning), equal_pruning(new_equal_pruning){
}

IDA::~IDA() {
}

uint64_t IDA::GetCounter()
{
	return counter;
}

uint64_t IDA::GetTopCounter()
{
	return top_counter;
}

uint64_t IDA::GetActionCounter()
{
	return action_counter;
}

double IDA::GetWeight()
{
	return weight;
}

void IDA::SetCounter(uint64_t new_counter)
{
	counter = 0;
}

int IDA::GetIterations()
{
	return iterations;
}

int IDA::GetNumberOfSolutions()
{

	return number_of_solutions;
}

std::vector<int> IDA::GetPath()
{
	return path;
}

int place(int pan, int ind)
{
	return pan < ind ? ind - 1 - pan : pan;
}

int IDA::GapHeur(PancakeState& state) const {
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



int IDA::TopHeur2(PancakeState& state, int inv) const {
    	std::vector<unsigned short int> moves;
    	gap_decreasing_moves3(state, moves, inv);
    	/*if(inv == 1)
    	{
    	for(unsigned i = 0; i < state.inverse.size(); i++)
	{
		std::cout << state.inverse[i] << ' ';
	}
	std::cout << std::endl;
	for(unsigned i = 0; i < moves.size(); i++)
	{
		std::cout << "move" << std::endl;;
		std::cout << moves[i] << std::endl;;
	}
	}*/
    	//std::cout << "abc" << std::endl;
    	//std::cout << "state" << std::endl;
    	//std::cout << state << std::endl;
    	/*std::cout << "gaps 1" << std::endl;
    	std::cout << gaps << std::endl;*/
    	if(moves.size() == 0)
    	{
    		return 1;
    	}
    	//return gaps;
    	else
    	{
    		int gaps = GapHeur(state);
    		for(unsigned j = 0; j < moves.size(); j++)
    		{
    			//OUTDATED!!! WANT TO CHECK IF INVERSE IF FUTURE LOCKED TOO BY A NORMAL FLIP
    			if(moves[j] >= state.puzzle.size())
    			{
    				if(!future_locked(state, -1, gaps, inv))
    				{
    					//std::cout << "not locked" << std::endl;
    					break;
    				}
    			}
    			else
    			{
    			if(!future_locked(state, moves[j], gaps, inv))
    			{
    				//std::cout << "not locked" << std::endl;
    				break;
    			}
    			}
    			if(j == moves.size() - 1)
    			{
    				return 1;
    			}
    		}
    		return 0;
    	}
    	/*std::cout << "gaps 3" << std::endl;
    	std::cout << gaps << std::endl;*/
}

int IDA::TopHeurWithInverse(PancakeState& state, int gaps) const {
	assert(puzzle.size == state.puzzle.size());
	
	if(gaps == 0)
	{
		return 0;
	}
	
    	return /*weight **/ std::max(gap_decreasing_moves(state, 0), gap_decreasing_moves(state, 1));
}

int IDA::TopHeurWithInverseAndL(PancakeState& state, int gaps) const {
	assert(puzzle.size == state.puzzle.size());
	
	if(gaps == 0)
	{
		return 0;
	}
    	return std::max(gap_decreasing_moves_with_inv_l(state, 0, gaps), gap_decreasing_moves_with_inv_l(state, 1, gaps));
}

int IDA::TopHeurWithInverseAndLFG_bf(PancakeState& state, int gaps, int flip) const {
	assert(puzzle.size == state.puzzle.size());
	
	if(gaps == 0)
	{
		return 0;
	}
	int ret;
	ret = TopHeurWithInverseAndLBF(state, gaps, flip);
	if(ret != 0)
	{
		int top = state.puzzle[flip - 1];
		int top_num = -1;
		int next_num = top;
		int next_top_num = top;
		int count = 0;
		bool good = false;
		for(unsigned i = 1; i < state.puzzle.size(); i++)
		{
			if(state.puzzle[place(i, flip)] == next_num - 1)
			{
				next_num = state.puzzle[place(i, flip)];
			}
			else if(next_num == top_num + 1 and next_num != next_top_num)
			{
				count++;
				if(count == 2 and next_top_num == top + 2)
				{
					good = true;
				}
				top_num = next_top_num;
				next_num = state.puzzle[place(i, flip)];
				next_top_num = state.puzzle[place(i, flip)];
				
			}
			else if(count > 1 and next_num == next_top_num)
			{
				
				if(state.puzzle[place(i, flip)] == next_num + 1)
				{
					top_num = next_top_num;
					next_num = state.puzzle[place(i, flip)];
					next_top_num = state.puzzle[place(i, flip)];
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
			if(i == state.puzzle.size() - 1)
			{
				if(count == 2 and (next_top_num == next_num + 1 or good))
				{
					ret = 1;
				}
				else if(count > 1)
				{
					ret = 2;
				}
			}
		}
		
	}
	return ret;
}

int IDA::TopHeurWithInverseAndLFG(PancakeState& state, int gaps) const {
	assert(puzzle.size == state.puzzle.size());
	
	if(gaps == 0)
	{
		return 0;
	}
	int ret;
	ret = TopHeurWithInverseAndL(state, gaps);
	if(ret != 0)
	{
		int top = state.puzzle[0];
		int top_num = -1;
		int next_num = top;
		int next_top_num = top;
		int count = 0;
		bool good = false;
		for(unsigned i = 1; i < state.puzzle.size(); i++)
		{
			if(state.puzzle[i] == next_num - 1)
			{
				next_num = state.puzzle[i];
			}
			else if(next_num == top_num + 1 and next_num != next_top_num)
			{
				count++;
				if(count == 2 and next_top_num == top + 2)
				{
					good = true;
				}
				top_num = next_top_num;
				next_num = state.puzzle[i];
				next_top_num = state.puzzle[i];
				
			}
			else if(count > 1 and next_num == next_top_num)
			{
				
				if(state.puzzle[i] == next_num + 1)
				{
					top_num = next_top_num;
					next_num = state.puzzle[i];
					next_top_num = state.puzzle[i];
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
			if(i == state.puzzle.size() - 1)
			{
				if(count == 2 and (next_top_num == next_num + 1 or good))
				{
					ret = 1;
				}
				else if(count > 1)
				{
					ret = 2;
				}
			}
		}
		
	}
	return ret;
}

int IDA::TopHeurWithInverseAndLBF(PancakeState& state, int gaps, int flip) const {
	assert(puzzle.size == state.puzzle.size());
	
	if(gaps == 0)
	{
		return 0;
	}
    	return std::max(gap_decreasing_moves_with_inv_lbf(state, 0, gaps, flip), gap_decreasing_moves_with_inv_lbf(state, 1, gaps, flip));
}

int IDA::TopHeurWithLNoInverse(PancakeState& state, int gaps) const {
	assert(puzzle.size == state.puzzle.size());
	
	if(gaps == 0)
	{
		return 0;
	}
	//DO CASE WHEN PAR_HEUR == -1
	/*PancakeState inversee(state.puzzle.size());
	for(unsigned i = 0; i < state.puzzle.size(); i++)
	{
		inversee.puzzle[state.puzzle[i]] = i;
		inversee.inverse[i] = state.puzzle[i];
		state.inverse[state.puzzle[i]] = i;
	}*/
	//int locked = 0;
	//bool checked = false;
	/*int top = state.puzzle[0];
    	if(state.puzzle[1] - state.puzzle[0] == 1)
    	{
		if(state.puzzle[0] == 0)
		{
			locked = 1;
			checked = true;
		}
		if(state.puzzle[0] == 1)
		{
			locked = 0;
			checked = true;
		}
	}*/
    	/*std::cout << "state" << std::endl;
    	std::cout << state << std::endl;
    	std::cout << "inverse" << std::endl;
    	std::cout << inversee << std::endl;*/
    	/*std::cout << "state" << std::endl;
    	std::cout << state << std::endl;
    	std::cout << "top heur 2 state" << std::endl;
    	std::cout << TopHeur2(state) << std::endl;
    	std::cout << "top heur 2 inverse" << std::endl;
    	std::cout << TopHeur2(inversee) << std::endl;*/
    	return /*weight **/ /*std::max(TopHeur2(state, 0), TopHeur2(state, 0))*/TopHeur2(state, 0);
}

int IDA::TopHeur3(PancakeState& state, int inv) const {
    	return gap_decreasing_moves(state, inv);
    	/*if(inv == 1)
    	{
    	for(unsigned i = 0; i < state.inverse.size(); i++)
	{
		std::cout << state.inverse[i] << ' ';
	}
	std::cout << std::endl;
	for(unsigned i = 0; i < moves.size(); i++)
	{
		std::cout << "move" << std::endl;;
		std::cout << moves[i] << std::endl;;
	}
	}*/
    	//std::cout << "abc" << std::endl;
    	//std::cout << "state" << std::endl;
    	//std::cout << state << std::endl;
    	/*std::cout << "gaps 1" << std::endl;
    	std::cout << gaps << std::endl;*/
    	/*std::cout << "gaps 3" << std::endl;
    	std::cout << gaps << std::endl;*/
}

int IDA::TopHeurWithInverseNoL(PancakeState& state, int gaps, int flip) const {
	assert(puzzle.size == state.puzzle.size());
	if(gaps == 0)
	{
		return 0;
	}
	//DO CASE WHEN PAR_HEUR == -1
	/*PancakeState inversee(state.puzzle.size());
	for(unsigned i = 0; i < state.puzzle.size(); i++)
	{
		inversee.puzzle[state.puzzle[i]] = i;
		inversee.inverse[i] = state.puzzle[i];
		state.inverse[state.puzzle[i]] = i;
	}*/
	//int locked = 0;
	//bool checked = false;
	/*int top = state.puzzle[0];
    	if(state.puzzle[1] - state.puzzle[0] == 1)
    	{
		if(state.puzzle[0] == 0)
		{
			locked = 1;
			checked = true;
		}
		if(state.puzzle[0] == 1)
		{
			locked = 0;
			checked = true;
		}
	}*/
    	/*std::cout << "state" << std::endl;
    	std::cout << state << std::endl;
    	std::cout << "inverse" << std::endl;
    	std::cout << inversee << std::endl;*/
    	/*std::cout << "state" << std::endl;
    	std::cout << state << std::endl;
    	std::cout << "top heur 2 state" << std::endl;
    	std::cout << TopHeur2(state) << std::endl;
    	std::cout << "top heur 2 inverse" << std::endl;
    	std::cout << TopHeur2(inversee) << std::endl;*/
    	return /*weight **/ std::max(gap_decreasing_moves2(state, flip, 0), gap_decreasing_moves2(state, flip, 1));
}

int IDA::TopHeurWithInverseNoL2(PancakeState& state, int gaps) const {
	assert(puzzle.size == state.puzzle.size());
	if(gaps == 0)
	{
		return 0;
	}
	//DO CASE WHEN PAR_HEUR == -1
	/*PancakeState inversee(state.puzzle.size());
	for(unsigned i = 0; i < state.puzzle.size(); i++)
	{
		inversee.puzzle[state.puzzle[i]] = i;
		inversee.inverse[i] = state.puzzle[i];
		state.inverse[state.puzzle[i]] = i;
	}*/
	//int locked = 0;
	//bool checked = false;
	/*int top = state.puzzle[0];
    	if(state.puzzle[1] - state.puzzle[0] == 1)
    	{
		if(state.puzzle[0] == 0)
		{
			locked = 1;
			checked = true;
		}
		if(state.puzzle[0] == 1)
		{
			locked = 0;
			checked = true;
		}
	}*/
    	/*std::cout << "state" << std::endl;
    	std::cout << state << std::endl;
    	std::cout << "inverse" << std::endl;
    	std::cout << inversee << std::endl;*/
    	/*std::cout << "state" << std::endl;
    	std::cout << state << std::endl;
    	std::cout << "top heur 2 state" << std::endl;
    	std::cout << TopHeur2(state) << std::endl;
    	std::cout << "top heur 2 inverse" << std::endl;
    	std::cout << TopHeur2(inversee) << std::endl;*/
    	return /*weight **/ std::max(gap_decreasing_moves(state, 0), gap_decreasing_moves(state, 1));
}

int IDA::TopHeurWithNoInverse(PancakeState& state, int gaps) const {
	assert(puzzle.size == state.puzzle.size());
	if(gaps == 0)
	{
		return 0;
	}
	//DO CASE WHEN PAR_HEUR == -1
	/*PancakeState inversee(state.puzzle.size());
	for(unsigned i = 0; i < state.puzzle.size(); i++)
	{
		inversee.puzzle[state.puzzle[i]] = i;
		inversee.inverse[i] = state.puzzle[i];
		state.inverse[state.puzzle[i]] = i;
	}*/
	//int locked = 0;
	//bool checked = false;
	/*int top = state.puzzle[0];
    	if(state.puzzle[1] - state.puzzle[0] == 1)
    	{
		if(state.puzzle[0] == 0)
		{
			locked = 1;
			checked = true;
		}
		if(state.puzzle[0] == 1)
		{
			locked = 0;
			checked = true;
		}
	}*/
    	/*std::cout << "state" << std::endl;
    	std::cout << state << std::endl;
    	std::cout << "inverse" << std::endl;
    	std::cout << inversee << std::endl;*/
    	/*std::cout << "state" << std::endl;
    	std::cout << state << std::endl;
    	std::cout << "top heur 2 state" << std::endl;
    	std::cout << TopHeur2(state) << std::endl;
    	std::cout << "top heur 2 inverse" << std::endl;
    	std::cout << TopHeur2(inversee) << std::endl;*/
    	return /*weight **/ /*std::max(TopHeur3(state, 0), TopHeur3(state, 0))*/gap_decreasing_moves(state, 0);
}

void IDA::setThreshold(int new_threshold)
{
	threshold = new_threshold;
}
    
int IDA::getThreshold()
{
	return threshold;
}

bool IDA::future_locked(PancakeState& state, int adj, int gaps, int inv) const
{
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	/*PancakeState llll(state.puzzle.size());
	llll.puzzle = state.inverse;*/
	/*std::cout << "inverse" << std::endl;
	std::cout << llll << std::endl;
	std::cout << "adj" << std::endl;
	std::cout << adj << std::endl;*/
	if(inv == 0)
	{
	int prev;
	int prevprev;
	int flip;
	if(adj != -1)
	{
		flip = adj;
	}
	else
	{
		flip = int(state.puzzle.size());
	}
	//std::cout << "aa" << std::endl;
	prev = state.puzzle[flip - 1];
	if(prev == int(state.puzzle.size()) - 1)
	{
		return false;
	}
	//std::cout << "a" << std::endl;
	prevprev = state.puzzle[flip - 2];
	//std::cout << "b" << std::endl;
	int loc1 = state.inverse[prev - 2];
	//std::cout << "c" << std::endl;
	int loc2 = state.inverse[prev - 1];
	//std::cout << "d" << std::endl;
	int loc3 = state.inverse[prev + 2];
	//std::cout << "e" << std::endl;
	int loc4 = state.inverse[prev + 1];
	//std::cout << prev << std::endl;
	//std::cout << prevprev << std::endl;
	//std::cout << "f" << std::endl;
	if(prev - prevprev == -1)
	{
		if(prev == 1)
		{
			return false;
		}
		if(prev == 0)
		{
		if(gaps == 1 && ((abs(state.puzzle.size() - state.puzzle[0]) == 1) || (abs(state.puzzle[flip] - state.puzzle[0]) == 1)))
		{
			return false;
		}
		else
		{
			return true;
		}
		}
		if(loc1 - loc2 == -1 && loc1 > (flip - 1))
		{
			return true;
		}
		else if(loc1 - loc2 == 1 && loc1 < flip)
		{
			return true;
		}
	}
	else if(prev - prevprev == 1)
	{
		if(prev == int(state.puzzle.size())-2)
		{
			return false;
		}
		if(loc3 - loc4 == -1 && loc3 > (flip - 1))
		{
			return true;
		}
		else if(loc3 - loc4 == 1 && loc3 < flip)
		{
			return true;
		}
	}
	else
	{
		if(prev == 1 || prev == int(state.puzzle.size())-2)
		{
			return false;
		}
		if(((prev == 0) || ((loc1 - loc2 == -1 && loc1 > (flip - 1)) || (loc1 - loc2 == 1 && loc1 < flip)))
		&&((loc3 - loc4 == -1 && loc3 > (flip - 1)) || (loc3 - loc4 == 1 && loc3 < flip)))
		{
			return true;
		}
	}
	return false;
	}
	else
	{
	int prev;
	int prevprev;
	int flip;
	//std::cout << "adj" << std::endl;
	//std::cout << adj << std::endl;
	if(adj != -1)
	{
		flip = adj;
	}
	else
	{
		flip = int(state.inverse.size());
	}
	//std::cout << "aa" << std::endl;
	prev = state.inverse[flip - 1];
	if(prev == int(state.inverse.size()) - 1)
	{
		return false;
	}
	//std::cout << "a" << std::endl;
	prevprev = state.inverse[flip - 2];
	//std::cout << "b" << std::endl;
	int loc1 = state.puzzle[prev - 2];
	//std::cout << "c" << std::endl;
	int loc2 = state.puzzle[prev - 1];
	//std::cout << "d" << std::endl;
	int loc3 = state.puzzle[prev + 2];
	//std::cout << "e" << std::endl;
	int loc4 = state.puzzle[prev + 1];
	//std::cout << prev << std::endl;
	//std::cout << prevprev << std::endl;
	//std::cout << "f" << std::endl;
	if(prev - prevprev == -1)
	{
		if(prev == 1)
		{
			return false;
		}
		if(prev == 0)
		{
		if(gaps == 1 && ((abs(state.inverse.size() - state.inverse[0]) == 1) || (abs(state.inverse[flip] - state.inverse[0]) == 1)))
		{
			return false;
		}
		else
		{
			return true;
		}
		}
		if(loc1 - loc2 == -1 && loc1 > (flip - 1))
		{
			return true;
		}
		else if(loc1 - loc2 == 1 && loc1 < flip)
		{
			return true;
		}
	}
	else if(prev - prevprev == 1)
	{
		if(prev == int(state.inverse.size())-2)
		{
			return false;
		}
		if(loc3 - loc4 == -1 && loc3 > (flip - 1))
		{
			return true;
		}
		else if(loc3 - loc4 == 1 && loc3 < flip)
		{
			return true;
		}
	}
	else
	{
		if(prev == 1 || prev == int(state.inverse.size())-2)
		{
			return false;
		}
		if(((prev == 0) || ((loc1 - loc2 == -1 && loc1 > (flip - 1)) || (loc1 - loc2 == 1 && loc1 < flip)))
		&&((loc3 - loc4 == -1 && loc3 > (flip - 1)) || (loc3 - loc4 == 1 && loc3 < flip)))
		{
			return true;
		}
	}
	return false;
	}
}

bool IDA::locked_or_not(PancakeState& state) const {
	assert(puzzle.size == state.puzzle.size());
	
	int lock_type = -1;
	int top = state.puzzle[0];
	bool half_true = false;
	if(state.puzzle.size() >= 2)
	{
		if(top == int(state.puzzle.size()) - 1)
		{
			return false;
		}
		if(state.puzzle[1] - top == 1)
		{
			if(top == 0)
			{
				return true;
			}
			if(top == 1)
			{
				return false;
			}
			lock_type = 0;
		}
		else if(state.puzzle[1] - top == -1)
		{
			if(top == int(state.puzzle.size()) - 2)
			{
				return false;
			}
			lock_type = 1;
		}
		else
		{
			if(top == 1)
			{
				return false;
			}
			if(top == 0)
			{
				half_true = true;
			}
			if(top == int(state.puzzle.size()) - 2)
			{
				return false;
			}
			lock_type = 2;
		}
		for(int i = 1; i < int(puzzle.size) - 1; i++)
		{
            		if(lock_type == 0 && state.puzzle[i] == top - 2 && state.puzzle[i + 1] == top - 1)
            		{
            			return true;
            		}
            		else if(lock_type == 0 && state.puzzle[i + 1] == top - 1)
            		{
            			return false;
            		}
            		if(lock_type == 1 && state.puzzle[i] == top + 2 && state.puzzle[i + 1] == top + 1)
            		{
            			return true;
            		}
            		else if(lock_type == 1 && state.puzzle[i + 1] == top + 1)
            		{
            			return false;
            		}
            		if(lock_type == 2 && state.puzzle[i] == top - 2 && state.puzzle[i + 1] == top - 1)
            		{
            			if(half_true)
            			{
            				return true;
            			}
            			half_true = true;
            		}
            		else if(lock_type == 2 && state.puzzle[i + 1] == top - 1)
            		{
            			return false;
            		}
            		if(lock_type == 2 && state.puzzle[i] == top + 2 && state.puzzle[i + 1] == top + 1)
            		{
            			if(half_true)
            			{
            				return true;
            			}
            			half_true = true;
            		}
            		else if(lock_type == 2 && state.puzzle[i + 1] == top + 1)
            		{
            			return false;
            		}
    		}
	}
    	return false;
}


void IDA::gap_decreasing_moves4(PancakeState& state, std::vector<unsigned short int>& moves) const {

	int top = state.puzzle[0];
	bool flip1 = false;
	bool flip2 = false;
	if(top == int(state.puzzle.size()) - 1)
	{
		moves.push_back(state.puzzle.size());
		flip2 = true;
	}
	for(unsigned i = 1; i < state.puzzle.size(); i++)
	{
		if(state.puzzle[i] == top - 1)
		{
			if(abs(state.puzzle[i - 1] - top + 1) != 1)
			{
				moves.push_back(i);
			}
			flip1 = true;
			if(flip1 and flip2)
			{
				break;
			}
		}
		if(state.puzzle[i] == top + 1)
		{
			if(abs(state.puzzle[i - 1] - top - 1) != 1)
			{
				moves.push_back(i);
			}
			flip2 = true;
			if(flip1 and flip2)
			{
				break;
			}
		}
		
	}
}

int IDA::gap_decreasing_moves_no_inv(PancakeState& state) const {

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

int IDA::TopHeur(PancakeState& state, int gaps) const {
	assert(puzzle.size == state.puzzle.size());
	if(gaps == 0)
	{
		return 0;
	}
	//int locked = 0;
	//bool checked = false;
	/*int top = state.puzzle[0];
    	if(state.puzzle[1] - state.puzzle[0] == 1)
    	{
		if(state.puzzle[0] == 0)
		{
			locked = 1;
			checked = true;
		}
		if(state.puzzle[0] == 1)
		{
			locked = 0;
			checked = true;
		}
	}*/
	/*for(int i = 0; i < int(puzzle.size) - 1; i++) 
	{
        	if(int(abs(state.puzzle[i] - state.puzzle[i + 1])) != 1)
        	{
            		gaps++;
            	}
    	}
    	if(int(abs(state.puzzle[int(puzzle.size) - 1] - puzzle.size)) != 1)
    	{
    		gaps++;
    	}*/
    	return gap_decreasing_moves_no_inv(state);
}

int IDA::gap_decreasing_moves_no_inv_l(PancakeState& state, int gaps) const {
	if(gaps == 0)
	{
		return 0;
	}
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
		if(gaps - 1 == 0)
		{
			return 0;
		}
		if(gap_decreasing_moves_no_inv_flip(state, int(state.puzzle.size())) == 0)
		{
			return 0;
		}
		flip2 = true;
	}
	for(unsigned i = 1; i < state.puzzle.size(); i++)
	{
		if(state.puzzle[i] == top - 1)
		{
			if(abs(state.puzzle[i - 1] - top + 1) != 1)
			{
				if(gaps - 1 == 0)
				{
					return 0;
				}
				if(gap_decreasing_moves_no_inv_flip(state, i) == 0)
				{
					return 0;
				}
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
				if(gaps - 1 == 0)
				{
					return 0;
				}
				if(gap_decreasing_moves_no_inv_flip(state, i) == 0)
				{
					return 0;
				}
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

int IDA::gap_decreasing_moves_no_inv_l_FG(PancakeState& state, int gaps) const {
	if(gaps == 0)
	{
		return 0;
	}
	int top = state.puzzle[0];
	bool flip2 = false;
	bool flip1;
	int top_num = -1;
	int next_num = top;
	int next_top_num = top;
	bool right = true;
	bool unlocked = false;
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
		if(gaps - 1 == 0)
		{
			return 0;
		}
		if(gap_decreasing_moves_no_inv_flip(state, int(state.puzzle.size())) == 0)
		{
			return 0;
		}
		flip2 = true;
	}
	for(unsigned i = 1; i < state.puzzle.size(); i++)
	{
		if(right)
		{
		if(state.puzzle[i] == next_num - 1)
		{
			next_num = state.puzzle[i];
		}
		else if(next_num == top_num + 1 and next_num != next_top_num)
		{
			top_num = next_top_num;
			next_num = state.puzzle[i];
			next_top_num = state.puzzle[i];
		}
		else
		{
			right = false;
			if(unlocked)
			{
				return 0;
			}
			else if(flip1 and flip2)
			{
				return 1;
			}
		}
		}
		if(!flip1 or !flip2)
		{
		if(state.puzzle[i] == top - 1)
		{
			if(abs(state.puzzle[i - 1] - top + 1) != 1)
			{
				if(gaps - 1 == 0)
				{
					unlocked = true;
					if(!right)
					{
						return 0;
					}
				}
				if(gap_decreasing_moves_no_inv_flip(state, i) == 0)
				{
					unlocked = true;
					if(!right)
					{
						return 0;
					}
				}
			}
			flip1 = true;
			if(!right and flip1 and flip2)
			{
				return 1;
			}
		}
		else if(state.puzzle[i] == top + 1)
		{
			if(abs(state.puzzle[i - 1] - top - 1) != 1)
			{
				if(gaps - 1 == 0)
				{
					unlocked = true;
					if(!right)
					{
						return 0;
					}
				}
				if(gap_decreasing_moves_no_inv_flip(state, i) == 0)
				{
					unlocked = true;
					if(!right)
					{
						return 0;
					}
				}
			}
			flip2 = true;
			if(!right and flip1 and flip2)
			{
				return 1;
			}
		}
		}
		
	}
	return 2;
}

/*int IDA::TopHeurWithInverseAndLFG(PancakeState& state, int gaps) const {
	assert(puzzle.size == state.puzzle.size());
	
	if(gaps == 0)
	{
		return 0;
	}
	int ret;
	ret = TopHeurWithInverseAndL(state, gaps);
	if(ret != 0)
	{
		int top = state.puzzle[0];
		int top_num = -1;
		int next_num = top;
		int next_top_num = top;
		int count = 0;
		bool good = false;
		for(unsigned i = 1; i < state.puzzle.size(); i++)
		{
			if(state.puzzle[i] == next_num - 1)
			{
				next_num = state.puzzle[i];
			}
			else if(next_num == top_num + 1 and next_num != next_top_num)
			{
				count++;
				if(count == 2 and next_top_num == top + 2)
				{
					good = true;
				}
				top_num = next_top_num;
				next_num = state.puzzle[i];
				next_top_num = state.puzzle[i];
				
			}
			else if(count > 1 and next_num == next_top_num)
			{
				
				if(state.puzzle[i] == next_num + 1)
				{
					top_num = next_top_num;
					next_num = state.puzzle[i];
					next_top_num = state.puzzle[i];
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
			if(i == state.puzzle.size() - 1)
			{
				if(count == 2 and (next_top_num == next_num + 1 or good))
				{
					ret = 1;
				}
				else if(count > 1)
				{
					ret = 2;
				}
			}
		}
		
	}
	return ret;
}*/

int IDA::gap_decreasing_moves_no_inv_l_FG2(PancakeState& state, int gaps) const {
	if(gaps == 0)
	{
		return 0;
	}
	int top = state.puzzle[0];
	bool flip2 = false;
	bool flip1;
	int top_num = -1;
	int next_num = top;
	int next_top_num = top;
	bool right = true;
	bool unlocked = false;
	int count = 0;
	bool good = false;
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
		if(gaps - 1 == 0)
		{
			return 0;
		}
		if(gap_decreasing_moves_no_inv_flip(state, int(state.puzzle.size())) == 0)
		{
			return 0;
		}
		flip2 = true;
	}
	for(unsigned i = 1; i < state.puzzle.size(); i++)
	{
		if(right)
		{
		if(state.puzzle[i] == next_num - 1)
		{
			next_num = state.puzzle[i];
		}
		else if(next_num == top_num + 1 and next_num != next_top_num)
			{
				count++;
				if(count == 2 and next_top_num == top + 2)
				{
					good = true;
				}
				top_num = next_top_num;
				next_num = state.puzzle[i];
				next_top_num = state.puzzle[i];
				
			}
			else if(count > 1 and next_num == next_top_num)
			{
				
				if(state.puzzle[i] == next_num + 1)
				{
					top_num = next_top_num;
					next_num = state.puzzle[i];
					next_top_num = state.puzzle[i];
				}
				else
				{
					right = false;
			if(unlocked)
			{
				return 0;
			}
			else if(flip1 and flip2)
			{
				return 1;
			}
				}
			}
		else
		{
			right = false;
			if(unlocked)
			{
				return 0;
			}
			else if(flip1 and flip2)
			{
				return 1;
			}
		}
		}
		if(!flip1 or !flip2)
		{
		if(state.puzzle[i] == top - 1)
		{
			if(abs(state.puzzle[i - 1] - top + 1) != 1)
			{
				if(gaps - 1 == 0)
				{
					unlocked = true;
					if(!right)
					{
						return 0;
					}
				}
				if(gap_decreasing_moves_no_inv_flip(state, i) == 0)
				{
					unlocked = true;
					if(!right)
					{
						return 0;
					}
				}
			}
			flip1 = true;
			if(!right and flip1 and flip2)
			{
				return 1;
			}
		}
		else if(state.puzzle[i] == top + 1)
		{
			if(abs(state.puzzle[i - 1] - top - 1) != 1)
			{
				if(gaps - 1 == 0)
				{
					unlocked = true;
					if(!right)
					{
						return 0;
					}
				}
				if(gap_decreasing_moves_no_inv_flip(state, i) == 0)
				{
					unlocked = true;
					if(!right)
					{
						return 0;
					}
				}
			}
			flip2 = true;
			if(!right and flip1 and flip2)
			{
				return 1;
			}
		}
		}
		
	}
	if(count == 2 and (next_top_num == next_num + 1 or good))
				{
					return 1;
				}
				else if(count > 1)
				{
					return 2;
				}
				else if(unlocked)
				{
					return 0;
				}
				return 1;
}

int IDA::gap_decreasing_moves_no_inv_flip(PancakeState& state, int ind) const {
	int top = state.puzzle[ind - 1];
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
	
	for(unsigned i = 0; i < state.puzzle.size(); i++)
	{
		if(state.puzzle[i] == top - 1)
		{
			if(abs(state.puzzle[place(place(i, ind) - 1, ind)] - top + 1) != 1)
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
			if(abs(state.puzzle[place(place(i, ind) - 1, ind)] - top - 1) != 1)
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


int IDA::gap_decreasing_moves_with_inv_l(PancakeState& state, int inv, int gaps) const {
	if(inv == 0)
	{
		//std::cout << "regular" << std::endl;
		if(gap_decreasing_moves(state, 0) == 0)
		{
			if(gaps - 1 == 0)
			{
				return 0;
			}
			int top = state.puzzle[0];
			if(top == 0)
			{
				//return std::max(gap_decreasing_moves2(state, state.inverse[top + 1], 0), gap_decreasing_moves2(state, state.inverse[top + 1], 1));
				return gap_decreasing_moves2(state, state.inverse[top + 1], 0);
			}
			else if(top == int(state.puzzle.size()) - 1)
			{
				//return std::min(std::max(gap_decreasing_moves2(state, state.inverse[top - 1], 0), gap_decreasing_moves2(state, state.inverse[top - 1], 1)), std::max(gap_decreasing_moves2(state, state.puzzle.size(), 0), gap_decreasing_moves2(state, state.puzzle.size(), 1)));
				if(gap_decreasing_moves2(state, state.puzzle.size(), 0) == 0)
				{
					return 0;
				}
				if(gap_decreasing_moves2(state, state.inverse[top - 1], 0) == 0)
				{
					return 0;
				}
				return 1;
				//return std::min(gap_decreasing_moves2(state, state.puzzle.size(), 0), gap_decreasing_moves2(state, state.inverse[top - 1], 0));
			}
			else
			{
				//return std::min(std::max(gap_decreasing_moves2(state, state.inverse[top - 1], 0), gap_decreasing_moves2(state, state.inverse[top - 1], 1)), std::max(gap_decreasing_moves2(state, state.inverse[top + 1], 0), gap_decreasing_moves2(state, state.inverse[top + 1], 1)));
				/*std::cout << "state" << std::endl;
				std::cout << state << std::endl;
				std::cout << "heur1" << std::endl;
				std::cout << gap_decreasing_moves2(state, state.inverse[top - 1], 0) << std::endl;
				std::cout << "heur2" << std::endl;
				std::cout << gap_decreasing_moves2(state, state.inverse[top + 1], 0) << std::endl;*/
				if(gap_decreasing_moves2(state, state.inverse[top + 1], 0) == 0)
				{
					return 0;
				}
				if(gap_decreasing_moves2(state, state.inverse[top - 1], 0) == 0)
				{
					return 0;
				}
				return 1;
				//return std::min(gap_decreasing_moves2(state, state.inverse[top + 1], 0),  gap_decreasing_moves2(state, state.inverse[top - 1], 0));
			}
		}
		else
		{
			return 1;
		}
	}
	else
	{
		//std::cout << "inverse" << std::endl;
		if(gap_decreasing_moves(state, 1) == 0)
		{
			if(gaps - 1 == 0)
			{
				return 0;
			}
			int top = state.inverse[0];
			if(top == 0)
			{
				//return std::max(gap_decreasing_moves2_v2(state, state.puzzle[top + 1], 0), gap_decreasing_moves2_v2(state, state.puzzle[top + 1], 1));
				return gap_decreasing_moves2_v2(state, state.puzzle[top + 1], 0);;
			}
			else if(top == int(state.inverse.size()) - 1)
			{
				//return std::min(std::max(gap_decreasing_moves2_v2(state, state.puzzle[top - 1], 0), gap_decreasing_moves2_v2(state, state.puzzle[top - 1], 1)), std::max(gap_decreasing_moves2_v2(state, state.inverse.size(), 0), gap_decreasing_moves2_v2(state, state.inverse.size(), 1)));
				if(gap_decreasing_moves2_v2(state, state.inverse.size(), 0) == 0)
				{
					return 0;
				}
				if(gap_decreasing_moves2_v2(state, state.puzzle[top - 1], 0) == 0)
				{
					return 0;
				}
				return 1;
				//return std::min(gap_decreasing_moves2_v2(state, state.inverse.size(), 0), gap_decreasing_moves2_v2(state, state.puzzle[top - 1], 0));
			}
			else
			{
				//return std::min(std::max(gap_decreasing_moves2_v2(state, state.puzzle[top - 1], 0), gap_decreasing_moves2_v2(state, state.puzzle[top - 1], 1)), std::max(gap_decreasing_moves2_v2(state, state.puzzle[top + 1], 0), gap_decreasing_moves2_v2(state, state.puzzle[top + 1], 1)));
				/*std::cout << "state" << std::endl;
				std::cout << state << std::endl;
				std::cout << "heur1" << std::endl;
				std::cout << gap_decreasing_moves2_v2(state, state.puzzle[top - 1], 0) << std::endl;
				std::cout << "heur2" << std::endl;
				std::cout << gap_decreasing_moves2_v2(state, state.puzzle[top + 1], 0) << std::endl;*/
				if(gap_decreasing_moves2_v2(state, state.puzzle[top + 1], 0) == 0)
				{
					return 0;
				}
				if(gap_decreasing_moves2_v2(state, state.puzzle[top - 1], 0) == 0)
				{
					return 0;
				}
				return 1;
				//return std::min(gap_decreasing_moves2_v2(state, state.puzzle[top + 1], 0),  gap_decreasing_moves2_v2(state, state.puzzle[top - 1], 0));
			}
		}
		else
		{
			return 1;
		}
	}
}

int IDA::gap_decreasing_moves_with_inv_lbf(PancakeState& state, int inv, int gaps, int flip) const {
	if(inv == 0)
	{
		//std::cout << "regular" << std::endl;
		if(gap_decreasing_moves2(state, flip, 0) == 0)
		{
			if(gaps - 1 == 0)
			{
				return 0;
			}
			int top = state.puzzle[flip - 1];
			if(top == 0)
			{
				//return std::max(gap_decreasing_moves2(state, state.inverse[top + 1], 0), gap_decreasing_moves2(state, state.inverse[top + 1], 1));
				return gap_decreasing_moves2bf(state, place(state.inverse[top + 1], flip), flip, 0);
			}
			else if(top == int(state.puzzle.size()) - 1)
			{
				//return std::min(std::max(gap_decreasing_moves2(state, state.inverse[top - 1], 0), gap_decreasing_moves2(state, state.inverse[top - 1], 1)), std::max(gap_decreasing_moves2(state, state.puzzle.size(), 0), gap_decreasing_moves2(state, state.puzzle.size(), 1)));
				return std::min(gap_decreasing_moves2bf(state, state.puzzle.size(), flip, 0), gap_decreasing_moves2bf(state, place(state.inverse[top - 1], flip), flip, 0));
			}
			else
			{
				//return std::min(std::max(gap_decreasing_moves2(state, state.inverse[top - 1], 0), gap_decreasing_moves2(state, state.inverse[top - 1], 1)), std::max(gap_decreasing_moves2(state, state.inverse[top + 1], 0), gap_decreasing_moves2(state, state.inverse[top + 1], 1)));
				/*std::cout << "state" << std::endl;
				std::cout << state << std::endl;
				std::cout << "heur1" << std::endl;
				std::cout << gap_decreasing_moves2bf(state, place(state.inverse[top + 1], flip), flip, 0) << std::endl;
				std::cout << "heur2" << std::endl;
				std::cout << gap_decreasing_moves2bf(state, place(state.inverse[top - 1], flip), flip, 0) << std::endl;*/
				/*std::cout << "normal" << std::endl;
				std::cout << std::min(gap_decreasing_moves2bf(state, place(state.inverse[top + 1], flip), flip, 0), gap_decreasing_moves2bf(state, place(state.inverse[top - 1], flip), flip, 0)) << std::endl;*/
				return std::min(gap_decreasing_moves2bf(state, place(state.inverse[top + 1], flip), flip, 0), gap_decreasing_moves2bf(state, place(state.inverse[top - 1], flip), flip, 0));
			}
		}
		else
		{
			return 1;
		}
	}
	else
	{
		//std::cout << "inverse" << std::endl;
		if(gap_decreasing_moves2(state, flip, 1) == 0)
		{
			if(gaps - 1 == 0)
			{
				return 0;
			}
			int top = place(state.inverse[0], flip);
			if(top == 0)
			{
				//return std::max(gap_decreasing_moves2_v2(state, state.puzzle[top + 1], 0), gap_decreasing_moves2_v2(state, state.puzzle[top + 1], 1));
				return gap_decreasing_moves2bf_v2(state, state.puzzle[place(top + 1, flip)], flip, 0);
			}
			else if(top == int(state.inverse.size()) - 1)
			{
				//return std::min(std::max(gap_decreasing_moves2_v2(state, state.puzzle[top - 1], 0), gap_decreasing_moves2_v2(state, state.puzzle[top - 1], 1)), std::max(gap_decreasing_moves2_v2(state, state.inverse.size(), 0), gap_decreasing_moves2_v2(state, state.inverse.size(), 1)));
				return std::min(gap_decreasing_moves2bf_v2(state, state.inverse.size(), flip, 0), gap_decreasing_moves2bf_v2(state, state.puzzle[place(top - 1, flip)], flip, 0));
			}
			else
			{
				//return std::min(std::max(gap_decreasing_moves2_v2(state, state.puzzle[top - 1], 0), gap_decreasing_moves2_v2(state, state.puzzle[top - 1], 1)), std::max(gap_decreasing_moves2_v2(state, state.puzzle[top + 1], 0), gap_decreasing_moves2_v2(state, state.puzzle[top + 1], 1)));
				/*std::cout << std::endl;
				std::cout << "heur1" << std::endl;
				std::cout << gap_decreasing_moves2bf_v2(state, state.puzzle[place(top + 1, flip)], flip, 0) << std::endl;
				std::cout << "heur2" << std::endl;
				std::cout << gap_decreasing_moves2bf_v2(state, state.puzzle[place(top - 1, flip)], flip, 0) << std::endl;*/
				/*std::cout << "inverse" << std::endl;
				std::cout << std::min(gap_decreasing_moves2bf_v2(state, state.puzzle[place(top + 1, flip)], flip, 0), gap_decreasing_moves2bf_v2(state, state.puzzle[place(top - 1, flip)], flip, 0)) << std::endl;*/
				return std::min(gap_decreasing_moves2bf_v2(state, state.puzzle[place(top + 1, flip)], flip, 0), gap_decreasing_moves2bf_v2(state, state.puzzle[place(top - 1, flip)], flip, 0));
			}
		}
		else
		{
			//std::cout << "wtffffffffffffff" << std::endl;
			return 1;
		}
	}
}

int IDA::gap_decreasing_moves_no_inv_lbf(PancakeState& state, int gaps, int flip) const {
	int top = state.puzzle[flip - 1];
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
		if(gaps - 1 == 0)
		{
			return 0;
		}
		if(gap_decreasing_moves_no_inv_flipbf(state, flip, int(state.puzzle.size())) == 0)
		{
			return 0;
		}
		flip2 = true;
	}
	for(unsigned i = 0; i < state.puzzle.size(); i++)
	{
		if(state.puzzle[i] == top - 1)
		{
			if(abs(state.puzzle[place(place(i, flip) - 1, flip)] - top + 1) != 1)
			{
				if(gaps - 1 == 0)
				{
					return 0;
				}
				if(gap_decreasing_moves_no_inv_flipbf(state, flip, place(i, flip)) == 0)
				{
					return 0;
				}
			}
			flip1 = true;
			if(flip1 and flip2)
			{
				break;
			}
		}
		else if(state.puzzle[i] == top + 1)
		{
			if(abs(state.puzzle[place(place(i, flip) - 1, flip)] - top - 1) != 1)
			{
				if(gaps - 1 == 0)
				{
					return 0;
				}
				if(gap_decreasing_moves_no_inv_flipbf(state, flip, place(i, flip)) == 0)
				{
					return 0;
				}
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

int IDA::gap_decreasing_moves_no_inv_lbffg(PancakeState& state, int gaps, int flip) const {
	if(gaps == 0)
	{
		return 0;
	}
	int top = state.puzzle[flip - 1];
	bool flip2 = false;
	bool flip1;
	int top_num = -1;
	int next_num = top;
	int next_top_num = top;
	bool right = true;
	bool unlocked = false;
	int count = 0;
	bool good = false;
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
		if(gaps - 1 == 0)
		{
			return 0;
		}
		if(gap_decreasing_moves_no_inv_flipbf(state, flip, int(state.puzzle.size())) == 0)
		{
			return 0;
		}
		flip2 = true;
	}
	for(unsigned i = 0; i < state.puzzle.size(); i++)
	{
		if(i>0)
		{
		if(right)
		{
		if(state.puzzle[place(i, flip)] == next_num - 1)
		{
			next_num = state.puzzle[place(i, flip)];
		}
		else if(next_num == top_num + 1 and next_num != next_top_num)
			{
				count++;
				if(count == 2 and next_top_num == top + 2)
				{
					good = true;
				}
				top_num = next_top_num;
				next_num = state.puzzle[place(i, flip)];
				next_top_num = state.puzzle[place(i, flip)];
				
			}
			else if(count > 1 and next_num == next_top_num)
			{
				
				if(state.puzzle[place(i, flip)] == next_num + 1)
				{
					top_num = next_top_num;
					next_num = state.puzzle[place(i, flip)];
					next_top_num = state.puzzle[place(i, flip)];
				}
				else
				{
					right = false;
			if(unlocked)
			{
				return 0;
			}
			else if(flip1 and flip2)
			{
				return 1;
			}
				}
			}
		else
		{
			right = false;
			if(unlocked)
			{
				return 0;
			}
			else if(flip1 and flip2)
			{
				return 1;
			}
		}
		}
		}
		if(!flip1 or !flip2)
		{
		if(state.puzzle[i] == top - 1)
		{
			if(abs(state.puzzle[place(place(i, flip) - 1, flip)] - top + 1) != 1)
			{
				if(gaps - 1 == 0)
				{
					unlocked = true;
					if(!right)
					{
						return 0;
					}
				}
				if(gap_decreasing_moves_no_inv_flipbf(state, flip, place(i, flip)) == 0)
				{
					unlocked = true;
					if(!right)
					{
						return 0;
					}
				}
			}
			flip1 = true;
			if(!right and flip1 and flip2)
			{
				return 1;
			}
		}
		else if(state.puzzle[i] == top + 1)
		{
			if(abs(state.puzzle[place(place(i, flip) - 1, flip)] - top - 1) != 1)
			{
				if(gaps - 1 == 0)
				{
					unlocked = true;
					if(!right)
					{
						return 0;
					}
				}
				if(gap_decreasing_moves_no_inv_flipbf(state, flip, place(i, flip)) == 0)
				{
					unlocked = true;
					if(!right)
					{
						return 0;
					}
				}
			}
			flip2 = true;
			if(!right and flip1 and flip2)
			{
				return 1;
			}
		}
		}
		
	}
	if(count == 2 and (next_top_num == next_num + 1 or good))
				{
					return 1;
				}
				else if(count > 1)
				{
					return 2;
				}
				else if(unlocked)
				{
					return 0;
				}
				return 1;
}

int IDA::gap_decreasing_moves_no_inv_flipbf(PancakeState& state, int flip, int ind) const {
	int top = state.puzzle[place(ind - 1, flip)];
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
	
	for(unsigned i = 0; i < state.puzzle.size(); i++)
	{
		if(state.puzzle[i] == top - 1)
		{
			if(abs(state.puzzle[place(place(place(place(i, flip), ind) - 1, ind), flip)] - top + 1) != 1)
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
			if(abs(state.puzzle[place(place(place(place(i, flip), ind) - 1, ind), flip)] - top - 1) != 1)
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

/*int IDA::gap_decreasing_moves_no_inv_lbf(PancakeState& state, int gaps, int flip) const {
		if(gap_decreasing_moves_no_inv_flip(state, flip) == 0)
		{
			if(gaps - 1 == 0)
			{
				return 0;
			}
			int top = state.puzzle[flip - 1];
			if(top == 0)
			{
				//return std::max(gap_decreasing_moves2(state, state.inverse[top + 1], 0), gap_decreasing_moves2(state, state.inverse[top + 1], 1));
				return gap_decreasing_moves2_no_invbf(state, place(state.inverse[top + 1], flip), flip, 0);
			}
			else if(top == int(state.puzzle.size()) - 1)
			{
				return std::min(gap_decreasing_moves2_no_invbf(state, state.puzzle.size(), flip, 0), gap_decreasing_moves2_no_invbf(state, place(state.inverse[top - 1], flip), flip, 0));
			}
			else
			{
				return std::min(gap_decreasing_moves2_no_invbf(state, place(state.inverse[top + 1], flip), flip, 0), gap_decreasing_moves2_no_invbf(state, place(state.inverse[top - 1], flip), flip, 0));
			}
		}
		else
		{
			return 1;
		}
}*/

void IDA::gap_decreasing_moves3(PancakeState& state, std::vector<unsigned short int>& moves, int inv) const {
	assert(puzzle.size == state.puzzle.size());
	
	//if only want to use this for top without look ahead, don't need to remember what moves they are, just how many are there
	if(inv == 0)
	{
	int top = state.puzzle[0];
	
	if(top == int(state.puzzle.size()) - 1)
		{
			moves.push_back(state.puzzle.size());
			int flip1 = state.inverse[top - 1];
				/*std::cout << "before" << std::endl;
				std::cout << state.puzzle[place(flip1 - 1, ind)] << std::endl;*/
				if(abs(state.puzzle[flip1 - 1] - top + 1) != 1)
				{
					//moves++;
					moves.push_back(flip1);
				}
		}
		else
		{
			if(top != 0)
			{
				int flip1 = state.inverse[top - 1];
				/*std::cout << "before" << std::endl;
				std::cout << state.puzzle[place(flip1 - 1, ind)] << std::endl;*/
				if(abs(state.puzzle[flip1 - 1] - top + 1) != 1)
				{
					//moves++;
					moves.push_back(flip1);
				}
			/*std::cout << "flip1" << std::endl;
			std::cout << flip1 << std::endl;
			std::cout << "where1" << std::endl;
			std::cout << where1 << std::endl;*/
			}
			int flip2 = state.inverse[top + 1];
			if(abs(state.puzzle[flip2 - 1] - top - 1) != 1)
			{
				//moves++;
				//std::cout << "safsadf" << std::endl;
				moves.push_back(flip2);
			}
			
			/*std::cout << "flip2" << std::endl;
			std::cout << flip2 << std::endl;
			
			std::cout << "where2" << std::endl;
			std::cout << where2 << std::endl;*/
		}
	}
	else
	{
	int top = state.inverse[0];
	
	if(top == int(state.inverse.size()) - 1)
		{
			moves.push_back(state.inverse.size());
			int flip1 = state.puzzle[top - 1];
				/*std::cout << "before" << std::endl;
				std::cout << state.puzzle[place(flip1 - 1, ind)] << std::endl;*/
				if(abs(state.inverse[flip1 - 1] - top + 1) != 1)
				{
					//moves++;
					moves.push_back(flip1);
				}
		}
		else
		{
			if(top != 0)
			{
				int flip1 = state.puzzle[top - 1];
				/*std::cout << "before" << std::endl;
				std::cout << state.puzzle[place(flip1 - 1, ind)] << std::endl;*/
				if(abs(state.inverse[flip1 - 1] - top + 1) != 1)
				{
					//moves++;
					moves.push_back(flip1);
				}
			/*std::cout << "flip1" << std::endl;
			std::cout << flip1 << std::endl;
			std::cout << "where1" << std::endl;
			std::cout << where1 << std::endl;*/
			}
			int flip2 = state.puzzle[top + 1];
			if(abs(state.inverse[flip2 - 1] - top - 1) != 1)
			{
				//moves++;
				//std::cout << "safsadf" << std::endl;
				moves.push_back(flip2);
			}
			
			/*std::cout << "flip2" << std::endl;
			std::cout << flip2 << std::endl;
			
			std::cout << "where2" << std::endl;
			std::cout << where2 << std::endl;*/
		}
	}
}

int IDA::gap_decreasing_moves(PancakeState& state, int inv) const {
	assert(puzzle.size == state.puzzle.size());
	
	//if only want to use this for top without look ahead, don't need to remember what moves they are, just how many are there
	if(inv == 0)
	{
	int top = state.puzzle[0];
	
	if(top == int(state.puzzle.size()) - 1)
		{
			/*moves++;
			int flip = state.inverse[top - 1];
			int where = sgn(ind - 1, flip);
			if(state.puzzle[flip + where] != top - 2 && (where == -1 or state.puzzle[flip + where] != top))
			{
				moves++;
			}*/
			return 0;
		}
		else
		{
			if(top != 0)
			{
				int flip1 = state.inverse[top - 1];
				/*std::cout << "before" << std::endl;
				std::cout << state.puzzle[place(flip1 - 1, ind)] << std::endl;*/
				if(abs(state.puzzle[flip1 - 1] - top + 1) != 1)
				{
					//moves++;
					return 0;
				}
			/*std::cout << "flip1" << std::endl;
			std::cout << flip1 << std::endl;
			std::cout << "where1" << std::endl;
			std::cout << where1 << std::endl;*/
			}
			int flip2 = state.inverse[top + 1];
			if(abs(state.puzzle[flip2 - 1] - top - 1) != 1)
			{
				//moves++;
				//std::cout << "safsadf" << std::endl;
				return 0;
			}
			
			/*std::cout << "flip2" << std::endl;
			std::cout << flip2 << std::endl;
			
			std::cout << "where2" << std::endl;
			std::cout << where2 << std::endl;*/
			return 1;
		}
	}
	else
	{
	int top = state.inverse[0];
	
	if(top == int(state.inverse.size()) - 1)
		{
			/*moves++;
			int flip = state.inverse[top - 1];
			int where = sgn(ind - 1, flip);
			if(state.puzzle[flip + where] != top - 2 && (where == -1 or state.puzzle[flip + where] != top))
			{
				moves++;
			}*/
			return 0;
		}
		else
		{
			if(top != 0)
			{
				int flip1 = state.puzzle[top - 1];
				/*std::cout << "before" << std::endl;
				std::cout << state.puzzle[place(flip1 - 1, ind)] << std::endl;*/
				if(abs(state.inverse[flip1 - 1] - top + 1) != 1)
				{
					//moves++;
					return 0;
				}
			/*std::cout << "flip1" << std::endl;
			std::cout << flip1 << std::endl;
			std::cout << "where1" << std::endl;
			std::cout << where1 << std::endl;*/
			}
			int flip2 = state.puzzle[top + 1];
			if(abs(state.inverse[flip2 - 1] - top - 1) != 1)
			{
				//moves++;
				//std::cout << "safsadf" << std::endl;
				return 0;
			}
			
			/*std::cout << "flip2" << std::endl;
			std::cout << flip2 << std::endl;
			
			std::cout << "where2" << std::endl;
			std::cout << where2 << std::endl;*/
			return 1;
		}
	}
}

int IDA::gap_decreasing_moves2(PancakeState& state, int ind, int inv) const {
	assert(puzzle.size == state.puzzle.size());
	if(ind == 1)
	{
		return 1;
	}
	//if only want to use this for top without look ahead, don't need to remember what moves they are, just how many are there
	//int moves = 0;
	if(inv == 0)
	{
	int top = state.puzzle[ind - 1];
		if(top == int(state.puzzle.size()) - 1)
		{
			/*moves++;
			int flip = state.inverse[top - 1];
			int where = sgn(ind - 1, flip);
			if(state.puzzle[flip + where] != top - 2 && (where == -1 or state.puzzle[flip + where] != top))
			{
				moves++;
			}*/
			return 0;
		}
		else
		{
			if(top != 0)
			{
				int flip1 = place(state.inverse[top - 1], ind);
				if(abs(state.puzzle[place(flip1 - 1, ind)] - top + 1) != 1)
				{
					//moves++;
					return 0;
				}
			/*std::cout << "flip1" << std::endl;
			std::cout << flip1 << std::endl;
			std::cout << "where1" << std::endl;
			std::cout << where1 << std::endl;*/
			}
			int flip2 = place(state.inverse[top + 1], ind);
			if(abs(state.puzzle[place(flip2 - 1, ind)] - top - 1) != 1)
			{
				//moves++;
				//std::cout << "safsadf" << std::endl;
				return 0;
			}
			
			/*std::cout << "flip2" << std::endl;
			std::cout << flip2 << std::endl;
			
			std::cout << "where2" << std::endl;
			std::cout << where2 << std::endl;*/
			return 1;
		}
	}
	else
	{
		int top = place(state.inverse[0], ind);
		if(top == int(state.inverse.size()) - 1)
		{
			/*moves++;
			int flip = state.inverse[top - 1];
			int where = sgn(ind - 1, flip);
			if(state.puzzle[flip + where] != top - 2 && (where == -1 or state.puzzle[flip + where] != top))
			{
				moves++;
			}*/
			return 0;
		}
		else
		{
			if(top != 0)
			{
				int flip1 = state.puzzle[place(top - 1, ind)];
				if(abs(place(state.inverse[flip1 - 1], ind) - top + 1) != 1)
				{
					//moves++;
					return 0;
				}
			/*std::cout << "flip1" << std::endl;
			std::cout << flip1 << std::endl;
			std::cout << "where1" << std::endl;
			std::cout << place(state.inverse[place(flip1 - 1, ind)], flip) << std::endl;*/
			}
			int flip2 = state.puzzle[place(top + 1, ind)];
			if(abs(place(state.inverse[flip2 - 1], ind) - top - 1) != 1)
			{
				//moves++;
				return 0;
			}
			
			/*std::cout << "flip2" << std::endl;
			std::cout << flip2 << std::endl;
			
			std::cout << "where2" << std::endl;
			std::cout << place(state.inverse[place(flip2 - 1, ind)], flip) << std::endl;*/
			return 1;
		}
		
	}
	return 1;
}
//flip is bf, ind is gap dec
int IDA::gap_decreasing_moves2bf(PancakeState& state, int ind, int flip, int inv) const {
	assert(puzzle.size == state.puzzle.size());
	if(ind == 1)
	{
		return 1;
	}
	//if only want to use this for top without look ahead, don't need to remember what moves they are, just how many are there
	//int moves = 0;
		int top = state.puzzle[place(ind - 1, flip)];
		if(top == int(state.puzzle.size()) - 1)
		{
			/*moves++;
			int flip = state.inverse[top - 1];
			int where = sgn(ind - 1, flip);
			if(state.puzzle[flip + where] != top - 2 && (where == -1 or state.puzzle[flip + where] != top))
			{
				moves++;
			}*/
			return 0;
		}
		else
		{
			if(top != 0)
			{
				int flip1 = place(place(state.inverse[top - 1], flip), ind);
				if(abs(state.puzzle[place(place(flip1 - 1, ind), flip)] - top + 1) != 1)
				{
					//moves++;
					return 0;
				}
			/*std::cout << "flip1" << std::endl;
			std::cout << flip1 << std::endl;
			std::cout << "where1" << std::endl;
			std::cout << where1 << std::endl;*/
			}
			int flip2 = place(place(state.inverse[top + 1], flip), ind);
			if(abs(state.puzzle[place(place(flip2 - 1, ind), flip)] - top - 1) != 1)
			{
				//moves++;
				//std::cout << "safsadf" << std::endl;
				return 0;
			}
			
			/*std::cout << "flip2" << std::endl;
			std::cout << flip2 << std::endl;
			
			std::cout << "where2" << std::endl;
			std::cout << where2 << std::endl;*/
			return 1;
		}
}
//flip is bf, ind is inverse flip
int IDA::gap_decreasing_moves2bf_v2(PancakeState& state, int ind, int flip, int inv) const {
	assert(puzzle.size == state.puzzle.size());
	if(ind == 1)
	{
		return 1;
	}
	//if only want to use this for top without look ahead, don't need to remember what moves they are, just how many are there
	//int moves = 0;
		//state.puzzle[place(top + 1, flip)]]
		int top = place(state.inverse[ind - 1], flip);
		if(top == int(state.inverse.size()) - 1)
		{
			/*moves++;
			int flip = state.inverse[top - 1];
			int where = sgn(ind - 1, flip);
			if(state.puzzle[flip + where] != top - 2 && (where == -1 or state.puzzle[flip + where] != top))
			{
				moves++;
			}*/
			return 0;
		}
		else
		{
			if(top != 0)
			{
				int flip1 = place(state.puzzle[place(top - 1, flip)], ind);
				if(abs(place(state.inverse[place(flip1 - 1, ind)], flip) - top + 1) != 1)
				{
					//moves++;
					return 0;
				}
			/*std::cout << "flip1" << std::endl;
			std::cout << flip1 << std::endl;
			std::cout << "where1" << std::endl;
			std::cout << place(state.inverse[place(flip1 - 1, ind)], flip) << std::endl;*/
			}
			int flip2 = place(state.puzzle[place(top + 1, flip)], ind);
			if(abs(place(state.inverse[place(flip2 - 1, ind)], flip) - top - 1) != 1)
			{
				//moves++;
				//std::cout << "safsadf" << std::endl;
				return 0;
			}
			
			/*std::cout << "flip2" << std::endl;
			std::cout << flip2 << std::endl;
			
			std::cout << "where2" << std::endl;
			std::cout << place(state.inverse[place(flip2 - 1, ind)], flip) << std::endl;*/
			return 1;
		}
}

int IDA::gap_decreasing_moves2_v2(PancakeState& state, int ind, int inv) const {
	assert(puzzle.size == state.puzzle.size());
	if(ind == 1)
	{
		return 1;
	}
	//if only want to use this for top without look ahead, don't need to remember what moves they are, just how many are there
	//int moves = 0;
	if(inv == 0)
	{
		int top = state.inverse[ind - 1];
		if(top == int(state.inverse.size()) - 1)
		{
			/*moves++;
			int flip = state.inverse[top - 1];
			int where = sgn(ind - 1, flip);
			if(state.puzzle[flip + where] != top - 2 && (where == -1 or state.puzzle[flip + where] != top))
			{
				moves++;
			}*/
			return 0;
		}
		else
		{
			if(top != 0)
			{
				int flip1 = place(state.puzzle[top - 1], ind);
				if(abs(state.inverse[place(flip1 - 1, ind)] - top + 1) != 1)
				{
					//moves++;
					return 0;
				}
			/*std::cout << "flip1" << std::endl;
			std::cout << flip1 << std::endl;
			std::cout << "where1" << std::endl;
			std::cout << where1 << std::endl;*/
			}
			int flip2 = place(state.puzzle[top + 1], ind);
			if(abs(state.inverse[place(flip2 - 1, ind)] - top - 1) != 1)
			{
				//moves++;
				//std::cout << "safsadf" << std::endl;
				return 0;
			}
			
			/*std::cout << "flip2" << std::endl;
			std::cout << flip2 << std::endl;
			
			std::cout << "where2" << std::endl;
			std::cout << where2 << std::endl;*/
			return 1;
		}
	}
	else
	{
		int afterpos = place(state.puzzle[0], ind);
		if(afterpos == int(state.inverse.size()) - 1)
		{
			/*moves++;
			int flip = state.inverse[top - 1];
			int where = sgn(ind - 1, flip);
			if(state.puzzle[flip + where] != top - 2 && (where == -1 or state.puzzle[flip + where] != top))
			{
				moves++;
			}*/
			return 0;
		}
		else
		{
			if(afterpos == 0 && state.inverse[place(1, ind)] - state.inverse[place(2, ind)] == 1)
			{
				return 1;
			}
			int diff1 = state.inverse[place(afterpos + 1, ind)] - state.inverse[place(afterpos + 2, ind)];
			int diff2 = state.inverse[place(afterpos - 1, ind)] - state.inverse[place(afterpos - 2, ind)];
			if(afterpos > 1 && (afterpos + 1 == place(state.puzzle[1], ind) || diff1 == 1) && diff2 == 1)
			{
				return 1;
			}
			if(afterpos < int(state.inverse.size()) - 2 && (afterpos - 1 == place(state.puzzle[1], ind) || diff2 == 1) && diff1 == 1)
			{
				return 1;
			}
			return 0;
		}
		/*int top = place(state.inverse[0], ind);
	if(state.inverse.size() >= 2)
	{
		if(top == int(state.inverse.size()) - 1)
		{
			return 0;
		}
		else
		{
			if(top != 0)
			{
				int flip1 = state.puzzle[top - 1];
				int where1 = place(flip1, ind)
				if(state.inverse[state.puzzle[where1] - 1] != top - 2 && (where1 == -1 or state.inverse[flip1 + where1] != top))
				{
					//moves++;
					return 0;
				}
			}
			int flip2 = state.puzzle[top + 1];
			int where2 = sgn(ind - 1, flip2);
			if(state.inverse[flip2 + where2] != top + 2 && (where2 == -1 or state.inverse[flip2 + where2] != top))
			{
				//moves++;
				return 0;
			}
			return 1;
		}
	}*/
		
	}
	return 1;
}

/*void IDA::gap_decreasing_moves5(PancakeState& state, int ind, std::vector<unsigned short int>& moves, int inv) const {
	assert(puzzle.size == state.puzzle.size());
	if(inv == 0)
	{
	int top = state.puzzle[ind - 1];
		if(top == int(state.puzzle.size()) - 1)
		{
			moves.push_back(state.puzzle.size());
		}
		else
		{
			if(top != 0)
			{
				int flip1 = place(state.inverse[top - 1], ind);
				if(abs(state.puzzle[place(flip1 - 1, ind)] - top + 1) != 1)
				{
					moves.push_back(flip1);
				}
			}
			int flip2 = place(state.inverse[top + 1], ind);
			if(abs(state.puzzle[place(flip2 - 1, ind)] - top - 1) != 1)
			{
				moves.push_back(flip2);
			}
		}
	}
	else
	{
		
		int afterpos = place(state.inverse[0], ind);
		
		int pan2 = state.puzzle[place(afterpos - 1, ind)];
		int pan1 = state.puzzle[place(afterpos + 1, ind)];
		int diff2 = pan2 - state.puzzle[place(afterpos - 2, ind)];
		int diff1 = pan1 - state.puzzle[place(afterpos + 2, ind)];
		if(afterpos == int(state.puzzle.size()) - 1)
		{
			moves.push_back(state.puzzle.size());
			if(diff2 != 1)
			{
				moves.push_back(pan2);
			}
		}
		else if(afterpos == 0 && diff1 != 1)
		{
			moves.push_back(pan1);
		}
		else if(afterpos == int(state.puzzle.size()) - 2)
		{
			
		}
		else
		{
			
			
			moves.push_back(pan2)
			if(!(afterpos == 0 && state.puzzle[place(1, ind)] - state.puzzle[place(2, ind)] == 1))
			{
				moves.remove()
			}
			
			int diff2 = state.puzzle[place(afterpos - 1, ind)] - state.puzzle[place(afterpos - 2, ind)];
			if(afterpos > 1 && (afterpos + 1 == place(state.inverse[1], ind) || diff1 == 1) && diff2 == 1)
			{
				moves.push_back(state.puzzle[place(1, ind)]);
			}
			if(afterpos < int(state.puzzle.size()) - 2 && (afterpos - 1 == place(state.inverse[1], ind) || diff2 == 1) && diff1 == 1)
			{
				moves.push_back(state.puzzle[place(1, ind)]);
			}
		}
		
	}
	return 1;
}*/

void IDA::top_decreasing_moves(PancakeState& state, std::vector<unsigned short int>& moves) const {
	assert(puzzle.size == state.puzzle.size());
	
	int top = state.puzzle[0];
	if(state.puzzle.size() >= 2)
	{
		if(top == int(state.puzzle.size()) - 1)
		{
			moves.push_back(int(state.puzzle.size()));
		}
		for(int i = 1; i < int(puzzle.size) - 1; i++)
		{
            		if(state.puzzle[i] != top - 2 && state.puzzle[i + 1] == top - 1)
            		{
            			puzzle.ApplyAction(state, i + 1);
            			if(locked_or_not(state))
            			{
            				moves.push_back(i + 1);
            			}
            			puzzle.ApplyAction(state, i + 1);
            		}
            		if(state.puzzle[i] != top + 2 && state.puzzle[i + 1] == top + 1)
            		{
            			puzzle.ApplyAction(state, i + 1);
            			if(locked_or_not(state))
            			{
            				moves.push_back(i + 1);
            			}
            			puzzle.ApplyAction(state, i + 1);
            		}
    		}
	}
}

void IDA::gap_creating_moves(PancakeState& state, std::vector<unsigned short int>& moves) const {
	assert(puzzle.size == state.puzzle.size());
	
	if(state.puzzle.size() >= 2)
	{
		for(int i = 1; i < int(puzzle.size) - 1; i++)
		{
            		if(state.puzzle[i] == state.puzzle[i + 1] + 1 || state.puzzle[i] == state.puzzle[i + 1] - 1)
            		{
            			moves.push_back(i + 1);
            		}
    		}
	}
}

bool IDA::locked_or_not2(PancakeState& state) const {
	assert(puzzle.size == state.puzzle.size());
	
	int lock_type = -1;
	int top = state.puzzle[0];
	bool half_true = false;
	if(state.puzzle.size() >= 2)
	{
		if(top == int(state.puzzle.size()) - 1)
		{
			puzzle.ApplyAction(state, state.puzzle.size());
            		if(locked_or_not(state))
            		{
            			puzzle.ApplyAction(state, state.puzzle.size());
            			return true;
            		}
            		else
            		{
            			puzzle.ApplyAction(state, state.puzzle.size());
            			return false;
            		}
		}
		if(state.puzzle[1] - top == 1)
		{
			if(top == 0)
			{
				return true;
			}
			lock_type = 0;
		}
		else if(state.puzzle[1] - top == -1)
		{
			lock_type = 1;
		}
		else
		{
			if(top == 0)
			{
				half_true = true;
			}
			lock_type = 2;
		}
		for(int i = 1; i < int(puzzle.size) - 1; i++)
		{
            		if(lock_type == 0 && state.puzzle[i] == top - 2 && state.puzzle[i + 1] == top - 1)
            		{
            			return true;
            		}
            		else if(lock_type == 0 && state.puzzle[i + 1] == top - 1)
            		{
            			//std::cout << "lala" << std::endl;
            			puzzle.ApplyAction(state, i + 1);
            			//std::cout << state << std::endl;
            			if(locked_or_not(state))
            			{
            				//std::cout << "locked" << std::endl;
            				puzzle.ApplyAction(state, i + 1);
            				return true;
            			}
            			else
            			{
            				puzzle.ApplyAction(state, i + 1);
            				return false;
            			}
            		}
            		if(lock_type == 1 && state.puzzle[i] == top + 2 && state.puzzle[i + 1] == top + 1)
            		{
            			return true;
            		}
            		else if(lock_type == 1 && state.puzzle[i + 1] == top + 1)
            		{
            			puzzle.ApplyAction(state, i + 1);
            			if(locked_or_not(state))
            			{
            				puzzle.ApplyAction(state, i + 1);
            				return true;
            			}
            			else
            			{
            				puzzle.ApplyAction(state, i + 1);
            				return false;
            			}
            		}
            		if(lock_type == 2 && state.puzzle[i] == top - 2 && state.puzzle[i + 1] == top - 1)
            		{
            			if(half_true)
            			{
            				return true;
            			}
            			half_true = true;
            		}
            		else if(lock_type == 2 && state.puzzle[i + 1] == top - 1)
            		{
            			puzzle.ApplyAction(state, i + 1);
            			if(locked_or_not(state))
            			{
            				puzzle.ApplyAction(state, i + 1);
            				if(half_true)
            				{
            					return true;
            				}
            				half_true = true;
            			}
            			else
            			{
            				puzzle.ApplyAction(state, i + 1);
            				return false;
            			}
            		}
            		if(lock_type == 2 && state.puzzle[i] == top + 2 && state.puzzle[i + 1] == top + 1)
            		{
            			if(half_true)
            			{
            				return true;
            			}
            			half_true = true;
            		}
            		else if(lock_type == 2 && state.puzzle[i + 1] == top + 1)
            		{
            			puzzle.ApplyAction(state, i + 1);
            			if(locked_or_not(state))
            			{
            				puzzle.ApplyAction(state, i + 1);
            				if(half_true)
            				{
            					return true;
            				}
            				half_true = true;
            			}
            			else
            			{
            				puzzle.ApplyAction(state, i + 1);
            				return false;
            			}
            		}
    		}
	}
    	return false;
}

bool IDA::one_move_only(PancakeState& state) const {
	assert(puzzle.size == state.puzzle.size());
	
	int lock_type = -1;
	int top = state.puzzle[0];
	int moves = 2;
	if(state.puzzle.size() >= 2)
	{
		if(top == int(state.puzzle.size()) - 1)
		{
			return true;
		}
		if(state.puzzle[1] - top == 1)
		{
			if(top == 0)
			{
				return false;
			}
			if(top == 1)
			{
				return true;
			}
			lock_type = 0;
		}
		else if(state.puzzle[1] - top == -1)
		{
			if(top == int(state.puzzle.size()) - 2)
			{
				return true;
			}
			lock_type = 1;
		}
		else
		{
			if(top == 0)
			{
				moves--;
			}
			lock_type = 2;
		}
		if(lock_type == -1)
		{
			return false;
		}
		for(int i = 1; i < int(puzzle.size) - 1; i++)
		{
            		if(lock_type == 0 && state.puzzle[i] == top - 2 && state.puzzle[i + 1] == top - 1)
            		{
            			return false;
            		}
            		else if(lock_type == 0 && state.puzzle[i + 1] == top - 1)
            		{
            			return true;
            		}
            		if(lock_type == 1 && state.puzzle[i] == top + 2 && state.puzzle[i + 1] == top + 1)
            		{
            			return false;
            		}
            		else if(lock_type == 1 && state.puzzle[i + 1] == top + 1)
            		{
            			return true;
            		}
            		if(lock_type == 2 && state.puzzle[i] == top - 2 && state.puzzle[i + 1] == top - 1)
            		{
            			moves--;
            		}
            		if(lock_type == 2 && state.puzzle[i] == top + 2 && state.puzzle[i + 1] == top + 1)
            		{
            			moves--;
            		}
    		}
    		if(lock_type == 2 and (moves == 0 or moves == 2))
    		{
    			return false;
    		}
    		else
    		{
    			return true;
    		}
	}
    	return true;
}

int IDA::Search(PancakeState& state, int g, int heur, int depth, int parent) {
	assert(puzzle.size == state.puzzle.size());
	
	
	if(puzzle.GoalTest(state))
	{
		//std::cout << "solution" << std::endl;
		return -g;
	}
	int f = g + heur;
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	int trans_cost;
	bool set = false;
	int min = -1;
	int temp = 0;
	int h;
	//can optimize this by not calling here instead pass it in to the function
	bool locked = locked_or_not2(state);
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;
	std::cout << locked << std::endl;*/
	/*if(greater_pruning && equal_pruning)
	{
	std::cout << "state" << std::endl;
	std::cout << state << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;
	std::cout << locked << std::endl;
	}*/
	//std::cout << "state" << std::endl;
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;
	std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	for(int i = 2; i <= int(puzzle.size); i++)
	{
		if(i == parent)
		{
			continue;
		}
		trans_cost = puzzle.GetActionCost(state, i);
		puzzle.ApplyAction(state, i);
		/*std::cout << "top" << std::endl;
		std::cout << TopHeur(state) << std::endl;*/
		h = TopHeur(state, GapHeur(state));
		counter++;
		/*std::cout << "i" << std::endl;
		std::cout << i << std::endl;
		std::cout << "h" << std::endl;
		std::cout << h << std::endl;
		std::cout << "parent h" << std::endl;
		std::cout << f - g << std::endl;*/
		if((greater_pruning && h > f - g) || (equal_pruning && !locked && h == f - g))
		{
			puzzle.ApplyAction(state, i);
			continue;
		}
		//std::cout << "here" << std::endl;
		temp = Search(state, g + trans_cost, h, depth, i);
		if(temp < 0)
		{
			path.push_back(i);
			puzzle.ApplyAction(state, i);
			return temp;
		}
		puzzle.ApplyAction(state, i);
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	/*std::cout << "min" << std::endl;
	std::cout << min << std::endl;*/
	return min;
}

int IDA::new_search(PancakeState& state, int g, int heur, int depth, int parent) {
	assert(puzzle.size == state.puzzle.size());
	
	/*if(counter == 10 && weight > 1.1)
	{
		return -g;
	}*/
	if(puzzle.GoalTest(state))
	{
		//std::cout << "solution" << std::endl;
		return -g;
	}
	int f = g + heur;
	//std::cout << counter << std::endl;
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << "f" << std::endl;
	//std::cout << f << std::endl;
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	int trans_cost;
	int min = -1;
	int temp = 0;
	int h;
	bool set = false;
	//bool locked = locked_or_not(state);
	bool one_move = one_move_only(state);
	//std::cout << "state" << std::endl;
	//std::cout << state << std::endl;
	//std::cout << locked << std::endl;
	for(int i = 2; i <= int(puzzle.size); i++)
	{
		if(i == parent)
		{
			continue;
		}
		trans_cost = puzzle.GetActionCost(state, i);
		puzzle.ApplyAction(state, i);
		/*std::cout << "gap" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		h = GapHeur(state);
		counter++;
		if((greater_pruning && h > f - g) || (equal_pruning && one_move && h == f - g))
		{
			puzzle.ApplyAction(state, i);
			continue;
		}
		temp = new_search(state, g + trans_cost, h, depth, i);
		if(temp < 0)
		{
			path.push_back(i);
			puzzle.ApplyAction(state, i);
			return temp;
		}
		puzzle.ApplyAction(state, i);
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	return min;
}

int IDA::Search3(PancakeState& state, int g, int heur, int depth, int parent, int gaps, clock_t &t1) {
	/*std::cout << "puzzle size" << std::endl;
	std::cout << puzzle.size << std::endl;
	std::cout << "state puzzle size" << std::endl;
	std::cout << state.puzzle.size() << std::endl;*/
	assert(puzzle.size == state.puzzle.size());
	
	/*if(counter == 10 && weight > 1.1)
	{
		return -g;
	}*/
	if(puzzle.GoalTest(state))
	{
		//std::cout << "solution" << std::endl;
		/*solution_count++;
		std::cout << solution_count << std::endl;
		if(solution_count < 3)
		{
			return 10000;
		}
		else
		{*/
			return -g;
		//}
	}
	int f = g + heur;
	//std::cout << counter << std::endl;
	/*std::cout << "normal state" << std::endl;
	std::cout << state << std::endl;*/
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << "f" << std::endl;
	//std::cout << f << std::endl;
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	clock_t t2;
	t2=clock();
	double diff1;
    	diff1 = ((double)(t2-t1)) / CLOCKS_PER_SEC;
    	if(diff1 > 1800)
    	{
    		return -100;
    	}
	int trans_cost;
	int min = f + 1;
	int temp = 0;
	int h;
	int ggaps = 0;
	bool set = false;
	
	
	/*std::vector<unsigned short int> moves;
	if(f == depth)
	{
		gap_decreasing_moves4(state, moves);
	}
	else
	{
		for(int i = 2; i <= int(puzzle.size); i++)
		{
			moves.push_back(i);
		}
	}*/
	/*bool locked = locked_or_not(state);
	bool one_move = one_move_only(state);*/
	//std::cout << locked << std::endl;
	//int i;
	for(int i = 2; i <= int(puzzle.size); i++/*int j = 0; j < int(moves.size()); j++*/)
	{
		//i = moves[j];
		if(i == parent)
		{
			continue;
		}
		ggaps = gaps;
		trans_cost = puzzle.GetActionCost(state, i);
		//put gap code here and compare
		puzzle.ApplyAction2(state, i);
		/*std::cout << "child" << std::endl;
		std::cout << state << std::endl;*/
		/*std::cout << "action" << std::endl;
		std::cout << i << std::endl;*/
		/*std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << ' ';
		}
		std::cout << std::endl;*/
		/*std::cout << "gap" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		if(i != int(puzzle.size))
		{
		if(int(abs(state.puzzle[0] - state.puzzle[i])) == 1)
		{
			ggaps++;
		}
		if(int(abs(state.puzzle[i - 1] - state.puzzle[i])) == 1)
		{
			ggaps--;
		}
		}
		else
		{
			if(state.puzzle[i - 1] == int(state.puzzle.size()) - 1)
			{
				ggaps--;
			}
			else if(state.puzzle[0] == int(state.puzzle.size()) - 1)
			{
				ggaps++;
			}
		}
		/*std::cout << "i" << std::endl;
		std::cout << i << std::endl;*/
		/*std::cout << "gapps" << std::endl;
		std::cout << ggaps << std::endl;
		std::cout << "gaapps" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
		h = ggaps;
		/*if(h == 0 and i == 3 and parent == std::min(int(puzzle.size), i + 2))
		{
			puzzle.ApplyAction(state, i);
			continue;
		}*/
		/*if(!locked && !one_move && !found && h < heur)
		{
			found = true;
			move = i;
		}*/
		/*if(gaps - ggaps == 1)
		{*/
			counter++;
		//}
		/*if((greater_pruning && !locked && h > f - g) || (equal_pruning && !locked && h == f - g))
		{
			puzzle.ApplyAction(state, i);
			continue;
		}*/
		temp = Search3(state, g + trans_cost, h, depth, i, ggaps, t1);
		/*if(temp == 10000)
		{
			puzzle.ApplyAction(state, i);
			continue;
		}*/
		if(temp < 0)
		{
			path.push_back(i);
			puzzle.ApplyAction2(state, i);
			/*if(heur == h)
			{
				index = path.size();
				flip = i;
			}*/
			return temp;
		}
		puzzle.ApplyAction2(state, i);
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	return min;
}

int IDA::Search3_Prune(PancakeState& state, int g, int heur, int depth, int parent, int gaps) {
	/*std::cout << "puzzle size" << std::endl;
	std::cout << puzzle.size << std::endl;
	std::cout << "state puzzle size" << std::endl;
	std::cout << state.puzzle.size() << std::endl;*/
	assert(puzzle.size == state.puzzle.size());
	
	/*if(counter == 10 && weight > 1.1)
	{
		return -g;
	}*/
	if(puzzle.GoalTest(state))
	{
		//std::cout << "solution" << std::endl;
		/*solution_count++;
		std::cout << solution_count << std::endl;
		if(solution_count < 3)
		{
			return 10000;
		}
		else
		{*/
			return -g;
		//}
	}
	int f = g + heur;
	//std::cout << counter << std::endl;
	/*std::cout << "normal state" << std::endl;
	std::cout << state << std::endl;*/
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << "f" << std::endl;
	//std::cout << f << std::endl;
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	int trans_cost;
	int min = f + 1;
	int temp = 0;
	int h;
	int ggaps = 0;
	bool set = false;
	/*std::vector<unsigned short int> moves;
	if(f == depth)
	{
		gap_decreasing_moves4(state, moves);
	}
	else
	{
		for(int i = 2; i <= int(puzzle.size); i++)
		{
			moves.push_back(i);
		}
	}*/
	/*bool locked = locked_or_not(state);
	bool one_move = one_move_only(state);*/
	//std::cout << locked << std::endl;
	//int i;
	for(int i = 2; i <= int(puzzle.size); i++/*int j = 0; j < int(moves.size()); j++*/)
	{
		//i = moves[j];
		if(i == parent)
		{
			continue;
		}
		if(i == int(puzzle.size) - 1)
		{
			if(abs(state.puzzle[i] - state.puzzle[i - 1]) == 1 and abs(state.puzzle[i] - state.puzzle[i - 2]) == 1)
			{
				continue;
			}
		}
		else if(i == int(puzzle.size))
		{
			
		}
		else
		{
			if((abs(state.puzzle[i] - state.puzzle[i - 1]) == 1 and abs(state.puzzle[i] - state.puzzle[i + 1]) == 1) or (abs(state.puzzle[i] - state.puzzle[i - 1]) == 1 and abs(state.puzzle[i] - state.puzzle[i - 2]) == 1))
			{
				continue;
			}
		}
		ggaps = gaps;
		trans_cost = puzzle.GetActionCost(state, i);
		//put gap code here and compare
		puzzle.ApplyAction2(state, i);
		/*std::cout << "child" << std::endl;
		std::cout << state << std::endl;*/
		/*std::cout << "action" << std::endl;
		std::cout << i << std::endl;*/
		/*std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << ' ';
		}
		std::cout << std::endl;*/
		/*std::cout << "gap" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		if(i != int(puzzle.size))
		{
		if(int(abs(state.puzzle[0] - state.puzzle[i])) == 1)
		{
			ggaps++;
		}
		if(int(abs(state.puzzle[i - 1] - state.puzzle[i])) == 1)
		{
			ggaps--;
		}
		}
		else
		{
			if(state.puzzle[i - 1] == int(state.puzzle.size()) - 1)
			{
				ggaps--;
			}
			else if(state.puzzle[0] == int(state.puzzle.size()) - 1)
			{
				ggaps++;
			}
		}
		/*std::cout << "i" << std::endl;
		std::cout << i << std::endl;*/
		/*std::cout << "gapps" << std::endl;
		std::cout << ggaps << std::endl;
		std::cout << "gaapps" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
		h = ggaps;
		/*if(h == 0 and i == 3 and parent == std::min(int(puzzle.size), i + 2))
		{
			puzzle.ApplyAction(state, i);
			continue;
		}*/
		/*if(!locked && !one_move && !found && h < heur)
		{
			found = true;
			move = i;
		}*/
		counter++;
		/*if((greater_pruning && !locked && h > f - g) || (equal_pruning && !locked && h == f - g))
		{
			puzzle.ApplyAction(state, i);
			continue;
		}*/
		temp = Search3_Prune(state, g + trans_cost, h, depth, i, ggaps);
		/*if(temp == 10000)
		{
			puzzle.ApplyAction(state, i);
			continue;
		}*/
		if(temp < 0)
		{
			path.push_back(i);
			puzzle.ApplyAction2(state, i);
			/*if(heur == h)
			{
				index = path.size();
				flip = i;
			}*/
			return temp;
		}
		puzzle.ApplyAction2(state, i);
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	return min;
}

int IDA::Search3EC_Inverse(PancakeState& state, int g, int heur, int depth, int parent, int gaps) {
/*std::cout << "puzzle size" << std::endl;
	std::cout << puzzle.size << std::endl;
	std::cout << "state puzzle size" << std::endl;
	std::cout << state.puzzle.size() << std::endl;*/
	assert(puzzle.size == state.puzzle.size());
	
	/*if(counter == 10 && weight > 1.1)
	{
		return -g;
	}*/
	if(puzzle.GoalTest(state))
	{
		//std::cout << "solution" << std::endl;
		/*solution_count++;
		std::cout << solution_count << std::endl;
		if(solution_count < 3)
		{
			return 10000;
		}
		else
		{*/
			return -g;
		//}
	}
	int f = g + heur;
	//std::cout << counter << std::endl;
	/*std::cout << "normal state" << std::endl;
	std::cout << state << std::endl;*/
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << "f" << std::endl;
	//std::cout << f << std::endl;
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	int trans_cost;
	int min = f + 1;
	int temp = 0;
	int h;
	int ggaps = 0;
	bool set = false;
	std::vector<unsigned short int> moves;
	if(f == depth)
	{
		gap_decreasing_moves4(state, moves);
	}
	else
	{
		for(int i = 2; i <= int(puzzle.size); i++)
		{
			moves.push_back(i);
		}
	}
	/*for(int i = 0; i < int(moves.size()); i++)
	{
		std::cout << moves[i] << std::endl;
	}*/
	/*if(f == depth)
	{
		for(int i = 2; i < int(puzzle.size); i++)
		{
			if(abs(state.puzzle[i] - state.puzzle[0]) != 1)
			{
				moves.push_back(i);
			}
		}
		if(state.puzzle[0] = puzzle.size)
		{
			moves.push_back(puzzle.size)
		}
	}
	else
	{
		for(int i = 2; i <= int(puzzle.size); i++)
		{
			moves.push_back(i);
		}
	}*/
	/*bool locked = locked_or_not(state);
	bool one_move = one_move_only(state);*/
	//std::cout << locked << std::endl;
	int i;
	for(/*int i = 2; i <= int(puzzle.size); i++*/int j = 0; j < int(moves.size()); j++)
	{
		i = moves[j];
		if(i == parent)
		{
			continue;
		}
		ggaps = gaps;
		if(i != int(puzzle.size))
		{
		if(int(abs(state.puzzle[i] - state.puzzle[i - 1])) == 1)
		{
			ggaps++;
		}
		if(int(abs(state.puzzle[0] - state.puzzle[i])) == 1)
		{
			ggaps--;
		}
		}
		else
		{
			if(state.puzzle[i - 1] == int(state.puzzle.size()) - 1)
			{
				ggaps++;
			}
			else if(state.puzzle[0] == int(state.puzzle.size()) - 1)
			{
				ggaps--;
			}
		}
		/*std::cout << "i" << std::endl;
		std::cout << i << std::endl;
		std::cout << "h" << std::endl;
		std::cout << ggaps << std::endl;*/
		counter++;
		
		trans_cost = puzzle.GetActionCost(state, i);
		if(ggaps == 0)
		{
			return -(g + trans_cost);
		}
		temp = g + trans_cost + ggaps;
		if(temp > depth)
		{
			if(set == false or temp < min)
			{
				min = temp;
				set = true;
			}
			continue;
		}
		h = ggaps;
		
		puzzle.ApplyAction(state, i);
		action_counter++;
		/*std::cout << "child" << std::endl;
		std::cout << state << std::endl;*/
		/*std::cout << "action" << std::endl;
		std::cout << i << std::endl;
		std::cout << "inverse" << std::endl;*/
		/*for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "gap" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "gapps" << std::endl;
		std::cout << ggaps << std::endl;
		std::cout << "gaapps" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
		
		/*if(h != GapHeur(state))
		{
			std::cout << "state" << std::endl;
			std::cout << state << std::endl;
			std::cout << "flip" << std::endl;
			std::cout << i << std::endl;
			std::cout << h << std::endl;
			std::cout << GapHeur(state) << std::endl;
			exit(0);
		}*/
		/*if(!locked && !one_move && !found && h < heur)
		{
			found = true;
			move = i;
		}*/
		
		/*if((greater_pruning && h > f - g) || (equal_pruning && !locked && h == f - g))
		{
			puzzle.ApplyAction(state, i);
			continue;
		}*/
		temp = Search3EC_Inverse(state, g + trans_cost, h, depth, i, ggaps);
		if(temp < 0)
		{
			path.push_back(i);
			//std::cout << i << std::endl;
			puzzle.ApplyAction(state, i);
			return temp;
		}
		puzzle.ApplyAction(state, i);
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	/*std::cout << "min" << std::endl;
	std::cout << min << std::endl;*/
	return min;
}

int IDA::Search3EC_With_Inverse(PancakeState& state, int g, int heur, int depth, int parent, int gaps) {
/*std::cout << "puzzle size" << std::endl;
	std::cout << puzzle.size << std::endl;
	std::cout << "state puzzle size" << std::endl;
	std::cout << state.puzzle.size() << std::endl;*/
	assert(puzzle.size == state.puzzle.size());
	
	/*if(counter == 10 && weight > 1.1)
	{
		return -g;
	}*/
	if(puzzle.GoalTest(state))
	{
		//std::cout << "solution" << std::endl;
		/*solution_count++;
		std::cout << solution_count << std::endl;
		if(solution_count < 3)
		{
			return 10000;
		}
		else
		{*/
			return -g;
		//}
	}
	int f = g + heur;
	//std::cout << counter << std::endl;
	/*std::cout << "normal state" << std::endl;
	std::cout << state << std::endl;*/
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << "f" << std::endl;
	//std::cout << f << std::endl;
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	int trans_cost;
	int min = f + 1;
	int temp = 0;
	int h;
	int ggaps = 0;
	bool set = false;
	std::vector<unsigned short int> moves;
	if(f == depth)
	{
		if(gap_decreasing_moves(state, 1) == 0)
		{
			gap_decreasing_moves4(state, moves);
		}
	}
	else
	{
		for(int i = 2; i <= int(puzzle.size); i++)
		{
			moves.push_back(i);
		}
	}
	/*for(int i = 0; i < int(moves.size()); i++)
	{
		std::cout << moves[i] << std::endl;
	}*/
	/*if(f == depth)
	{
		for(int i = 2; i < int(puzzle.size); i++)
		{
			if(abs(state.puzzle[i] - state.puzzle[0]) != 1)
			{
				moves.push_back(i);
			}
		}
		if(state.puzzle[0] = puzzle.size)
		{
			moves.push_back(puzzle.size)
		}
	}
	else
	{
		for(int i = 2; i <= int(puzzle.size); i++)
		{
			moves.push_back(i);
		}
	}*/
	/*bool locked = locked_or_not(state);
	bool one_move = one_move_only(state);*/
	//std::cout << locked << std::endl;
	int i;
	
	for(/*int i = 2; i <= int(puzzle.size); i++*/int j = 0; j < int(moves.size()); j++)
	{
		i = moves[j];
		if(i == parent)
		{
			continue;
		}
		ggaps = gaps;
		if(i != int(puzzle.size))
		{
		if(int(abs(state.puzzle[i] - state.puzzle[i - 1])) == 1)
		{
			ggaps++;
		}
		if(int(abs(state.puzzle[0] - state.puzzle[i])) == 1)
		{
			ggaps--;
		}
		}
		else
		{
			if(state.puzzle[i - 1] == int(state.puzzle.size()) - 1)
			{
				ggaps++;
			}
			else if(state.puzzle[0] == int(state.puzzle.size()) - 1)
			{
				ggaps--;
			}
		}
		/*std::cout << "i" << std::endl;
		std::cout << i << std::endl;
		std::cout << "h" << std::endl;
		std::cout << ggaps << std::endl;*/
		counter++;
		
		trans_cost = puzzle.GetActionCost(state, i);
		if(ggaps == 0)
		{
			return -(g + trans_cost);
		}
		temp = g + trans_cost + ggaps;
		if(temp > depth)
		{
			if(set == false or temp < min)
			{
				min = temp;
				set = true;
			}
			continue;
		}
		
		
		
		
		
		
		h = ggaps;
		
		
		
		
		
		puzzle.ApplyAction(state, i);
		action_counter++;
		/*std::cout << "child" << std::endl;
		std::cout << state << std::endl;*/
		/*std::cout << "action" << std::endl;
		std::cout << i << std::endl;
		std::cout << "inverse" << std::endl;*/
		/*for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "gap" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "gapps" << std::endl;
		std::cout << ggaps << std::endl;
		std::cout << "gaapps" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
		
		/*if(h != GapHeur(state))
		{
			std::cout << "state" << std::endl;
			std::cout << state << std::endl;
			std::cout << "flip" << std::endl;
			std::cout << i << std::endl;
			std::cout << h << std::endl;
			std::cout << GapHeur(state) << std::endl;
			exit(0);
		}*/
		/*if(!locked && !one_move && !found && h < heur)
		{
			found = true;
			move = i;
		}*/
		
		/*if((greater_pruning && h > f - g) || (equal_pruning && !locked && h == f - g))
		{
			puzzle.ApplyAction(state, i);
			continue;
		}*/
		temp = Search3EC_With_Inverse(state, g + trans_cost, h, depth, i, ggaps);
		if(temp < 0)
		{
			path.push_back(i);
			//std::cout << i << std::endl;
			puzzle.ApplyAction(state, i);
			return temp;
		}
		puzzle.ApplyAction(state, i);
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	/*std::cout << "min" << std::endl;
	std::cout << min << std::endl;*/
	return min;
}

int IDA::Search3_Early(PancakeState& state, int g, int heur, int depth, int parent, int gaps, clock_t &t1) {
	/*std::cout << "puzzle size" << std::endl;
	std::cout << puzzle.size << std::endl;
	std::cout << "state puzzle size" << std::endl;
	std::cout << state.puzzle.size() << std::endl;*/
	assert(puzzle.size == state.puzzle.size());
	
	/*if(counter == 10 && weight > 1.1)
	{
		return -g;
	}*/
	if(puzzle.GoalTest(state))
	{
		//std::cout << "solution" << std::endl;
		/*solution_count++;
		std::cout << solution_count << std::endl;
		if(solution_count < 3)
		{
			return 10000;
		}
		else
		{*/
			return -g;
		//}
	}
	int f = g + heur;
	//std::cout << counter << std::endl;
	/*std::cout << "normal state" << std::endl;
	std::cout << state << std::endl;*/
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << "f" << std::endl;
	//std::cout << f << std::endl;
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	clock_t t2;
	t2=clock();
	double diff1;
    	diff1 = ((double)(t2-t1)) / CLOCKS_PER_SEC;
    	if(diff1 > 1800)
    	{
    		return -100;
    	}
	int trans_cost;
	int min = f + 1;
	int temp = 0;
	int h;
	int ggaps = 0;
	bool set = false;
	/*std::vector<unsigned short int> moves;
	if(f == depth)
	{
		gap_decreasing_moves4(state, moves);
	}
	else
	{
		for(int i = 2; i <= int(puzzle.size); i++)
		{
			moves.push_back(i);
		}
	}*/
	/*for(int i = 0; i < int(moves.size()); i++)
	{
		std::cout << moves[i] << std::endl;
	}*/
	/*if(f == depth)
	{
		for(int i = 2; i < int(puzzle.size); i++)
		{
			if(abs(state.puzzle[i] - state.puzzle[0]) != 1)
			{
				moves.push_back(i);
			}
		}
		if(state.puzzle[0] = puzzle.size)
		{
			moves.push_back(puzzle.size)
		}
	}
	else
	{
		for(int i = 2; i <= int(puzzle.size); i++)
		{
			moves.push_back(i);
		}
	}*/
	/*bool locked = locked_or_not(state);
	bool one_move = one_move_only(state);*/
	//std::cout << locked << std::endl;
	//int i;
	for(int i = 2; i <= int(puzzle.size); i++/*int j = 0; j < int(moves.size()); j++*/)
	{
		//i = moves[j];
		if(i == parent)
		{
			continue;
		}
		ggaps = gaps;
		if(i != int(puzzle.size))
		{
		if(int(abs(state.puzzle[i] - state.puzzle[i - 1])) == 1)
		{
			ggaps++;
		}
		if(int(abs(state.puzzle[0] - state.puzzle[i])) == 1)
		{
			ggaps--;
		}
		}
		else
		{
			if(state.puzzle[i - 1] == int(state.puzzle.size()) - 1)
			{
				ggaps++;
			}
			else if(state.puzzle[0] == int(state.puzzle.size()) - 1)
			{
				ggaps--;
			}
		}
		/*std::cout << "i" << std::endl;
		std::cout << i << std::endl;
		std::cout << "h" << std::endl;
		std::cout << ggaps << std::endl;*/
		counter++;
		
		trans_cost = puzzle.GetActionCost(state, i);
		if(ggaps == 0)
		{
			return -(g + trans_cost);
		}
		temp = g + trans_cost + ggaps;
		if(temp > depth)
		{
			if(set == false or temp < min)
			{
				min = temp;
				set = true;
			}
			continue;
		}
		h = ggaps;
		
		puzzle.ApplyAction2(state, i);
		action_counter++;
		/*std::cout << "child" << std::endl;
		std::cout << state << std::endl;*/
		/*std::cout << "action" << std::endl;
		std::cout << i << std::endl;
		std::cout << "inverse" << std::endl;*/
		/*for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "gap" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "gapps" << std::endl;
		std::cout << ggaps << std::endl;
		std::cout << "gaapps" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
		
		/*if(h != GapHeur(state))
		{
			std::cout << "state" << std::endl;
			std::cout << state << std::endl;
			std::cout << "flip" << std::endl;
			std::cout << i << std::endl;
			std::cout << h << std::endl;
			std::cout << GapHeur(state) << std::endl;
			exit(0);
		}*/
		/*if(!locked && !one_move && !found && h < heur)
		{
			found = true;
			move = i;
		}*/
		
		/*if((greater_pruning && h > f - g) || (equal_pruning && !locked && h == f - g))
		{
			puzzle.ApplyAction(state, i);
			continue;
		}*/
		temp = Search3_Early(state, g + trans_cost, h, depth, i, ggaps, t1);
		if(temp < 0)
		{
			path.push_back(i);
			//std::cout << i << std::endl;
			puzzle.ApplyAction2(state, i);
			return temp;
		}
		puzzle.ApplyAction2(state, i);
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	/*std::cout << "min" << std::endl;
	std::cout << min << std::endl;*/
	return min;
}

int IDA::Search_Big(PancakeState& state, int g, int heur, int depth, int parent) {
	/*std::cout << "puzzle size" << std::endl;
	std::cout << puzzle.size << std::endl;
	std::cout << "state puzzle size" << std::endl;
	std::cout << state.puzzle.size() << std::endl;*/
	assert(puzzle.size == state.puzzle.size());
	
	/*if(counter == 10 && weight > 1.1)
	{
		return -g;
	}*/
	if(puzzle.GoalTest(state))
	{
		//std::cout << "solution" << std::endl;
		return -g;
	}
	int f = g + heur;
	//std::cout << counter << std::endl;
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << "f" << std::endl;
	//std::cout << f << std::endl;
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	int trans_cost;
	int min = -1;
	int temp = 0;
	int h;
	bool set = false;
	bool locked = locked_or_not(state);
	bool one_move = one_move_only(state);
	//std::cout << "state" << std::endl;
	//std::cout << state << std::endl;
	//std::cout << locked << std::endl;
	for(int i = 2; i <= int(puzzle.size); i++)
	{
		if(i == parent)
		{
			continue;
		}
		if(state.puzzle[0] == int(state.puzzle.size()) - 1 && i != int(state.puzzle.size()))
		{
			continue;
		}
		trans_cost = puzzle.GetActionCost(state, i);
		puzzle.ApplyAction(state, i);
		/*std::cout << "gap" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		h = GapHeur(state);
		if(!locked && !one_move && !found && h < heur)
		{
			found = true;
			move = i;
		}
		counter++;
		if((greater_pruning /*&& !locked*/ && h > f - g) || (equal_pruning && !locked && h == f - g))
		{
			puzzle.ApplyAction(state, i);
			continue;
		}
		temp = Search_Big(state, g + trans_cost, h, depth, i);
		if(temp < 0)
		{
			path.push_back(i);
			puzzle.ApplyAction(state, i);
			return temp;
		}
		puzzle.ApplyAction(state, i);
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	return min;
}

int IDA::Search_Top(PancakeState& state, int g, int heur, int depth, int parent) {
	/*std::cout << "puzzle size" << std::endl;
	std::cout << puzzle.size << std::endl;
	std::cout << "state puzzle size" << std::endl;
	std::cout << state.puzzle.size() << std::endl;*/
	assert(puzzle.size == state.puzzle.size());
	
	/*if(counter == 10 && weight > 1.1)
	{
		return -g;
	}*/
	if(puzzle.GoalTest(state))
	{
		//std::cout << "solution" << std::endl;
		return -g;
	}
	int f = g + heur;
	//std::cout << counter << std::endl;
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << "f" << std::endl;
	//std::cout << f << std::endl;
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	int trans_cost;
	int min = -1;
	int temp = 0;
	int h;
	bool set = false;
	bool locked = locked_or_not(state);
	bool one_move = one_move_only(state);
	//std::cout << "state" << std::endl;
	//std::cout << state << std::endl;
	//std::cout << locked << std::endl;
	for(int i = 2; i <= int(puzzle.size); i++)
	{
		if(i == parent)
		{
			continue;
		}
		trans_cost = puzzle.GetActionCost(state, i);
		puzzle.ApplyAction(state, i);
		/*std::cout << "gap" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		h = TopHeur(state, GapHeur(state));
		if(!locked && !one_move && !found && h < heur)
		{
			found = true;
			move = i;
		}
		counter++;
		if((greater_pruning /*&& !locked*/ && h > f - g) || (equal_pruning && !locked && h == f - g))
		{
			puzzle.ApplyAction(state, i);
			continue;
		}
		temp = Search_Top(state, g + trans_cost, h, depth, i);
		if(temp < 0)
		{
			path.push_back(i);
			puzzle.ApplyAction(state, i);
			return temp;
		}
		puzzle.ApplyAction(state, i);
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	return min;
}

int IDA::Search_Ordered(PancakeState& state, int g, int heur, int depth, int parent, int iterations) {
	/*std::cout << "puzzle size" << std::endl;
	std::cout << puzzle.size << std::endl;
	std::cout << "state puzzle size" << std::endl;
	std::cout << state.puzzle.size() << std::endl;*/
	assert(puzzle.size == state.puzzle.size());
	
	/*if(counter == 10 && weight > 1.1)
	{
		return -g;
	}*/
	if(puzzle.GoalTest(state))
	{
		//std::cout << "solution" << std::endl;
		return -g;
	}
	int f = g + heur;
	//std::cout << counter << std::endl;
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << "f" << std::endl;
	//std::cout << f << std::endl;
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	int trans_cost;
	int min = -1;
	int temp = 0;
	int h;
	bool set = false;
	bool locked = locked_or_not(state);
	bool one_move = one_move_only(state);
	std::vector<unsigned short int> moves;
	std::vector<unsigned short int> gap_moves;
	top_decreasing_moves(state, gap_moves);
	std::vector<unsigned short int> gap_moves2;
	gap_creating_moves(state, gap_moves2);
	/*std::cout << "iterations" << std::endl;
	std::cout << iterations << std::endl;*/
	//std::cout << locked << std::endl;
	//std::cout << iterations << std::endl;
	/*if(iterations == 0)
	{
		for(unsigned i = 0; i < gap_moves.size(); i++)
		{
			moves.push_back(gap_moves[i]);
		}
		if(moves.size() == 0)
		{
			return f + 1;
		}
	}*/
	/*else
	{*/
	if(iterations > 0)
	{
		for(unsigned i = 2; i <= state.puzzle.size(); i++)
		{
			if(std::find(gap_moves.begin(), gap_moves.end(), i) == gap_moves.end())
			{
				moves.push_back(i);
			}
		}
		for(unsigned i = 0; i < gap_moves.size(); i++)
		{
			moves.push_back(gap_moves[i]);
		}
	}
	else
	{
		for(unsigned i = 2; i <= state.puzzle.size(); i++)
		{
			moves.push_back(i);
		}
	}
	//}
	/*else
	{
		for(unsigned i = 2; i <= state.puzzle.size(); i++)
		{
			if(std::find(gap_moves.begin(), gap_moves.end(), i) == gap_moves.end())
			{
				moves.push_back(i);
			}
		}
		for(unsigned i = 0; i < gap_moves.size(); i++)
		{
			moves.push_back(gap_moves[i]);
		}
	}*/
	/*std::cout << "moves" << std::endl;
	for(unsigned k = 0; k < moves.size(); k++)
    	{
    		std::cout << moves[k] << ' ';
    	}
    	std::cout << std::endl;
    	std::cout << "parent" << std::endl;
    	std::cout << parent << std::endl;*/
	for(unsigned i = 0; i < moves.size(); i++)
	{
		/*std::cout << "move" << std::endl;
    		std::cout << moves[i] << std::endl;*/
		if(moves[i] == parent)
		{
			if(moves.size() == 1)
			{
				return f + 1;
			}
			continue;
		}
		trans_cost = puzzle.GetActionCost(state, moves[i]);
		puzzle.ApplyAction(state, moves[i]);
		/*std::cout << "gap" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		h = TopHeur(state, GapHeur(state));
		if(!locked && !one_move && !found && h < heur)
		{
			found = true;
			move = moves[i];
		}
		counter++;
		if((greater_pruning /*&& !locked*/ && h > f - g) || (equal_pruning && !locked && h == f - g))
		{
			puzzle.ApplyAction(state, moves[i]);
			continue;
		}
		temp = Search_Ordered(state, g + trans_cost, h, depth, moves[i], iterations - (h - heur + 1));
		if(temp < 0)
		{
			path.push_back(moves[i]);
			puzzle.ApplyAction(state, moves[i]);
			if(heur == h)
			{
				index = path.size();
				flip = moves[i];
			}
			return temp;
		}
		puzzle.ApplyAction(state, moves[i]);
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	return min;
}

int IDA::Search_Inverse(PancakeState& state, int g, int heur, int depth, int parent, int gaps) {
	/*std::cout << "puzzle size" << std::endl;
	std::cout << puzzle.size << std::endl;
	std::cout << "state puzzle size" << std::endl;
	std::cout << state.puzzle.size() << std::endl;*/
	assert(puzzle.size == state.puzzle.size());
	
	/*if(counter == 10 && weight > 1.1)
	{
		return -g;
	}*/
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
	//std::cout << counter << std::endl;
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << "f" << std::endl;
	//std::cout << f << std::endl;
	/*std::cout << "top inverse" << std::endl;
    	std::cout << heur << std::endl;*/
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	int trans_cost;
	int min = -1;
	int temp = 0;
	int ggaps = 0;
	int h;
	bool set = false;
	//bool locked = locked_or_not2(state);
	//bool one_move = one_move_only(state);
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	//std::cout << locked << std::endl;
	//IMPLEMENT THE BINARY TREE THING IN THE FIRST ITERATION
	for(int i = 2; i <= int(puzzle.size); i++)
	{
		ggaps = gaps;
		if(i == parent)
		{
			continue;
		}
		trans_cost = puzzle.GetActionCost(state, i);
		//put gap code here and compare
		puzzle.ApplyAction(state, i);
		/*std::cout << "child" << std::endl;
		std::cout << state << std::endl;*/
		/*std::cout << "action" << std::endl;
		std::cout << i << std::endl;
		std::cout << "inverse" << std::endl;*/
		/*for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "gap" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		if(i != int(puzzle.size))
		{
		if(int(abs(state.puzzle[0] - state.puzzle[i])) == 1)
		{
			ggaps++;
		}
		if(int(abs(state.puzzle[i - 1] - state.puzzle[i])) == 1)
		{
			ggaps--;
		}
		}
		else
		{
			if(state.puzzle[i - 1] == int(state.puzzle.size()) - 1)
			{
				ggaps--;
			}
			else if(state.puzzle[0] == int(state.puzzle.size()) - 1)
			{
				ggaps++;
			}
		}
		/*std::cout << "gapps" << std::endl;
		std::cout << ggaps << std::endl;
		std::cout << "gaapps" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverseAndL(state, ggaps) << std::endl;*/
		h = ggaps + TopHeurWithInverseAndL(state, ggaps);
		/*if(h != GapHeur(state))
		{
			std::cout << "state" << std::endl;
			std::cout << state << std::endl;
			std::cout << "flip" << std::endl;
			std::cout << i << std::endl;
			std::cout << h << std::endl;
			std::cout << GapHeur(state) << std::endl;
			exit(0);
		}*/
		/*if(!locked && !one_move && !found && h < heur)
		{
			found = true;
			move = i;
		}*/
		counter++;
		/*if((greater_pruning && h > f - g) || (equal_pruning && !locked && h == f - g))
		{
			puzzle.ApplyAction(state, i);
			continue;
		}*/
		temp = Search_Inverse(state, g + trans_cost, h, depth, i, ggaps);
		if(temp < 0)
		{
			path.push_back(i);
			//std::cout << i << std::endl;
			puzzle.ApplyAction(state, i);
			return temp;
		}
		puzzle.ApplyAction(state, i);
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	return min;
}

int IDA::Search_Inverse_EC(PancakeState& state, int g, int heur, int depth, int parent, int gaps, clock_t &t1) {
	/*std::cout << "puzzle size" << std::endl;
	std::cout << puzzle.size << std::endl;
	std::cout << "state puzzle size" << std::endl;
	std::cout << state.puzzle.size() << std::endl;*/
	assert(puzzle.size == state.puzzle.size());
	
	/*if(counter == 10 && weight > 1.1)
	{
		return -g;
	}*/
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
	//std::cout << counter << std::endl;
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << "f" << std::endl;
	//std::cout << f << std::endl;
	/*std::cout << "top inverse" << std::endl;
    	std::cout << heur << std::endl;*/
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	/*clock_t t2;
	t2=clock();
	double diff1;
    	diff1 = ((double)(t2-t1)) / CLOCKS_PER_SEC;
    	if(diff1 > 1800)
    	{
    		return -100;
    	}*/
	int trans_cost;
	int min = -1;
	int temp = 0;
	int ggaps = 0;
	int h;
	bool set = false;
	//bool locked = locked_or_not2(state);
	//bool one_move = one_move_only(state);
	/*std::cout << "before state" << std::endl;
	std::cout << state << std::endl;
	for(unsigned i = 0; i < state.inverse.size(); i++)
	{
		std::cout << state.inverse[i] << ' ';
	}
	std::cout << std::endl;*/
	//std::cout << locked << std::endl;
	//IMPLEMENT THE BINARY TREE THING IN THE FIRST ITERATION
	for(int i = 2; i <= int(puzzle.size); i++)
	{
		if(i == parent)
		{
			continue;
		}
		ggaps = gaps;
		if(i != int(puzzle.size))
		{
		if(int(abs(state.puzzle[i] - state.puzzle[i - 1])) == 1)
		{
			ggaps++;
		}
		if(int(abs(state.puzzle[0] - state.puzzle[i])) == 1)
		{
			ggaps--;
		}
		}
		else
		{
			if(state.puzzle[i - 1] == int(state.puzzle.size()) - 1)
			{
				ggaps++;
			}
			else if(state.puzzle[0] == int(state.puzzle.size()) - 1)
			{
				ggaps--;
			}
		}
		/*std::cout << "i" << std::endl;
		std::cout << i << std::endl;*/
		
		/*std::cout << "h" << std::endl;
		std::cout << ggaps << std::endl;*/
		counter++;
		
		trans_cost = puzzle.GetActionCost(state, i);
		if(ggaps == 0)
		{
			return -(g + trans_cost);
		}
		h = ggaps;
		temp = g + trans_cost + ggaps;
		if(temp > depth)
		{
			if(set == false or temp < min)
			{
				min = temp;
				set = true;
			}
			continue;
		}
		//std::cout << "hehehehehehe" << std::endl;
		
		h = h + TopHeurWithInverseAndLBF(state, ggaps, i);
		/*std::cout << "before state" << std::endl;
		std::cout << state << std::endl;
		puzzle.ApplyAction(state, i);
		std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		if(h - ggaps != TopHeurWithInverseAndL(state, ggaps))
		{
			std::cout << "TopHeurWithInverseAndLBF!!!!!!!!!!!!!!!!!!!!" << std::endl;
			std::cout << h - ggaps << std::endl;
		}
		puzzle.ApplyAction(state, i);*/
		
		
		
		top_counter++;
		
		temp = g + trans_cost + h;
		/*std::cout << "temp" << std::endl;
		std::cout << temp << std::endl;*/
		if(temp > depth)
		{
			if(set == false or temp < min)
			{
				min = temp;
				set = true;
			}
			continue;
		}
		
		puzzle.ApplyAction(state, i);
		
		action_counter++;
		/*std::cout << "child" << std::endl;
		std::cout << state << std::endl;*/
		/*std::cout << "action" << std::endl;
		std::cout << i << std::endl;
		std::cout << "inverse" << std::endl;*/
		/*for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "gap" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "gapps" << std::endl;
		std::cout << ggaps << std::endl;
		std::cout << "gaapps" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
		
		/*if(h != GapHeur(state))
		{
			std::cout << "state" << std::endl;
			std::cout << state << std::endl;
			std::cout << "flip" << std::endl;
			std::cout << i << std::endl;
			std::cout << h << std::endl;
			std::cout << GapHeur(state) << std::endl;
			exit(0);
		}*/
		/*if(!locked && !one_move && !found && h < heur)
		{
			found = true;
			move = i;
		}*/
		
		/*if((greater_pruning && h > f - g) || (equal_pruning && !locked && h == f - g))
		{
			puzzle.ApplyAction(state, i);
			continue;
		}*/
		temp = Search_Inverse_EC(state, g + trans_cost, h, depth, i, ggaps, t1);
		if(temp < 0)
		{
			path.push_back(i);
			//std::cout << i << std::endl;
			puzzle.ApplyAction(state, i);
			return temp;
		}
		puzzle.ApplyAction(state, i);
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	return min;
}

int IDA::Search_Inverse_FG_EC(PancakeState& state, int g, int heur, int depth, int parent, int gaps, clock_t &t1) {
	/*std::cout << "puzzle size" << std::endl;
	std::cout << puzzle.size << std::endl;
	std::cout << "state puzzle size" << std::endl;
	std::cout << state.puzzle.size() << std::endl;*/
	assert(puzzle.size == state.puzzle.size());
	
	/*if(counter == 10 && weight > 1.1)
	{
		return -g;
	}*/
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
	//std::cout << counter << std::endl;
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << "f" << std::endl;
	//std::cout << f << std::endl;
	/*std::cout << "top inverse" << std::endl;
    	std::cout << heur << std::endl;*/
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	/*clock_t t2;
	t2=clock();
	double diff1;
    	diff1 = ((double)(t2-t1)) / CLOCKS_PER_SEC;
    	if(diff1 > 1800)
    	{
    		return -100;
    	}*/
	int trans_cost;
	int min = -1;
	int temp = 0;
	int ggaps = 0;
	int h;
	bool set = false;
	//bool locked = locked_or_not2(state);
	//bool one_move = one_move_only(state);
	/*std::cout << "before state" << std::endl;
	std::cout << state << std::endl;
	for(unsigned i = 0; i < state.inverse.size(); i++)
	{
		std::cout << state.inverse[i] << ' ';
	}
	std::cout << std::endl;*/
	//std::cout << locked << std::endl;
	//IMPLEMENT THE BINARY TREE THING IN THE FIRST ITERATION
	for(int i = 2; i <= int(puzzle.size); i++)
	{
		if(i == parent)
		{
			continue;
		}
		ggaps = gaps;
		if(i != int(puzzle.size))
		{
		if(int(abs(state.puzzle[i] - state.puzzle[i - 1])) == 1)
		{
			ggaps++;
		}
		if(int(abs(state.puzzle[0] - state.puzzle[i])) == 1)
		{
			ggaps--;
		}
		}
		else
		{
			if(state.puzzle[i - 1] == int(state.puzzle.size()) - 1)
			{
				ggaps++;
			}
			else if(state.puzzle[0] == int(state.puzzle.size()) - 1)
			{
				ggaps--;
			}
		}
		/*std::cout << "i" << std::endl;
		std::cout << i << std::endl;*/
		
		/*std::cout << "h" << std::endl;
		std::cout << ggaps << std::endl;*/
		counter++;
		
		trans_cost = puzzle.GetActionCost(state, i);
		if(ggaps == 0)
		{
			return -(g + trans_cost);
		}
		h = ggaps;
		temp = g + trans_cost + ggaps;
		if(temp > depth)
		{
			if(set == false or temp < min)
			{
				min = temp;
				set = true;
			}
			continue;
		}
		//std::cout << "hehehehehehe" << std::endl;
		
		h = h + TopHeurWithInverseAndLFG_bf(state, ggaps, i);
		/*std::cout << "before state" << std::endl;
		std::cout << state << std::endl;
		puzzle.ApplyAction(state, i);
		std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		if(h - ggaps != TopHeurWithInverseAndL(state, ggaps))
		{
			std::cout << "TopHeurWithInverseAndLBF!!!!!!!!!!!!!!!!!!!!" << std::endl;
			std::cout << h - ggaps << std::endl;
		}
		puzzle.ApplyAction(state, i);*/
		
		
		
		top_counter++;
		
		temp = g + trans_cost + h;
		/*std::cout << "temp" << std::endl;
		std::cout << temp << std::endl;*/
		if(temp > depth)
		{
			if(set == false or temp < min)
			{
				min = temp;
				set = true;
			}
			continue;
		}
		
		puzzle.ApplyAction(state, i);
		
		action_counter++;
		/*std::cout << "child" << std::endl;
		std::cout << state << std::endl;*/
		/*std::cout << "action" << std::endl;
		std::cout << i << std::endl;
		std::cout << "inverse" << std::endl;*/
		/*for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "gap" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "gapps" << std::endl;
		std::cout << ggaps << std::endl;
		std::cout << "gaapps" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
		
		/*if(h != GapHeur(state))
		{
			std::cout << "state" << std::endl;
			std::cout << state << std::endl;
			std::cout << "flip" << std::endl;
			std::cout << i << std::endl;
			std::cout << h << std::endl;
			std::cout << GapHeur(state) << std::endl;
			exit(0);
		}*/
		/*if(!locked && !one_move && !found && h < heur)
		{
			found = true;
			move = i;
		}*/
		
		/*if((greater_pruning && h > f - g) || (equal_pruning && !locked && h == f - g))
		{
			puzzle.ApplyAction(state, i);
			continue;
		}*/
		temp = Search_Inverse_FG_EC(state, g + trans_cost, h, depth, i, ggaps, t1);
		if(temp < 0)
		{
			path.push_back(i);
			//std::cout << i << std::endl;
			puzzle.ApplyAction(state, i);
			return temp;
		}
		puzzle.ApplyAction(state, i);
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	return min;
}

int IDA::Search_Top_No_Inverse_EC(PancakeState& state, int g, int heur, int depth, int parent, int gaps, clock_t &t1) {
	/*std::cout << "puzzle size" << std::endl;
	std::cout << puzzle.size << std::endl;
	std::cout << "state puzzle size" << std::endl;
	std::cout << state.puzzle.size() << std::endl;*/
	assert(puzzle.size == state.puzzle.size());
	
	/*if(counter == 10 && weight > 1.1)
	{
		return -g;
	}*/
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
	//std::cout << counter << std::endl;
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << "f" << std::endl;
	//std::cout << f << std::endl;
	/*std::cout << "top inverse" << std::endl;
    	std::cout << heur << std::endl;*/
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	clock_t t2;
	t2=clock();
	double diff1;
    	diff1 = ((double)(t2-t1)) / CLOCKS_PER_SEC;
    	if(diff1 > 1800)
    	{
    		return -100;
    	}
	int trans_cost;
	int min = -1;
	int temp = 0;
	int ggaps = 0;
	int h;
	bool set = false;
	//bool locked = locked_or_not2(state);
	//bool one_move = one_move_only(state);
	/*std::cout << "before state" << std::endl;
	std::cout << state << std::endl;*/
	//std::cout << locked << std::endl;
	//IMPLEMENT THE BINARY TREE THING IN THE FIRST ITERATION
	for(int i = 2; i <= int(puzzle.size); i++)
	{
		if(i == parent)
		{
			continue;
		}
		ggaps = gaps;
		if(i != int(puzzle.size))
		{
		if(int(abs(state.puzzle[i] - state.puzzle[i - 1])) == 1)
		{
			ggaps++;
		}
		if(int(abs(state.puzzle[0] - state.puzzle[i])) == 1)
		{
			ggaps--;
		}
		}
		else
		{
			if(state.puzzle[i - 1] == int(state.puzzle.size()) - 1)
			{
				ggaps++;
			}
			else if(state.puzzle[0] == int(state.puzzle.size()) - 1)
			{
				ggaps--;
			}
		}
		/*std::cout << "i" << std::endl;
		std::cout << i << std::endl;*/
		
		/*std::cout << "h" << std::endl;
		std::cout << ggaps << std::endl;*/
		counter++;
		
		trans_cost = puzzle.GetActionCost(state, i);
		if(ggaps == 0)
		{
			return -(g + trans_cost);
		}
		h = ggaps;
		temp = g + trans_cost + ggaps;
		if(temp > depth)
		{
			if(set == false or temp < min)
			{
				min = temp;
				set = true;
			}
			continue;
		}
		//std::cout << "hehehehehehe" << std::endl;
		
		h = h + gap_decreasing_moves_no_inv_flip(state, i);
		/*std::cout << "before state" << std::endl;
		std::cout << state << std::endl;
		puzzle.ApplyAction(state, i);
		std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		if(h - ggaps != TopHeurWithInverseAndL(state, ggaps))
		{
			std::cout << "TopHeurWithInverseAndLBF!!!!!!!!!!!!!!!!!!!!" << std::endl;
			std::cout << h - ggaps << std::endl;
		}
		puzzle.ApplyAction(state, i);*/
		top_counter++;
		
		temp = g + trans_cost + h;
		/*std::cout << "temp" << std::endl;
		std::cout << temp << std::endl;*/
		if(temp > depth)
		{
			if(set == false or temp < min)
			{
				min = temp;
				set = true;
			}
			continue;
		}
		
		puzzle.ApplyAction2(state, i);
		
		action_counter++;
		/*std::cout << "child" << std::endl;
		std::cout << state << std::endl;*/
		/*std::cout << "action" << std::endl;
		std::cout << i << std::endl;
		std::cout << "inverse" << std::endl;*/
		/*for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "gap" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "gapps" << std::endl;
		std::cout << ggaps << std::endl;
		std::cout << "gaapps" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
		
		/*if(h != GapHeur(state))
		{
			std::cout << "state" << std::endl;
			std::cout << state << std::endl;
			std::cout << "flip" << std::endl;
			std::cout << i << std::endl;
			std::cout << h << std::endl;
			std::cout << GapHeur(state) << std::endl;
			exit(0);
		}*/
		/*if(!locked && !one_move && !found && h < heur)
		{
			found = true;
			move = i;
		}*/
		
		/*if((greater_pruning && h > f - g) || (equal_pruning && !locked && h == f - g))
		{
			puzzle.ApplyAction(state, i);
			continue;
		}*/
		temp = Search_Top_No_Inverse_EC(state, g + trans_cost, h, depth, i, ggaps, t1);
		if(temp < 0)
		{
			path.push_back(i);
			//std::cout << i << std::endl;
			puzzle.ApplyAction2(state, i);
			return temp;
		}
		puzzle.ApplyAction2(state, i);
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	return min;
}

int IDA::Search_Top_With_Inverse_EC(PancakeState& state, int g, int heur, int depth, int parent, int gaps, clock_t &t1) {
	/*std::cout << "puzzle size" << std::endl;
	std::cout << puzzle.size << std::endl;
	std::cout << "state puzzle size" << std::endl;
	std::cout << state.puzzle.size() << std::endl;*/
	assert(puzzle.size == state.puzzle.size());
	
	/*if(counter == 10 && weight > 1.1)
	{
		return -g;
	}*/
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
	//std::cout << counter << std::endl;
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << "f" << std::endl;
	//std::cout << f << std::endl;
	/*std::cout << "top inverse" << std::endl;
    	std::cout << heur << std::endl;*/
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	clock_t t2;
	t2=clock();
	double diff1;
    	diff1 = ((double)(t2-t1)) / CLOCKS_PER_SEC;
    	if(diff1 > 1800)
    	{
    		return -100;
    	}
	int trans_cost;
	int min = -1;
	int temp = 0;
	int ggaps = 0;
	int h;
	bool set = false;
	//bool locked = locked_or_not2(state);
	//bool one_move = one_move_only(state);
	/*std::cout << "before state" << std::endl;
	std::cout << state << std::endl;*/
	//std::cout << locked << std::endl;
	//IMPLEMENT THE BINARY TREE THING IN THE FIRST ITERATION
	for(int i = 2; i <= int(puzzle.size); i++)
	{
		if(i == parent)
		{
			continue;
		}
		ggaps = gaps;
		if(i != int(puzzle.size))
		{
		if(int(abs(state.puzzle[i] - state.puzzle[i - 1])) == 1)
		{
			ggaps++;
		}
		if(int(abs(state.puzzle[0] - state.puzzle[i])) == 1)
		{
			ggaps--;
		}
		}
		else
		{
			if(state.puzzle[i - 1] == int(state.puzzle.size()) - 1)
			{
				ggaps++;
			}
			else if(state.puzzle[0] == int(state.puzzle.size()) - 1)
			{
				ggaps--;
			}
		}
		/*std::cout << "i" << std::endl;
		std::cout << i << std::endl;*/
		
		/*std::cout << "h" << std::endl;
		std::cout << ggaps << std::endl;*/
		counter++;
		
		trans_cost = puzzle.GetActionCost(state, i);
		if(ggaps == 0)
		{
			return -(g + trans_cost);
		}
		h = ggaps;
		temp = g + trans_cost + ggaps;
		if(temp > depth)
		{
			if(set == false or temp < min)
			{
				min = temp;
				set = true;
			}
			continue;
		}
		//std::cout << "hehehehehehe" << std::endl;
		
		h = h + std::max(gap_decreasing_moves2(state, i, 0), gap_decreasing_moves2(state, i, 1));
		/*std::cout << "before state" << std::endl;
		std::cout << state << std::endl;
		puzzle.ApplyAction(state, i);
		std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		if(h - ggaps != TopHeurWithInverseAndL(state, ggaps))
		{
			std::cout << "TopHeurWithInverseAndLBF!!!!!!!!!!!!!!!!!!!!" << std::endl;
			std::cout << h - ggaps << std::endl;
		}
		puzzle.ApplyAction(state, i);*/
		top_counter++;
		
		temp = g + trans_cost + h;
		/*std::cout << "temp" << std::endl;
		std::cout << temp << std::endl;*/
		if(temp > depth)
		{
			if(set == false or temp < min)
			{
				min = temp;
				set = true;
			}
			continue;
		}
		
		puzzle.ApplyAction(state, i);
		
		action_counter++;
		/*std::cout << "child" << std::endl;
		std::cout << state << std::endl;*/
		/*std::cout << "action" << std::endl;
		std::cout << i << std::endl;
		std::cout << "inverse" << std::endl;*/
		/*for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "gap" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "gapps" << std::endl;
		std::cout << ggaps << std::endl;
		std::cout << "gaapps" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
		
		/*if(h != GapHeur(state))
		{
			std::cout << "state" << std::endl;
			std::cout << state << std::endl;
			std::cout << "flip" << std::endl;
			std::cout << i << std::endl;
			std::cout << h << std::endl;
			std::cout << GapHeur(state) << std::endl;
			exit(0);
		}*/
		/*if(!locked && !one_move && !found && h < heur)
		{
			found = true;
			move = i;
		}*/
		
		/*if((greater_pruning && h > f - g) || (equal_pruning && !locked && h == f - g))
		{
			puzzle.ApplyAction(state, i);
			continue;
		}*/
		temp = Search_Top_With_Inverse_EC(state, g + trans_cost, h, depth, i, ggaps, t1);
		if(temp < 0)
		{
			path.push_back(i);
			//std::cout << i << std::endl;
			puzzle.ApplyAction(state, i);
			return temp;
		}
		puzzle.ApplyAction(state, i);
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	return min;
}

int IDA::Search_L_No_Inverse_EC(PancakeState& state, int g, int heur, int depth, int parent, int gaps, clock_t &t1) {
	/*std::cout << "puzzle size" << std::endl;
	std::cout << puzzle.size << std::endl;
	std::cout << "state puzzle size" << std::endl;
	std::cout << state.puzzle.size() << std::endl;*/
	assert(puzzle.size == state.puzzle.size());
	
	/*if(counter == 10 && weight > 1.1)
	{
		return -g;
	}*/
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
	//std::cout << counter << std::endl;
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << "f" << std::endl;
	//std::cout << f << std::endl;
	/*std::cout << "top inverse" << std::endl;
    	std::cout << heur << std::endl;*/
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	clock_t t2;
	t2=clock();
	double diff1;
    	diff1 = ((double)(t2-t1)) / CLOCKS_PER_SEC;
    	if(diff1 > 1800)
    	{
    		return -100;
    	}
	int trans_cost;
	int min = -1;
	int temp = 0;
	int ggaps = 0;
	int h;
	bool set = false;
	//bool locked = locked_or_not2(state);
	//bool one_move = one_move_only(state);
	/*std::cout << "before state" << std::endl;
	std::cout << state << std::endl;*/
	//std::cout << locked << std::endl;
	//IMPLEMENT THE BINARY TREE THING IN THE FIRST ITERATION
	for(int i = 2; i <= int(puzzle.size); i++)
	{
		if(i == parent)
		{
			continue;
		}
		ggaps = gaps;
		if(i != int(puzzle.size))
		{
		if(int(abs(state.puzzle[i] - state.puzzle[i - 1])) == 1)
		{
			ggaps++;
		}
		if(int(abs(state.puzzle[0] - state.puzzle[i])) == 1)
		{
			ggaps--;
		}
		}
		else
		{
			if(state.puzzle[i - 1] == int(state.puzzle.size()) - 1)
			{
				ggaps++;
			}
			else if(state.puzzle[0] == int(state.puzzle.size()) - 1)
			{
				ggaps--;
			}
		}
		/*std::cout << "i" << std::endl;
		std::cout << i << std::endl;*/
		
		/*std::cout << "h" << std::endl;
		std::cout << ggaps << std::endl;*/
		counter++;
		
		trans_cost = puzzle.GetActionCost(state, i);
		if(ggaps == 0)
		{
			return -(g + trans_cost);
		}
		h = ggaps;
		temp = g + trans_cost + ggaps;
		if(temp > depth)
		{
			if(set == false or temp < min)
			{
				min = temp;
				set = true;
			}
			continue;
		}
		//std::cout << "hehehehehehe" << std::endl;
		
		h = h + gap_decreasing_moves_no_inv_lbf(state, ggaps, i);
		/*std::cout << "before state" << std::endl;
		std::cout << state << std::endl;
		puzzle.ApplyAction(state, i);
		std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		if(h - ggaps != TopHeurWithInverseAndL(state, ggaps))
		{
			std::cout << "TopHeurWithInverseAndLBF!!!!!!!!!!!!!!!!!!!!" << std::endl;
			std::cout << h - ggaps << std::endl;
		}
		puzzle.ApplyAction(state, i);*/
		top_counter++;
		
		temp = g + trans_cost + h;
		/*std::cout << "temp" << std::endl;
		std::cout << temp << std::endl;*/
		if(temp > depth)
		{
			if(set == false or temp < min)
			{
				min = temp;
				set = true;
			}
			continue;
		}
		
		puzzle.ApplyAction2(state, i);
		
		action_counter++;
		/*std::cout << "child" << std::endl;
		std::cout << state << std::endl;*/
		/*std::cout << "action" << std::endl;
		std::cout << i << std::endl;
		std::cout << "inverse" << std::endl;*/
		/*for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "gap" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "gapps" << std::endl;
		std::cout << ggaps << std::endl;
		std::cout << "gaapps" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
		
		/*if(h != GapHeur(state))
		{
			std::cout << "state" << std::endl;
			std::cout << state << std::endl;
			std::cout << "flip" << std::endl;
			std::cout << i << std::endl;
			std::cout << h << std::endl;
			std::cout << GapHeur(state) << std::endl;
			exit(0);
		}*/
		/*if(!locked && !one_move && !found && h < heur)
		{
			found = true;
			move = i;
		}*/
		
		/*if((greater_pruning && h > f - g) || (equal_pruning && !locked && h == f - g))
		{
			puzzle.ApplyAction(state, i);
			continue;
		}*/
		temp = Search_L_No_Inverse_EC(state, g + trans_cost, h, depth, i, ggaps, t1);
		if(temp < 0)
		{
			path.push_back(i);
			//std::cout << i << std::endl;
			puzzle.ApplyAction2(state, i);
			return temp;
		}
		puzzle.ApplyAction2(state, i);
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	return min;
}

int IDA::Search_L_No_Inverse_FG_EC(PancakeState& state, int g, int heur, int depth, int parent, int gaps, clock_t &t1) {
	/*std::cout << "puzzle size" << std::endl;
	std::cout << puzzle.size << std::endl;
	std::cout << "state puzzle size" << std::endl;
	std::cout << state.puzzle.size() << std::endl;*/
	assert(puzzle.size == state.puzzle.size());
	
	/*if(counter == 10 && weight > 1.1)
	{
		return -g;
	}*/
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
	//std::cout << counter << std::endl;
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << "f" << std::endl;
	//std::cout << f << std::endl;
	/*std::cout << "top inverse" << std::endl;
    	std::cout << heur << std::endl;*/
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	/*clock_t t2;
	t2=clock();
	double diff1;
    	diff1 = ((double)(t2-t1)) / CLOCKS_PER_SEC;
    	if(diff1 > 1800)
    	{
    		return -100;
    	}*/
	int trans_cost;
	int min = -1;
	int temp = 0;
	int ggaps = 0;
	int h;
	bool set = false;
	//bool locked = locked_or_not2(state);
	//bool one_move = one_move_only(state);
	/*std::cout << "before state" << std::endl;
	std::cout << state << std::endl;*/
	//std::cout << locked << std::endl;
	//IMPLEMENT THE BINARY TREE THING IN THE FIRST ITERATION
	for(int i = 2; i <= int(puzzle.size); i++)
	{
		if(i == parent)
		{
			continue;
		}
		ggaps = gaps;
		if(i != int(puzzle.size))
		{
		if(int(abs(state.puzzle[i] - state.puzzle[i - 1])) == 1)
		{
			ggaps++;
		}
		if(int(abs(state.puzzle[0] - state.puzzle[i])) == 1)
		{
			ggaps--;
		}
		}
		else
		{
			if(state.puzzle[i - 1] == int(state.puzzle.size()) - 1)
			{
				ggaps++;
			}
			else if(state.puzzle[0] == int(state.puzzle.size()) - 1)
			{
				ggaps--;
			}
		}
		/*std::cout << "i" << std::endl;
		std::cout << i << std::endl;*/
		
		/*std::cout << "h" << std::endl;
		std::cout << ggaps << std::endl;*/
		counter++;
		
		trans_cost = puzzle.GetActionCost(state, i);
		if(ggaps == 0)
		{
			return -(g + trans_cost);
		}
		h = ggaps;
		temp = g + trans_cost + ggaps;
		if(temp > depth)
		{
			if(set == false or temp < min)
			{
				min = temp;
				set = true;
			}
			continue;
		}
		//std::cout << "hehehehehehe" << std::endl;
		
		h = h + gap_decreasing_moves_no_inv_lbffg(state, ggaps, i);
		/*std::cout << "before state" << std::endl;
		std::cout << state << std::endl;
		puzzle.ApplyAction(state, i);
		std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		if(h - ggaps != TopHeurWithInverseAndL(state, ggaps))
		{
			std::cout << "TopHeurWithInverseAndLBF!!!!!!!!!!!!!!!!!!!!" << std::endl;
			std::cout << h - ggaps << std::endl;
		}
		puzzle.ApplyAction(state, i);*/
		top_counter++;
		
		temp = g + trans_cost + h;
		/*std::cout << "temp" << std::endl;
		std::cout << temp << std::endl;*/
		if(temp > depth)
		{
			if(set == false or temp < min)
			{
				min = temp;
				set = true;
			}
			continue;
		}
		
		puzzle.ApplyAction2(state, i);
		
		action_counter++;
		/*std::cout << "child" << std::endl;
		std::cout << state << std::endl;*/
		/*std::cout << "action" << std::endl;
		std::cout << i << std::endl;
		std::cout << "inverse" << std::endl;*/
		/*for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "gap" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "gapps" << std::endl;
		std::cout << ggaps << std::endl;
		std::cout << "gaapps" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
		
		/*if(h != GapHeur(state))
		{
			std::cout << "state" << std::endl;
			std::cout << state << std::endl;
			std::cout << "flip" << std::endl;
			std::cout << i << std::endl;
			std::cout << h << std::endl;
			std::cout << GapHeur(state) << std::endl;
			exit(0);
		}*/
		/*if(!locked && !one_move && !found && h < heur)
		{
			found = true;
			move = i;
		}*/
		
		/*if((greater_pruning && h > f - g) || (equal_pruning && !locked && h == f - g))
		{
			puzzle.ApplyAction(state, i);
			continue;
		}*/
		temp = Search_L_No_Inverse_FG_EC(state, g + trans_cost, h, depth, i, ggaps, t1);
		if(temp < 0)
		{
			path.push_back(i);
			//std::cout << i << std::endl;
			puzzle.ApplyAction2(state, i);
			return temp;
		}
		puzzle.ApplyAction2(state, i);
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	return min;
}

int IDA::Search_Inverse_NoL2(PancakeState& state, int g, int heur, int depth, int parent, int gaps) {
	/*std::cout << "puzzle size" << std::endl;
	std::cout << puzzle.size << std::endl;
	std::cout << "state puzzle size" << std::endl;
	std::cout << state.puzzle.size() << std::endl;*/
	assert(puzzle.size == state.puzzle.size());
	
	/*if(counter == 10 && weight > 1.1)
	{
		return -g;
	}*/
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
	//std::cout << counter << std::endl;
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << "f" << std::endl;
	//std::cout << f << std::endl;
	/*std::cout << "top inverse" << std::endl;
    	std::cout << heur << std::endl;*/
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	int trans_cost;
	int min = -1;
	int temp = 0;
	int ggaps = 0;
	int h;
	bool set = false;
	//bool locked = locked_or_not2(state);
	//bool one_move = one_move_only(state);
	//std::cout << locked << std::endl;
	//IMPLEMENT THE BINARY TREE THING IN THE FIRST ITERATION
	for(int i = 2; i <= int(puzzle.size); i++)
	{
		ggaps = gaps;
		if(i == parent)
		{
			continue;
		}
		trans_cost = puzzle.GetActionCost(state, i);
		//put gap code here and compare
		puzzle.ApplyAction(state, i);
		/*std::cout << "child" << std::endl;
		std::cout << state << std::endl;*/
		/*std::cout << "action" << std::endl;
		std::cout << i << std::endl;*/
		/*std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << ' ';
		}
		std::cout << std::endl;*/
		/*std::cout << "gap" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		if(i != int(puzzle.size))
		{
		if(int(abs(state.puzzle[0] - state.puzzle[i])) == 1)
		{
			ggaps++;
		}
		if(int(abs(state.puzzle[i - 1] - state.puzzle[i])) == 1)
		{
			ggaps--;
		}
		}
		else
		{
			if(state.puzzle[i - 1] == int(state.puzzle.size()) - 1)
			{
				ggaps--;
			}
			else if(state.puzzle[0] == int(state.puzzle.size()) - 1)
			{
				ggaps++;
			}
		}
		/*std::cout << "i" << std::endl;
		std::cout << i << std::endl;*/
		/*std::cout << "gapps" << std::endl;
		std::cout << ggaps << std::endl;
		std::cout << "gaapps" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
		h = ggaps + TopHeurWithInverseNoL2(state, ggaps);
		/*std::cout << "h" << std::endl;
		std::cout << TopHeurWithInverseNoL2(state, ggaps) << std::endl;*/
		/*if(h != GapHeur(state))
		{
			std::cout << "state" << std::endl;
			std::cout << state << std::endl;
			std::cout << "flip" << std::endl;
			std::cout << i << std::endl;
			std::cout << h << std::endl;
			std::cout << GapHeur(state) << std::endl;
			exit(0);
		}*/
		/*if(!locked && !one_move && !found && h < heur)
		{
			found = true;
			move = i;
		}*/
		counter++;
		/*if((greater_pruning && h > f - g) || (equal_pruning && !locked && h == f - g))
		{
			puzzle.ApplyAction(state, i);
			continue;
		}*/
		temp = Search_Inverse_NoL2(state, g + trans_cost, h, depth, i, ggaps);
		if(temp < 0)
		{
			path.push_back(i);
			//std::cout << i << std::endl;
			puzzle.ApplyAction(state, i);
			return temp;
		}
		puzzle.ApplyAction(state, i);
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	return min;
}

int IDA::Search_Inverse_NoL(PancakeState& state, int g, int heur, int depth, int parent, int gaps, int flip) {
	/*std::cout << "puzzle size" << std::endl;
	std::cout << puzzle.size << std::endl;
	std::cout << "state puzzle size" << std::endl;
	std::cout << state.puzzle.size() << std::endl;*/
	assert(puzzle.size == state.puzzle.size());
	
	/*if(counter == 10 && weight > 1.1)
	{
		return -g;
	}*/
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
	//std::cout << counter << std::endl;
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << "f" << std::endl;
	//std::cout << f << std::endl;
	/*std::cout << "top inverse" << std::endl;
    	std::cout << heur << std::endl;*/
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	int trans_cost;
	int min = f + 1;
	int temp = 0;
	int ggaps = 0;
	int h;
	bool set = false;
	//bool locked = locked_or_not2(state);
	//bool one_move = one_move_only(state);
	
	
	/*std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << locked << std::endl;
	//IMPLEMENT THE BINARY TREE THING IN THE FIRST ITERATION
	//std::vector<unsigned short int> child_moves;
	/*std::vector<unsigned short int> moves;
	
	if(f == depth && heur - gaps != 1)
	{
		
		gap_decreasing_moves3(state, moves, 0);
	}
	else
	{
		for(int i = 2; i <= int(puzzle.size); i++)
		{
			moves.push_back(i);
		}
	}*/
	/*for(int i = 0; i < int(moves.size()); i++)
	{
		std::cout << moves[i] << std::endl;
	}*/
	//int i;
	for(/*int j = 0; j < int(moves.size()); j++*/int i = 2; i <= int(puzzle.size); i++)
	{
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "initinverse" << std::endl;
		for(unsigned k = 0; k < state.puzzle.size(); k++)
		{
			std::cout << state.inverse[k] << ' ';
		}
		std::cout << std::endl;*/
		//i = moves[j];
		if(i == parent)
		{
			continue;
		}
		ggaps = gaps;
		if(i != int(puzzle.size))
		{
		if(int(abs(state.puzzle[i] - state.puzzle[i - 1])) == 1)
		{
			ggaps++;
		}
		if(int(abs(state.puzzle[0] - state.puzzle[i])) == 1)
		{
			ggaps--;
		}
		}
		else
		{
			if(state.puzzle[i - 1] == int(state.puzzle.size()) - 1)
			{
				ggaps++;
			}
			else if(state.puzzle[0] == int(state.puzzle.size()) - 1)
			{
				ggaps--;
			}
		}
		/*std::cout << "i" << std::endl;
		std::cout << i << std::endl;
		std::cout << "h" << std::endl;
		std::cout << TopHeurWithInverseNoL(state, ggaps, i) << std::endl;*/
		
		
		trans_cost = puzzle.GetActionCost(state, i);
		if(ggaps == 0)
		{
			return -(g + trans_cost);
		}
		counter++;
		h = ggaps;
		temp = g + trans_cost + h;
		/*std::cout << "temp" << std::endl;
		std::cout << temp << std::endl;*/
		if(temp > depth)
		{
			if(set == false or temp < min)
			{
				min = temp;
				set = true;
			}
			continue;
		}
		else if(temp == depth)
		{
		if(!updated)
		{
		//std::cout << "yolo" << std::endl;
		for(int j = 0; j < flip; j++)
    		{
    			state.inverse[state.puzzle[j]] = j;
    		}
    		updated = true;
    		}
    		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
    		std::cout << "i" << std::endl;
    		std::cout << i << std::endl;
    		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << ' ';
		}
		std::cout << std::endl;*/
		h = h + TopHeurWithInverseNoL(state, ggaps, i);
		/*std::cout << "h" << std::endl;
		std::cout << h << std::endl;*/
		top_counter++;
		
		temp = g + trans_cost + h;
		/*std::cout << "temp" << std::endl;
		std::cout << temp << std::endl;*/
		if(temp > depth)
		{
			if(set == false or temp < min)
			{
				min = temp;
				set = true;
			}
			continue;
		}
		
		
		}
		if(updated)
		{
			puzzle.ApplyAction(state, i);
		}
		else
		{
			puzzle.ApplyAction2(state, i);
		}
		action_counter++;
		/*std::cout << "child" << std::endl;
		std::cout << state << std::endl;*/
		/*std::cout << "action" << std::endl;
		std::cout << i << std::endl;
		std::cout << "inverse" << std::endl;*/
		/*for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "gap" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "gapps" << std::endl;
		std::cout << ggaps << std::endl;
		std::cout << "gaapps" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
		
		/*if(h != GapHeur(state))
		{
			std::cout << "state" << std::endl;
			std::cout << state << std::endl;
			std::cout << "flip" << std::endl;
			std::cout << i << std::endl;
			std::cout << h << std::endl;
			std::cout << GapHeur(state) << std::endl;
			exit(0);
		}*/
		/*if(!locked && !one_move && !found && h < heur)
		{
			found = true;
			move = i;
		}*/
		
		/*if((greater_pruning && h > f - g) || (equal_pruning && !locked && h == f - g))
		{
			puzzle.ApplyAction(state, i);
			continue;
		}*/
		/*child_moves.clear();
		if(h - ggaps != 1)
		{
			gap_decreasing_moves3(state, child_moves, 0);
			for(unsigned k = 0; k < child_moves.size(); k++)
			{
				if(TopHeurWithInverseNoL(state, ggaps - 1, child_moves[k]) == 0)
				{
					break;
				}
				if(k == child_moves.size() - 1)
				{
					h = h + 1;
				}
			}
		}*/
		temp = Search_Inverse_NoL(state, g + trans_cost, h, depth, i, ggaps, std::max(flip, i));
		if(temp < 0)
		{
			path.push_back(i);
			//std::cout << i << std::endl;
			puzzle.ApplyAction(state, i);
			return temp;
		}
		if(!updated)
		{
			puzzle.ApplyAction2(state, i);
		}
		else
		{
			puzzle.ApplyAction(state, i);
		}
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	/*std::cout << "min" << std::endl;
	std::cout << min << std::endl;*/
	return min;
}

int IDA::Search_Top_No_Inverse(PancakeState& state, int g, int heur, int depth, int parent, int gaps) {
	/*std::cout << "puzzle size" << std::endl;
	std::cout << puzzle.size << std::endl;
	std::cout << "state puzzle size" << std::endl;
	std::cout << state.puzzle.size() << std::endl;*/
	assert(puzzle.size == state.puzzle.size());
	//can add flip only when you get into the function
	
	/*if(counter == 10 && weight > 1.1)
	{
		return -g;
	}*/
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
	//std::cout << counter << std::endl;
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << "f" << std::endl;
	//std::cout << f << std::endl;
	/*std::cout << "top inverse" << std::endl;
    	std::cout << heur << std::endl;*/
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	int trans_cost;
	int min = -1;
	int temp = 0;
	int ggaps = 0;
	int h;
	bool set = false;
	//bool locked = locked_or_not2(state);
	//bool one_move = one_move_only(state);
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	//std::cout << locked << std::endl;
	//IMPLEMENT THE BINARY TREE THING IN THE FIRST ITERATION
	for(int i = 2; i <= int(puzzle.size); i++)
	{
		ggaps = gaps;
		if(i == parent)
		{
			continue;
		}
		trans_cost = puzzle.GetActionCost(state, i);
		puzzle.ApplyAction2(state, i);
		/*std::cout << "child" << std::endl;
		std::cout << state << std::endl;*/
		/*std::cout << "action" << std::endl;
		std::cout << i << std::endl;
		std::cout << "inverse" << std::endl;*/
		/*for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "gap" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		if(i != int(puzzle.size))
		{
		if(int(abs(state.puzzle[0] - state.puzzle[i])) == 1)
		{
			ggaps++;
		}
		if(int(abs(state.puzzle[i - 1] - state.puzzle[i])) == 1)
		{
			ggaps--;
		}
		}
		else
		{
			if(state.puzzle[i - 1] == int(state.puzzle.size()) - 1)
			{
				ggaps--;
			}
			else if(state.puzzle[0] == int(state.puzzle.size()) - 1)
			{
				ggaps++;
			}
		}
		/*std::cout << "gapps" << std::endl;
		std::cout << ggaps << std::endl;
		std::cout << "gaapps" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
		h = ggaps + TopHeur(state, ggaps);
		/*if(h != GapHeur(state))
		{
			std::cout << "state" << std::endl;
			std::cout << state << std::endl;
			std::cout << "flip" << std::endl;
			std::cout << i << std::endl;
			std::cout << h << std::endl;
			std::cout << GapHeur(state) << std::endl;
			exit(0);
		}*/
		/*if(!locked && !one_move && !found && h < heur)
		{
			found = true;
			move = i;
		}*/
		counter++;
		/*if((greater_pruning && h > f - g) || (equal_pruning && !locked && h == f - g))
		{
			puzzle.ApplyAction2(state, i);
			continue;
		}*/
		temp = Search_Top_No_Inverse(state, g + trans_cost, h, depth, i, ggaps);
		if(temp < 0)
		{
			path.push_back(i);
			//std::cout << i << std::endl;
			puzzle.ApplyAction2(state, i);
			return temp;
		}
		puzzle.ApplyAction2(state, i);
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	return min;
}

int IDA::Search_L_No_Inverse(PancakeState& state, int g, int heur, int depth, int parent, int gaps) {
	/*std::cout << "puzzle size" << std::endl;
	std::cout << puzzle.size << std::endl;
	std::cout << "state puzzle size" << std::endl;
	std::cout << state.puzzle.size() << std::endl;*/
	assert(puzzle.size == state.puzzle.size());
	
	/*if(counter == 10 && weight > 1.1)
	{
		return -g;
	}*/
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
	//std::cout << counter << std::endl;
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << "f" << std::endl;
	//std::cout << f << std::endl;
	/*std::cout << "top inverse" << std::endl;
    	std::cout << heur << std::endl;*/
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	int trans_cost;
	int min = -1;
	int temp = 0;
	int ggaps = 0;
	int h;
	bool set = false;
	//bool locked = locked_or_not2(state);
	//bool one_move = one_move_only(state);
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	//std::cout << locked << std::endl;
	//IMPLEMENT THE BINARY TREE THING IN THE FIRST ITERATION
	for(int i = 2; i <= int(puzzle.size); i++)
	{
		ggaps = gaps;
		if(i == parent)
		{
			continue;
		}
		trans_cost = puzzle.GetActionCost(state, i);
		puzzle.ApplyAction2(state, i);
		/*std::cout << "child" << std::endl;
		std::cout << state << std::endl;*/
		/*std::cout << "action" << std::endl;
		std::cout << i << std::endl;
		std::cout << "inverse" << std::endl;*/
		/*for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "gap" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		if(i != int(puzzle.size))
		{
		if(int(abs(state.puzzle[0] - state.puzzle[i])) == 1)
		{
			ggaps++;
		}
		if(int(abs(state.puzzle[i - 1] - state.puzzle[i])) == 1)
		{
			ggaps--;
		}
		}
		else
		{
			if(state.puzzle[i - 1] == int(state.puzzle.size()) - 1)
			{
				ggaps--;
			}
			else if(state.puzzle[0] == int(state.puzzle.size()) - 1)
			{
				ggaps++;
			}
		}
		/*std::cout << "gapps" << std::endl;
		std::cout << ggaps << std::endl;
		std::cout << "gaapps" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
		/*std::cout << "topheur" << std::endl;
		std::cout << gap_decreasing_moves_no_inv_l(state, ggaps) << std::endl;*/
		h = ggaps + gap_decreasing_moves_no_inv_l(state, ggaps);
		/*if(h != GapHeur(state))
		{
			std::cout << "state" << std::endl;
			std::cout << state << std::endl;
			std::cout << "flip" << std::endl;
			std::cout << i << std::endl;
			std::cout << h << std::endl;
			std::cout << GapHeur(state) << std::endl;
			exit(0);
		}*/
		/*if(!locked && !one_move && !found && h < heur)
		{
			found = true;
			move = i;
		}*/
		counter++;
		/*if((greater_pruning && h > f - g) || (equal_pruning && !locked && h == f - g))
		{
			puzzle.ApplyAction2(state, i);
			continue;
		}*/
		temp = Search_L_No_Inverse(state, g + trans_cost, h, depth, i, ggaps);
		if(temp < 0)
		{
			path.push_back(i);
			//std::cout << i << std::endl;
			puzzle.ApplyAction2(state, i);
			return temp;
		}
		puzzle.ApplyAction2(state, i);
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	return min;
}

int IDA::Search_L_No_Inverse_FG(PancakeState& state, int g, int heur, int depth, int parent, int gaps) {
	/*std::cout << "puzzle size" << std::endl;
	std::cout << puzzle.size << std::endl;
	std::cout << "state puzzle size" << std::endl;
	std::cout << state.puzzle.size() << std::endl;*/
	assert(puzzle.size == state.puzzle.size());
	
	/*if(counter == 10 && weight > 1.1)
	{
		return -g;
	}*/
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
	//std::cout << counter << std::endl;
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << "f" << std::endl;
	//std::cout << f << std::endl;
	/*std::cout << "top inverse" << std::endl;
    	std::cout << heur << std::endl;*/
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	int trans_cost;
	int min = -1;
	int temp = 0;
	int ggaps = 0;
	int h;
	bool set = false;
	//bool locked = locked_or_not2(state);
	//bool one_move = one_move_only(state);
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	//std::cout << locked << std::endl;
	//IMPLEMENT THE BINARY TREE THING IN THE FIRST ITERATION
	for(int i = 2; i <= int(puzzle.size); i++)
	{
		ggaps = gaps;
		if(i == parent)
		{
			continue;
		}
		trans_cost = puzzle.GetActionCost(state, i);
		puzzle.ApplyAction2(state, i);
		/*std::cout << "child" << std::endl;
		std::cout << state << std::endl;*/
		/*std::cout << "action" << std::endl;
		std::cout << i << std::endl;
		std::cout << "inverse" << std::endl;*/
		/*for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "gap" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		if(i != int(puzzle.size))
		{
		if(int(abs(state.puzzle[0] - state.puzzle[i])) == 1)
		{
			ggaps++;
		}
		if(int(abs(state.puzzle[i - 1] - state.puzzle[i])) == 1)
		{
			ggaps--;
		}
		}
		else
		{
			if(state.puzzle[i - 1] == int(state.puzzle.size()) - 1)
			{
				ggaps--;
			}
			else if(state.puzzle[0] == int(state.puzzle.size()) - 1)
			{
				ggaps++;
			}
		}
		/*std::cout << "gapps" << std::endl;
		std::cout << ggaps << std::endl;
		std::cout << "gaapps" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
		/*std::cout << "topheur" << std::endl;
		std::cout << gap_decreasing_moves_no_inv_l(state, ggaps) << std::endl;*/
		h = ggaps + gap_decreasing_moves_no_inv_l_FG2(state, ggaps);
		if(h - ggaps == 2)
		{
			//std::cout << state << std::endl;
			top_counter++;
		}
		/*if(h != GapHeur(state))
		{
			std::cout << "state" << std::endl;
			std::cout << state << std::endl;
			std::cout << "flip" << std::endl;
			std::cout << i << std::endl;
			std::cout << h << std::endl;
			std::cout << GapHeur(state) << std::endl;
			exit(0);
		}*/
		/*if(!locked && !one_move && !found && h < heur)
		{
			found = true;
			move = i;
		}*/
		counter++;
		/*if((greater_pruning && h > f - g) || (equal_pruning && !locked && h == f - g))
		{
			puzzle.ApplyAction2(state, i);
			continue;
		}*/
		temp = Search_L_No_Inverse_FG(state, g + trans_cost, h, depth, i, ggaps);
		if(temp < 0)
		{
			path.push_back(i);
			//std::cout << i << std::endl;
			puzzle.ApplyAction2(state, i);
			return temp;
		}
		puzzle.ApplyAction2(state, i);
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	return min;
}

int IDA::Search_Inverse_FG(PancakeState& state, int g, int heur, int depth, int parent, int gaps) {
	/*std::cout << "puzzle size" << std::endl;
	std::cout << puzzle.size << std::endl;
	std::cout << "state puzzle size" << std::endl;
	std::cout << state.puzzle.size() << std::endl;*/
	assert(puzzle.size == state.puzzle.size());
	
	/*if(counter == 10 && weight > 1.1)
	{
		return -g;
	}*/
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
	//std::cout << counter << std::endl;
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << "f" << std::endl;
	//std::cout << f << std::endl;
	/*std::cout << "top inverse" << std::endl;
    	std::cout << heur << std::endl;*/
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	int trans_cost;
	int min = -1;
	int temp = 0;
	int ggaps = 0;
	int h;
	bool set = false;
	//bool locked = locked_or_not2(state);
	//bool one_move = one_move_only(state);
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	//std::cout << locked << std::endl;
	//IMPLEMENT THE BINARY TREE THING IN THE FIRST ITERATION
	for(int i = 2; i <= int(puzzle.size); i++)
	{
		ggaps = gaps;
		if(i == parent)
		{
			continue;
		}
		trans_cost = puzzle.GetActionCost(state, i);
		puzzle.ApplyAction(state, i);
		/*std::cout << "child" << std::endl;
		std::cout << state << std::endl;*/
		/*std::cout << "action" << std::endl;
		std::cout << i << std::endl;
		std::cout << "inverse" << std::endl;*/
		/*for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "gap" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		if(i != int(puzzle.size))
		{
		if(int(abs(state.puzzle[0] - state.puzzle[i])) == 1)
		{
			ggaps++;
		}
		if(int(abs(state.puzzle[i - 1] - state.puzzle[i])) == 1)
		{
			ggaps--;
		}
		}
		else
		{
			if(state.puzzle[i - 1] == int(state.puzzle.size()) - 1)
			{
				ggaps--;
			}
			else if(state.puzzle[0] == int(state.puzzle.size()) - 1)
			{
				ggaps++;
			}
		}
		/*std::cout << "gapps" << std::endl;
		std::cout << ggaps << std::endl;
		std::cout << "gaapps" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		/*std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
		/*std::cout << "topheur" << std::endl;
		std::cout << gap_decreasing_moves_no_inv_l(state, ggaps) << std::endl;*/
		h = ggaps + TopHeurWithInverseAndLFG(state, ggaps);
		if(h - ggaps == 2)
		{
			std::cout << "state" << std::endl;
			std::cout << state << std::endl;
			std::cout << "inv" << std::endl;
			for(unsigned j = 0; j < state.puzzle.size(); j++)
			{
				std::cout << state.inverse[j] << ' ';
			}
			std::cout << std::endl;
			top_counter++;
		}
		/*if(h != GapHeur(state))
		{
			std::cout << "state" << std::endl;
			std::cout << state << std::endl;
			std::cout << "flip" << std::endl;
			std::cout << i << std::endl;
			std::cout << h << std::endl;
			std::cout << GapHeur(state) << std::endl;
			exit(0);
		}*/
		/*if(!locked && !one_move && !found && h < heur)
		{
			found = true;
			move = i;
		}*/
		counter++;
		/*if((greater_pruning && h > f - g) || (equal_pruning && !locked && h == f - g))
		{
			puzzle.ApplyAction2(state, i);
			continue;
		}*/
		temp = Search_Inverse_FG(state, g + trans_cost, h, depth, i, ggaps);
		if(temp < 0)
		{
			path.push_back(i);
			//std::cout << i << std::endl;
			puzzle.ApplyAction(state, i);
			return temp;
		}
		puzzle.ApplyAction(state, i);
		/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "inverse" << std::endl;
		for(unsigned j = 0; j < state.puzzle.size(); j++)
		{
			std::cout << state.inverse[j] << std::endl;
		}*/
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	return min;
}

int IDA::new_search2(PancakeState& state, int g, int heur, int depth, int parent) {
	assert(puzzle.size == state.puzzle.size());
	
	/*if(counter == 10 && weight > 1.1)
	{
		return -g;
	}*/
	if(puzzle.GoalTest(state))
	{
		//std::cout << "solution" << std::endl;
		return -g;
	}
	int f = g + heur;
	//std::cout << counter << std::endl;
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << "f" << std::endl;
	//std::cout << f << std::endl;
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	int trans_cost;
	int min = -1;
	int temp = 0;
	int h;
	bool set = false;
	bool locked = locked_or_not(state);
	bool one_move = one_move_only(state);
	//std::cout << "state" << std::endl;
	//std::cout << state << std::endl;
	//std::cout << locked << std::endl;
	for(int i = 2; i <= int(puzzle.size); i++)
	{
		if(i == parent)
		{
			continue;
		}
		trans_cost = puzzle.GetActionCost(state, i);
		puzzle.ApplyAction(state, i);
		/*std::cout << "gap" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		h = GapHeur(state);
		counter++;
		if((greater_pruning && h > f - g) || (equal_pruning && !locked && !one_move && h == f - g))
		{
			puzzle.ApplyAction(state, i);
			continue;
		}
		temp = new_search2(state, g + trans_cost, h, depth, i);
		if(temp < 0)
		{
			path.push_back(i);
			puzzle.ApplyAction(state, i);
			return temp;
		}
		puzzle.ApplyAction(state, i);
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	return min;
}

int IDA::Search_Reverse(PancakeState& state, int g, int depth, int parent) {
	assert(puzzle.size == state.puzzle.size());
	
	counter++;
	int f = g + GapHeur(state);
	if(f > depth)
	{
		//path.clear();
		return f;
	}
	if(puzzle.GoalTest(state))
	{
		//std::cout << "solution" << std::endl;
		return -g;
	}
	int trans_cost;
	int min = -1;
	int temp = 0;
	for(int i = int(puzzle.size); i >= 2; i--)
	{
		if(i == parent)
		{
			continue;
		}
		trans_cost = puzzle.GetActionCost(state, i);
		puzzle.ApplyAction(state, i);
		temp = Search_Reverse(state, g + trans_cost, depth, i);
		if(temp < 0)
		{
			path.push_back(i);
			puzzle.ApplyAction(state, i);
			return temp;
		}
		puzzle.ApplyAction(state, i);
		if(min == -1 or temp < min)
		{
			min = temp;
		}
	}
	return min;
}

/*double convertToDouble(std::vector<int>& list)
{
	double repr = 0;
	int digit = 1;
	for(unsigned i = 0; i < list.size(); i++)
	{
		repr += list[i] / double(digit);
		digit *= 10;
	}
	return repr;
}*/

unsigned int IDA::factorial(unsigned short n) 
{
    if (n == 0)
    {
    	return 1;
    }
    
    return n * factorial(n - 1);
}

unsigned int IDA::hash_fn(std::vector<int>& list)
{
	std::vector<int> puzzle = list;
	unsigned int hashVal = 0;
	int numEntriesLeft = list.size();
	for (unsigned int x = 0; x < list.size(); x++)
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

void IDA::reverse_hash(PancakeState &s, unsigned int hash)
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

int IDA::Search2(PancakeState& state, int g, int heur, int depth, int parent, std::vector<int>& history) {
	/*std::cout << "puzzle size" << std::endl;
	std::cout << puzzle.size << std::endl;
	std::cout << "state puzzle size" << std::endl;
	std::cout << state.puzzle.size() << std::endl;*/
	assert(puzzle.size == state.puzzle.size());
	
	/*if(counter == 10 && weight > 1.1)
	{
		return -g;
	}*/
	if(puzzle.GoalTest(state))
	{
		//std::cout << "solution" << std::endl;
		//std::cout << "solution" << std::endl;
		/*for(unsigned i = 0; i < history.size(); i++)
		{
			std::cout << history[i] << std::endl;
		}*/
		paths.push_back(std::vector<int>());
		//std::cout << history.size() << std::endl;
		for(int i = history.size() - 1; i >= 0 ; i--)
		{
			paths[paths.size() - 1].push_back(history[i]);
		}
		//paths.push_back(hash_fn(history));
		flag = 1;
		number_of_solutions++;
		solution_points.push_back(counter);
		depth_sum += g;
		heur_sum += heur;
		leaf_sum++;
		/*if(has)
		{
			decreasing = true;
		}*/
		//has = true;
		//start = false;
		return g;
	}
	int f = g + heur;
	//std::cout << counter << std::endl;
	/*std::cout << "state" << std::endl;
	std::cout << state << std::endl;*/
	/*std::cout << "f" << std::endl;
	std::cout << f << std::endl;
	std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	//std::cout << "f" << std::endl;
	//std::cout << f << std::endl;
	if(f > depth)
	{
		//path.clear();
		//child_count++;
		return f;
	}
	counter++;
	int trans_cost;
	int min = -1;
	int temp = 0;
	int h;
	bool set = false;
	bool locked = locked_or_not(state);
	bool one_move = one_move_only(state);
	if(locked)
	{
		leaf_sum++;
		depth_sum += g;
		heur_sum += heur;
		zero_child++;
	}
	else if(one_move)
	{
		one_child++;
	}
	else
	{
		two_child++;
	}
	//std::cout << "state" << std::endl;
	//std::cout << state << std::endl;
	//std::cout << locked << std::endl;
	for(int i = 2; i <= int(puzzle.size); i++)
	{
		if(i == parent)
		{
			continue;
		}
		trans_cost = puzzle.GetActionCost(state, i);
		puzzle.ApplyAction(state, i);
		/*std::cout << "gap" << std::endl;
		std::cout << GapHeur(state) << std::endl;*/
		h = GapHeur(state);
		/*if(!decreasing)
		{
		if(h < heur)
		{
			start = true;
		}
		else
		{
			if(start)
			{
				has = false;
			}
		}
		}*/
		if((greater_pruning && h > f - g) || (equal_pruning && !locked && h == f - g))
		{
			puzzle.ApplyAction(state, i);
			continue;
		}
		history.push_back(i);
		temp = Search2(state, g + trans_cost, h, depth, i, history);
		history.pop_back();
		puzzle.ApplyAction(state, i);
		if(set == false or temp < min)
		{
			min = temp;
			set = true;
		}
	}
	return min;
}

void IDA::go_through(PancakeState& state, int heur, std::vector<int>& path) {
	assert(puzzle.size == state.puzzle.size());
	
	if(path.size() == 0)
	{
		return;
	}
	int h;
	int movee = path[path.size() - 1];
	puzzle.ApplyAction(state, movee);
	path.pop_back();
	h = GapHeur(state);
	if(h < heur)
	{
		start = true;
	}
	else
	{
		if(start)
		{
			has = false;
		}
	}
	go_through(state, h, path);
	puzzle.ApplyAction(state, movee);
	path.push_back(movee);
}

int IDA::Solve(PancakeState& state) {
	//std::cout << size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	int depth = TopHeur(state, GapHeur(state));
	int h = depth;
	int temp = 0;
	//std::vector<int> t_path;
	while (1)
	{
		temp = Search(state, 0, h, depth, -1/*t_path*/);
		/*std::cout << "temp" << std::endl;
		std::cout << temp << std::endl;*/
		if(temp < 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
	}
}

int IDA::Solve2(PancakeState& state) {
	//std::cout << size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	/*start = false;
	has = true;
	decreasing = false;*/
	depth_sum = 0;
	heur_sum = 0;
	leaf_sum = 0;
	zero_child = 0;
	one_child = 0;
	two_child = 0;
	//child_count = 0;
	found = false;
	move = -1;
	counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	solution_points.clear();
	flag = 0;
	path.clear();
	paths.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	int depth = GapHeur(state);
	int h = depth;
	int temp = 0;
	std::vector<int> t_path;
	/*while (1)
	{*/
		temp = Search2(state, 0, h, depth, -1, t_path);
		/*std::cout << "temp" << std::endl;
		std::cout << temp << std::endl;*/
		if(temp < 0 || flag != 0)
		{
			return abs(temp);
		}
		else
		{
			return -1;
		}
		/*depth = temp;
		iterations++;*/
	//}
}
int IDA::Solve3(PancakeState& state) {
	//std::cout << puzzle.size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	srand ( time(NULL) );
	clock_t t1;
	solution_count = 0;
	counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	int gaps = GapHeur(state);
	int depth = gaps;
	int h = depth;
	int temp = 0;
	//std::vector<int> t_path;
	t1=clock();
	while (1)
	{
		temp = Search3(state, 0, h, depth, -1, gaps/*t_path*/, t1);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
		/*if(iterations > 0)
		{
			return -1;
		}*/
	}
}

int IDA::Solve3_Prune(PancakeState& state) {
	//std::cout << puzzle.size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	solution_count = 0;
	counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	int gaps = GapHeur(state);
	int depth = gaps;
	int h = depth;
	int temp = 0;
	//std::vector<int> t_path;
	while (1)
	{
		temp = Search3_Prune(state, 0, h, depth, -1, gaps/*t_path*/);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
		/*if(iterations > 1)
		{
			return -1;
		}*/
	}
}

int IDA::Solve3_Early(PancakeState& state) {
	//std::cout << puzzle.size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	solution_count = 0;
	counter = 0;
	action_counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	int gaps = GapHeur(state);
	//std::cout << gaps << std::endl;
	int depth = gaps;
	int h = depth;
	int temp = 0;
	clock_t t1;
	t1 = clock();
	//std::vector<int> t_path;
	while (1)
	{
		temp = Search3_Early(state, 0, h, depth, -1, gaps/*t_path*/, t1);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
		/*if(iterations > 1)
		{
			return -1;
		}*/
	}
}

int IDA::Solve3EC_Inverse(PancakeState& state) {
	//std::cout << puzzle.size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	solution_count = 0;
	counter = 0;
	top_counter = 0;
	action_counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	int gaps = GapHeur(state);
	int depth = gaps;
	int h = depth;
	int temp = 0;
	//std::vector<int> t_path;
	while (1)
	{
		temp = Search3EC_Inverse(state, 0, h, depth, -1, gaps/*t_path*/);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
		/*if(iterations > 1)
		{
			return -1;
		}*/
	}
}

int IDA::Solve3EC_With_Inverse(PancakeState& state) {
	//std::cout << puzzle.size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	solution_count = 0;
	counter = 0;
	top_counter = 0;
	action_counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	int gaps = GapHeur(state);
	int depth = gaps;
	int h = depth;
	int temp = 0;
	//std::vector<int> t_path;
	while (1)
	{
		temp = Search3EC_With_Inverse(state, 0, h, depth, -1, gaps/*t_path*/);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
		/*if(iterations > 1)
		{
			return -1;
		}*/
	}
}

int IDA::Solve_Ordered(PancakeState& state) {
	//std::cout << puzzle.size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	int depth = TopHeur(state, GapHeur(state));
	int h = depth;
	int temp = 0;
	//std::vector<int> t_path;
	while (1)
	{
		temp = Search_Ordered(state, 0, h, depth, -1, iterations/*t_path*/);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
		/*if(iterations > 1)
		{
			return -1;
		}*/
	}
}

int IDA::Solve_Big(PancakeState& state) {
	//std::cout << puzzle.size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	int depth = GapHeur(state);
	int h = depth;
	int temp = 0;
	//std::vector<int> t_path;
	while (1)
	{
		temp = Search_Big(state, 0, h, depth, -1/*t_path*/);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
		/*if(iterations > 1)
		{
			return -1;
		}*/
	}
}

int IDA::Solve_Top(PancakeState& state) {
	//std::cout << puzzle.size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	int depth = TopHeur(state, GapHeur(state));
	int h = depth;
	int temp = 0;
	//std::vector<int> t_path;
	while (1)
	{
		temp = Search_Top(state, 0, h, depth, -1/*t_path*/);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
		/*if(iterations > 1)
		{
			return -1;
		}*/
	}
}

int IDA::Solve_Inverse(PancakeState& state) {
	//std::cout << puzzle.size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	//std::cout << state << std::endl;
	int gaps = GapHeur(state);
	int depth = gaps + TopHeurWithInverseAndL(state, gaps);
	/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
	/*std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	int h = depth;
	int temp = 0;
	//std::vector<int> t_path;
	while (1)
	{
		/*std::cout << "iterations" << std::endl;
		std::cout << depth << std::endl;*/
		temp = Search_Inverse(state, 0, h, depth, -1, gaps /*t_path*/);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
		/*if(iterations > 1)
		{
			return -1;
		}*/
	}
}

int IDA::Solve_Inverse_EC(PancakeState& state) {
	//std::cout << puzzle.size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	counter = 0;
	top_counter = 0;
	action_counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	//std::cout << state << std::endl;
	int gaps = GapHeur(state);
	int depth = gaps + TopHeurWithInverseAndL(state, gaps);
	/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
	/*std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	int h = depth;
	int temp = 0;
	clock_t t1;
	t1 = clock();
	//std::vector<int> t_path;
	while (1)
	{
		/*std::cout << "iterations" << std::endl;
		std::cout << depth << std::endl;*/
		temp = Search_Inverse_EC(state, 0, h, depth, -1, gaps /*t_path*/, t1);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
		/*if(iterations > 1)
		{
			return -1;
		}*/
	}
}

int IDA::Solve_Inverse_FG_EC(PancakeState& state) {
	//std::cout << puzzle.size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	counter = 0;
	top_counter = 0;
	action_counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	//std::cout << state << std::endl;
	int gaps = GapHeur(state);
	int depth = gaps + TopHeurWithInverseAndLFG(state, gaps);
	/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
	/*std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	int h = depth;
	int temp = 0;
	clock_t t1;
	t1 = clock();
	//std::vector<int> t_path;
	while (1)
	{
		/*std::cout << "iterations" << std::endl;
		std::cout << depth << std::endl;*/
		temp = Search_Inverse_FG_EC(state, 0, h, depth, -1, gaps /*t_path*/, t1);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
		/*if(iterations > 1)
		{
			return -1;
		}*/
	}
}

int IDA::Solve_Top_No_Inverse_EC(PancakeState& state) {
	//std::cout << puzzle.size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	counter = 0;
	top_counter = 0;
	action_counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	//std::cout << state << std::endl;
	int gaps = GapHeur(state);
	int depth = gaps + TopHeur(state, gaps);
	/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
	/*std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	int h = depth;
	int temp = 0;
	clock_t t1;
	t1 = clock();
	//std::vector<int> t_path;
	while (1)
	{
		/*std::cout << "iterations" << std::endl;
		std::cout << depth << std::endl;*/
		temp = Search_Top_No_Inverse_EC(state, 0, h, depth, -1, gaps /*t_path*/, t1);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
		/*if(iterations > 1)
		{
			return -1;
		}*/
	}
}

int IDA::Solve_L_No_Inverse_EC(PancakeState& state) {
	//std::cout << puzzle.size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	counter = 0;
	top_counter = 0;
	action_counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	//std::cout << state << std::endl;
	int gaps = GapHeur(state);
	int depth = gaps + gap_decreasing_moves_no_inv_l(state, gaps);
	/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
	/*std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	int h = depth;
	int temp = 0;
	clock_t t1;
	t1 = clock();
	//std::vector<int> t_path;
	while (1)
	{
		/*std::cout << "iterations" << std::endl;
		std::cout << depth << std::endl;*/
		temp = Search_L_No_Inverse_EC(state, 0, h, depth, -1, gaps /*t_path*/, t1);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
		/*if(iterations > 1)
		{
			return -1;
		}*/
	}
}

int IDA::Solve_L_No_Inverse_FG_EC(PancakeState& state) {
	//std::cout << puzzle.size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	counter = 0;
	top_counter = 0;
	action_counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	//std::cout << state << std::endl;
	int gaps = GapHeur(state);
	int depth = gaps + gap_decreasing_moves_no_inv_l_FG2(state, gaps);
	/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
	/*std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	int h = depth;
	int temp = 0;
	clock_t t1;
	t1 = clock();
	//std::vector<int> t_path;
	while (1)
	{
		/*std::cout << "iterations" << std::endl;
		std::cout << depth << std::endl;*/
		temp = Search_L_No_Inverse_FG_EC(state, 0, h, depth, -1, gaps /*t_path*/, t1);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
		/*if(iterations > 1)
		{
			return -1;
		}*/
	}
}

int IDA::Solve_Top_With_Inverse_EC(PancakeState& state) {
	//std::cout << puzzle.size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	counter = 0;
	top_counter = 0;
	action_counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	//std::cout << state << std::endl;
	int gaps = GapHeur(state);
	int depth = gaps + TopHeurWithInverse(state, gaps);
	/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
	/*std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	int h = depth;
	int temp = 0;
	clock_t t1;
	t1 = clock();
	//std::vector<int> t_path;
	while (1)
	{
		/*std::cout << "iterations" << std::endl;
		std::cout << depth << std::endl;*/
		temp = Search_Top_With_Inverse_EC(state, 0, h, depth, -1, gaps /*t_path*/, t1);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
		/*if(iterations > 1)
		{
			return -1;
		}*/
	}
}

int IDA::Solve_Inverse_NoL(PancakeState& state) {
	//std::cout << puzzle.size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	counter = 0;
	top_counter = 0;
	action_counter=0;
	iterations = 0;
	updated = false;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	//std::cout << state << std::endl;
	int gaps = GapHeur(state);
	int depth = gaps + TopHeurWithInverseNoL2(state, gaps);
	/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
	/*std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	int h = depth;
	int temp = 0;
	//std::vector<int> t_path;
	while (1)
	{
		/*std::cout << "iterations" << std::endl;
		std::cout << depth << std::endl;*/
		updated = false;
		temp = Search_Inverse_NoL(state, 0, h, depth, -1, gaps, 0 /*t_path*/);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
		/*if(iterations > 1)
		{
			return -1;
		}*/
	}
}

int IDA::Solve_Inverse_NoL2(PancakeState& state) {
	//std::cout << puzzle.size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	counter = 0;
	top_counter = 0;
	action_counter=0;
	iterations = 0;
	updated = false;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	//std::cout << state << std::endl;
	int gaps = GapHeur(state);
	int depth = gaps + TopHeurWithInverseNoL2(state, gaps);
	/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
	/*std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	int h = depth;
	int temp = 0;
	//std::vector<int> t_path;
	while (1)
	{
		/*std::cout << "iterations" << std::endl;
		std::cout << depth << std::endl;*/
		updated = false;
		temp = Search_Inverse_NoL2(state, 0, h, depth, -1, gaps/*t_path*/);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
		/*if(iterations > 1)
		{
			return -1;
		}*/
	}
}

int IDA::Solve_Top_No_Inverse(PancakeState& state) {
	//std::cout << puzzle.size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	//std::cout << state << std::endl;
	int gaps = GapHeur(state);
	int depth = gaps + TopHeur(state, GapHeur(state));
	/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
	/*std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	int h = depth;
	int temp = 0;
	//std::vector<int> t_path;
	while (1)
	{
		/*std::cout << "iterations" << std::endl;
		std::cout << depth << std::endl;*/
		temp = Search_Top_No_Inverse(state, 0, h, depth, -1, gaps /*t_path*/);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
		/*if(iterations > 1)
		{
			return -1;
		}*/
	}
}

int IDA::Solve_L_No_Inverse(PancakeState& state) {
	//std::cout << puzzle.size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	//std::cout << state << std::endl;
	int gaps = GapHeur(state);
	int depth = gaps + gap_decreasing_moves_no_inv_l(state, gaps);
	/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
	/*std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	int h = depth;
	int temp = 0;
	//std::vector<int> t_path;
	while (1)
	{
		/*std::cout << "iterations" << std::endl;
		std::cout << depth << std::endl;*/
		temp = Search_L_No_Inverse(state, 0, h, depth, -1, gaps /*t_path*/);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
		/*if(iterations > 1)
		{
			return -1;
		}*/
	}
}

int IDA::Solve_L_No_Inverse_FG(PancakeState& state) {
	//std::cout << puzzle.size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	flag = 0;
	top_counter = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	//std::cout << state << std::endl;
	int gaps = GapHeur(state);
	int depth = gaps + gap_decreasing_moves_no_inv_l_FG2(state, gaps);
	/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
	/*std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	int h = depth;
	int temp = 0;
	//std::vector<int> t_path;
	while (1)
	{
		/*std::cout << "iterations" << std::endl;
		std::cout << depth << std::endl;*/
		temp = Search_L_No_Inverse_FG(state, 0, h, depth, -1, gaps /*t_path*/);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
		/*if(iterations > 1)
		{
			return -1;
		}*/
	}
}

int IDA::Solve_Inverse_FG(PancakeState& state) {
	//std::cout << puzzle.size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	top_counter = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	//std::cout << state << std::endl;
	int gaps = GapHeur(state);
	int depth = gaps + TopHeurWithInverseAndLFG(state, gaps);
	/*std::cout << "state" << std::endl;
		std::cout << state << std::endl;
		std::cout << "topheurwithinverse" << std::endl;
		std::cout << TopHeurWithInverse(state) << std::endl;*/
	/*std::cout << "depth" << std::endl;
	std::cout << depth << std::endl;*/
	int h = depth;
	int temp = 0;
	//std::vector<int> t_path;
	while (1)
	{
		/*std::cout << "iterations" << std::endl;
		std::cout << depth << std::endl;*/
		temp = Search_Inverse_FG(state, 0, h, depth, -1, gaps /*t_path*/);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
		/*if(iterations > 1)
		{
			return -1;
		}*/
	}
}

int IDA::new_solve(PancakeState& state) {
	//std::cout << puzzle.size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	int depth = GapHeur(state);
	int h = depth;
	int temp = 0;
	//std::vector<int> t_path;
	while (1)
	{
		temp = new_search(state, 0, h, depth, -1/*t_path*/);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
		/*if(iterations > 1)
		{
			return -1;
		}*/
	}
}

int IDA::new_solve2(PancakeState& state) {
	//std::cout << puzzle.size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	int depth = GapHeur(state);
	int h = depth;
	int temp = 0;
	//std::vector<int> t_path;
	while (1)
	{
		temp = new_search2(state, 0, h, depth, -1/*t_path*/);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
		/*if(iterations > 1)
		{
			return -1;
		}*/
	}
}

int IDA::Solve4(PancakeState& state) {
	//std::cout << size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	int depth = TopHeur(state, GapHeur(state));
	int h = depth;
	int temp = 0;
	//std::vector<int> t_path;
	while (1)
	{
		/*std::cout << "bound depth:" << std::endl;
		std::cout << depth << std::endl;
		std::cout << "threshold:" << std::endl;
		std::cout << threshold << std::endl;\
		std::cout << "bound iterations:" << std::endl;
		std::cout << iterations << std::endl;*/
		temp = Search(state, 0, h, depth, -1/*t_path*/);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		if(depth >= threshold)
		{
			return -1;
		}
		iterations++;
	}
}

int IDA::alternate_solve(PancakeState& state) {
	//std::cout << size << std::endl;
	//std::cout << state.puzzle.size() << std::endl;
	assert(puzzle.size == state.puzzle.size());
	
	counter = 0;
	iterations = 0;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	int gaps = GapHeur(state);
	int depth = gaps;
	int h = depth;
	int temp = 0;
	//std::vector<int> t_path;
	clock_t t1;
	t1 = clock();
	while (1)
	{
		/*std::cout << "alternate depth:" << std::endl;
		std::cout << depth << std::endl;
		std::cout << "alternate iterations:" << std::endl;
		std::cout << iterations << std::endl;*/
		temp = Search3(state, 0, h, depth, -1, gaps/*t_path*/, t1);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		if(iterations != 0)
		{
		if(!greater_pruning)
		{
			depth = temp;
		}
		greater_pruning = !greater_pruning;
		equal_pruning = !equal_pruning;
		}
		else
		{
			depth = temp;
		}
		iterations++;
	}
}
