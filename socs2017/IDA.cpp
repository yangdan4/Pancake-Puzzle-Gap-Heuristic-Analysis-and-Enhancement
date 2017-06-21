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

int IDA::TopHeurWithInverse(PancakeState& state, int gaps) const {
	assert(puzzle.size == state.puzzle.size());
	
	if(gaps == 0)
	{
		return 0;
	}
	
    	return /*weight **/ std::max(gap_decreasing_moves(state, 0), gap_decreasing_moves(state, 1));
}

int IDA::TopHeurWithInverseBF(PancakeState& state, int flip, int gaps) const {
	assert(puzzle.size == state.puzzle.size());
	
	if(gaps == 0)
	{
		return 0;
	}
	
    	return /*weight **/ std::max(gap_decreasing_moves2(state, flip, 0), gap_decreasing_moves2(state, flip, 1));
}

int IDA::TopHeurWithInverseAndL(PancakeState& state, int gaps) const {
	assert(puzzle.size == state.puzzle.size());
	
	if(gaps == 0)
	{
		return 0;
	}
    	return std::max(gap_decreasing_moves_with_inv_l(state, 0, gaps), gap_decreasing_moves_with_inv_l(state, 1, gaps));
}

int IDA::TopHeurWithLNoInverse(PancakeState& state, int gaps) const {
	assert(puzzle.size == state.puzzle.size());
	
	if(gaps == 0)
	{
		return 0;
	}
    	return gap_decreasing_moves_no_inv_l(state, gaps);
}

int IDA::TopHeurWithLNoInverseBF(PancakeState& state, int flip, int gaps) const {
	assert(puzzle.size == state.puzzle.size());
	
	if(gaps == 0)
	{
		return 0;
	}
    	return gap_decreasing_moves_no_inv_lbf(state, flip, gaps);
}

int IDA::TopHeurWithLNoInverseFG(PancakeState& state, int gaps) const {
	assert(puzzle.size == state.puzzle.size());
	
	if(gaps == 0)
	{
		return 0;
	}
    	return gap_decreasing_moves_no_inv_l_FG2(state, gaps);
}

int IDA::TopHeurWithLNoInverseFG_bf(PancakeState& state, int flip, int gaps) const {
	assert(puzzle.size == state.puzzle.size());
	
	if(gaps == 0)
	{
		return 0;
	}
    	return gap_decreasing_moves_no_inv_lbffg(state, flip, gaps);
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

void IDA::setThreshold(int new_threshold)
{
	threshold = new_threshold;
}
    
int IDA::getThreshold()
{
	return threshold;
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
    	return gap_decreasing_moves_no_inv(state);
}

int IDA::TopHeurBF(PancakeState& state, int flip, int gaps) const {
	assert(puzzle.size == state.puzzle.size());
	if(gaps == 0)
	{
		return 0;
	}
    	return gap_decreasing_moves_no_inv_flip(state, flip);
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
		if(gap_decreasing_moves(state, 0) == 0)
		{
			if(gaps - 1 == 0)
			{
				return 0;
			}
			int top = state.puzzle[0];
			if(top == 0)
			{
				return gap_decreasing_moves2(state, state.inverse[top + 1], 0);
			}
			else if(top == int(state.puzzle.size()) - 1)
			{
				if(gap_decreasing_moves2(state, state.puzzle.size(), 0) == 0)
				{
					return 0;
				}
				if(gap_decreasing_moves2(state, state.inverse[top - 1], 0) == 0)
				{
					return 0;
				}
				return 1;
			}
			else
			{
				if(gap_decreasing_moves2(state, state.inverse[top + 1], 0) == 0)
				{
					return 0;
				}
				if(gap_decreasing_moves2(state, state.inverse[top - 1], 0) == 0)
				{
					return 0;
				}
				return 1;
			}
		}
		else
		{
			return 1;
		}
	}
	else
	{
		if(gap_decreasing_moves(state, 1) == 0)
		{
			if(gaps - 1 == 0)
			{
				return 0;
			}
			int top = state.inverse[0];
			if(top == 0)
			{
				return gap_decreasing_moves2_v2(state, state.puzzle[top + 1], 0);;
			}
			else if(top == int(state.inverse.size()) - 1)
			{
				if(gap_decreasing_moves2_v2(state, state.inverse.size(), 0) == 0)
				{
					return 0;
				}
				if(gap_decreasing_moves2_v2(state, state.puzzle[top - 1], 0) == 0)
				{
					return 0;
				}
				return 1;
			}
			else
			{
				if(gap_decreasing_moves2_v2(state, state.puzzle[top + 1], 0) == 0)
				{
					return 0;
				}
				if(gap_decreasing_moves2_v2(state, state.puzzle[top - 1], 0) == 0)
				{
					return 0;
				}
				return 1;
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
		if(gap_decreasing_moves2(state, flip, 0) == 0)
		{
			if(gaps - 1 == 0)
			{
				return 0;
			}
			int top = state.puzzle[flip - 1];
			if(top == 0)
			{
				return gap_decreasing_moves2bf(state, place(state.inverse[top + 1], flip), flip, 0);
			}
			else if(top == int(state.puzzle.size()) - 1)
			{
				return std::min(gap_decreasing_moves2bf(state, state.puzzle.size(), flip, 0), gap_decreasing_moves2bf(state, place(state.inverse[top - 1], flip), flip, 0));
			}
			else
			{
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
		if(gap_decreasing_moves2(state, flip, 1) == 0)
		{
			if(gaps - 1 == 0)
			{
				return 0;
			}
			int top = place(state.inverse[0], flip);
			if(top == 0)
			{
				return gap_decreasing_moves2bf_v2(state, state.puzzle[place(top + 1, flip)], flip, 0);
			}
			else if(top == int(state.inverse.size()) - 1)
			{
				return std::min(gap_decreasing_moves2bf_v2(state, state.inverse.size(), flip, 0), gap_decreasing_moves2bf_v2(state, state.puzzle[place(top - 1, flip)], flip, 0));
			}
			else
			{
				return std::min(gap_decreasing_moves2bf_v2(state, state.puzzle[place(top + 1, flip)], flip, 0), gap_decreasing_moves2bf_v2(state, state.puzzle[place(top - 1, flip)], flip, 0));
			}
		}
		else
		{
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

int IDA::gap_decreasing_moves(PancakeState& state, int inv) const {
	assert(puzzle.size == state.puzzle.size());
	if(inv == 0)
	{
	int top = state.puzzle[0];
	
	if(top == int(state.puzzle.size()) - 1)
		{
			return 0;
		}
		else
		{
			if(top != 0)
			{
				int flip1 = state.inverse[top - 1];
				if(abs(state.puzzle[flip1 - 1] - top + 1) != 1)
				{
					return 0;
				}
			}
			int flip2 = state.inverse[top + 1];
			if(abs(state.puzzle[flip2 - 1] - top - 1) != 1)
			{
				return 0;
			}
			
			return 1;
		}
	}
	else
	{
	int top = state.inverse[0];
	
	if(top == int(state.inverse.size()) - 1)
		{
			return 0;
		}
		else
		{
			if(top != 0)
			{
				int flip1 = state.puzzle[top - 1];
				if(abs(state.inverse[flip1 - 1] - top + 1) != 1)
				{
					//moves++;
					return 0;
				}
			}
			int flip2 = state.puzzle[top + 1];
			if(abs(state.inverse[flip2 - 1] - top - 1) != 1)
			{
				return 0;
			}
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
	if(inv == 0)
	{
	int top = state.puzzle[ind - 1];
		if(top == int(state.puzzle.size()) - 1)
		{
			return 0;
		}
		else
		{
			if(top != 0)
			{
				int flip1 = place(state.inverse[top - 1], ind);
				if(abs(state.puzzle[place(flip1 - 1, ind)] - top + 1) != 1)
				{
					return 0;
				}
			}
			int flip2 = place(state.inverse[top + 1], ind);
			if(abs(state.puzzle[place(flip2 - 1, ind)] - top - 1) != 1)
			{
				return 0;
			}
			return 1;
		}
	}
	else
	{
		int top = place(state.inverse[0], ind);
		if(top == int(state.inverse.size()) - 1)
		{
			return 0;
		}
		else
		{
			if(top != 0)
			{
				int flip1 = state.puzzle[place(top - 1, ind)];
				if(abs(place(state.inverse[flip1 - 1], ind) - top + 1) != 1)
				{
					return 0;
				}
			}
			int flip2 = state.puzzle[place(top + 1, ind)];
			if(abs(place(state.inverse[flip2 - 1], ind) - top - 1) != 1)
			{
				return 0;
			}
			return 1;
		}
		
	}
	return 1;
}
int IDA::gap_decreasing_moves2bf(PancakeState& state, int ind, int flip, int inv) const {
	assert(puzzle.size == state.puzzle.size());
	if(ind == 1)
	{
		return 1;
	}
		int top = state.puzzle[place(ind - 1, flip)];
		if(top == int(state.puzzle.size()) - 1)
		{
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
			}
			int flip2 = place(place(state.inverse[top + 1], flip), ind);
			if(abs(state.puzzle[place(place(flip2 - 1, ind), flip)] - top - 1) != 1)
			{
				return 0;
			}
			return 1;
		}
}
int IDA::gap_decreasing_moves2bf_v2(PancakeState& state, int ind, int flip, int inv) const {
	assert(puzzle.size == state.puzzle.size());
	if(ind == 1)
	{
		return 1;
	}
		int top = place(state.inverse[ind - 1], flip);
		if(top == int(state.inverse.size()) - 1)
		{
			return 0;
		}
		else
		{
			if(top != 0)
			{
				int flip1 = place(state.puzzle[place(top - 1, flip)], ind);
				if(abs(place(state.inverse[place(flip1 - 1, ind)], flip) - top + 1) != 1)
				{
					return 0;
				}
			}
			int flip2 = place(state.puzzle[place(top + 1, flip)], ind);
			if(abs(place(state.inverse[place(flip2 - 1, ind)], flip) - top - 1) != 1)
			{
				return 0;
			}
			return 1;
		}
}

int IDA::gap_decreasing_moves2_v2(PancakeState& state, int ind, int inv) const {
	assert(puzzle.size == state.puzzle.size());
	if(ind == 1)
	{
		return 1;
	}
	if(inv == 0)
	{
		int top = state.inverse[ind - 1];
		if(top == int(state.inverse.size()) - 1)
		{
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
			}
			int flip2 = place(state.puzzle[top + 1], ind);
			if(abs(state.inverse[place(flip2 - 1, ind)] - top - 1) != 1)
			{
				return 0;
			}
			return 1;
		}
	}
	else
	{
		int afterpos = place(state.puzzle[0], ind);
		if(afterpos == int(state.inverse.size()) - 1)
		{
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
		
	}
	return 1;
}

int IDA::Search_Gap(PancakeState& state, int g, int heur, int depth, int parent, int gaps, clock_t &t1) {
	assert(puzzle.size == state.puzzle.size());
	if(puzzle.GoalTest(state))
	{
			return -g;
	}
	int f = g + heur;
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
	for(int i = 2; i <= int(puzzle.size); i++/*int j = 0; j < int(moves.size()); j++*/)
	{
		if(i == parent)
		{
			continue;
		}
		ggaps = gaps;
		trans_cost = puzzle.GetActionCost(state, i);
		puzzle.ApplyAction2(state, i);
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
		h = ggaps;
			counter++;
		temp = Search_Gap(state, g + trans_cost, h, depth, i, ggaps, t1);
		if(temp < 0)
		{
			path.push_back(i);
			puzzle.ApplyAction2(state, i);
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

int IDA::Search_Gap_EC(PancakeState& state, int g, int heur, int depth, int parent, int gaps, clock_t &t1) {
	assert(puzzle.size == state.puzzle.size());
	if(puzzle.GoalTest(state))
	{

			return -g;
	}
	int f = g + heur;
	if(f > depth)
	{
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
		temp = Search_Gap_EC(state, g + trans_cost, h, depth, i, ggaps, t1);
		if(temp < 0)
		{
			path.push_back(i);
			puzzle.ApplyAction2(state, i);
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

int IDA::Search_Inverse(PancakeState& state, int g, int heur, int depth, int parent, int gaps) {
	assert(puzzle.size == state.puzzle.size());
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
	if(f > depth)
	{
		return f;
	}
	int trans_cost;
	int min = -1;
	int temp = 0;
	int ggaps = 0;
	int h;
	bool set = false;
	for(int i = 2; i <= int(puzzle.size); i++)
	{
		ggaps = gaps;
		if(i == parent)
		{
			continue;
		}
		trans_cost = puzzle.GetActionCost(state, i);
		puzzle.ApplyAction(state, i);
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
		h = ggaps + TopHeurWithInverseAndL(state, ggaps);
		counter++;
		temp = Search_Inverse(state, g + trans_cost, h, depth, i, ggaps);
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

int IDA::Search_Inverse_EC(PancakeState& state, int g, int heur, int depth, int parent, int gaps, clock_t &t1) {
	assert(puzzle.size == state.puzzle.size());
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
	if(f > depth)
	{
		return f;
	}
	int trans_cost;
	int min = -1;
	int temp = 0;
	int ggaps = 0;
	int h;
	bool set = false;
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
		
		h = h + TopHeurWithInverseAndLBF(state, ggaps, i);
		top_counter++;
		
		temp = g + trans_cost + h;
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
		temp = Search_Inverse_EC(state, g + trans_cost, h, depth, i, ggaps, t1);
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

int IDA::Search_Inverse_FG_EC(PancakeState& state, int g, int heur, int depth, int parent, int gaps, clock_t &t1) {
	assert(puzzle.size == state.puzzle.size());
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
	if(f > depth)
	{
		return f;
	}
	int trans_cost;
	int min = -1;
	int temp = 0;
	int ggaps = 0;
	int h;
	bool set = false;
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
		
		h = h + TopHeurWithInverseAndLFG_bf(state, ggaps, i);
		
		
		
		top_counter++;
		
		temp = g + trans_cost + h;
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
		temp = Search_Inverse_FG_EC(state, g + trans_cost, h, depth, i, ggaps, t1);
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

int IDA::Search_Top_No_Inverse_EC(PancakeState& state, int g, int heur, int depth, int parent, int gaps, clock_t &t1) {
	assert(puzzle.size == state.puzzle.size());
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
	if(f > depth)
	{
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
		
		h = h + TopHeurBF(state, i, ggaps);
		top_counter++;
		
		temp = g + trans_cost + h;
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
		temp = Search_Top_No_Inverse_EC(state, g + trans_cost, h, depth, i, ggaps, t1);
		if(temp < 0)
		{
			path.push_back(i);
			puzzle.ApplyAction2(state, i);
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

int IDA::Search_Top_With_Inverse_EC(PancakeState& state, int g, int heur, int depth, int parent, int gaps, clock_t &t1) {
	assert(puzzle.size == state.puzzle.size());
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
	if(f > depth)
	{
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
		
		h = h + TopHeurWithInverseBF(state, i, ggaps);
		top_counter++;
		
		temp = g + trans_cost + h;
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
		temp = Search_Top_With_Inverse_EC(state, g + trans_cost, h, depth, i, ggaps, t1);
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

int IDA::Search_L_No_Inverse_EC(PancakeState& state, int g, int heur, int depth, int parent, int gaps, clock_t &t1) {
	assert(puzzle.size == state.puzzle.size());
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
	if(f > depth)
	{
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
		
		h = h + gap_decreasing_moves_no_inv_lbf(state, ggaps, i);
		top_counter++;
		
		temp = g + trans_cost + h;
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
		temp = Search_L_No_Inverse_EC(state, g + trans_cost, h, depth, i, ggaps, t1);
		if(temp < 0)
		{
			path.push_back(i);
			//std::cout << i << std::endl;
			puzzle.ApplyAction2(state, i);
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

int IDA::Search_L_No_Inverse_FG_EC(PancakeState& state, int g, int heur, int depth, int parent, int gaps, clock_t &t1) {
	assert(puzzle.size == state.puzzle.size());
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
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
		
		h = h + gap_decreasing_moves_no_inv_lbffg(state, ggaps, i);
		top_counter++;
		
		temp = g + trans_cost + h;
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
		temp = Search_L_No_Inverse_FG_EC(state, g + trans_cost, h, depth, i, ggaps, t1);
		if(temp < 0)
		{
			path.push_back(i);
			//std::cout << i << std::endl;
			puzzle.ApplyAction2(state, i);
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

int IDA::Search_Top_With_Inverse(PancakeState& state, int g, int heur, int depth, int parent, int gaps) {
	assert(puzzle.size == state.puzzle.size());
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
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
	for(int i = 2; i <= int(puzzle.size); i++)
	{
		ggaps = gaps;
		if(i == parent)
		{
			continue;
		}
		trans_cost = puzzle.GetActionCost(state, i);
		puzzle.ApplyAction(state, i);
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
		h = ggaps + TopHeurWithInverse(state, ggaps);
		counter++;
		temp = Search_Top_With_Inverse(state, g + trans_cost, h, depth, i, ggaps);
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

int IDA::Search_Top_No_Inverse(PancakeState& state, int g, int heur, int depth, int parent, int gaps) {
	assert(puzzle.size == state.puzzle.size());
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
	if(f > depth)
	{
		return f;
	}
	int trans_cost;
	int min = -1;
	int temp = 0;
	int ggaps = 0;
	int h;
	bool set = false;
	for(int i = 2; i <= int(puzzle.size); i++)
	{
		ggaps = gaps;
		if(i == parent)
		{
			continue;
		}
		trans_cost = puzzle.GetActionCost(state, i);
		puzzle.ApplyAction2(state, i);
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
		h = ggaps + TopHeur(state, ggaps);
		counter++;
		temp = Search_Top_No_Inverse(state, g + trans_cost, h, depth, i, ggaps);
		if(temp < 0)
		{
			path.push_back(i);
			puzzle.ApplyAction2(state, i);
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

int IDA::Search_L_No_Inverse(PancakeState& state, int g, int heur, int depth, int parent, int gaps) {
	assert(puzzle.size == state.puzzle.size());
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
	if(f > depth)
	{
		return f;
	}
	int trans_cost;
	int min = -1;
	int temp = 0;
	int ggaps = 0;
	int h;
	bool set = false;
	for(int i = 2; i <= int(puzzle.size); i++)
	{
		ggaps = gaps;
		if(i == parent)
		{
			continue;
		}
		trans_cost = puzzle.GetActionCost(state, i);
		puzzle.ApplyAction2(state, i);
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
		h = ggaps + gap_decreasing_moves_no_inv_l(state, ggaps);
		counter++;
		temp = Search_L_No_Inverse(state, g + trans_cost, h, depth, i, ggaps);
		if(temp < 0)
		{
			path.push_back(i);
			puzzle.ApplyAction2(state, i);
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

int IDA::Search_L_No_Inverse_FG(PancakeState& state, int g, int heur, int depth, int parent, int gaps) {
	assert(puzzle.size == state.puzzle.size());
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
	if(f > depth)
	{
		return f;
	}
	int trans_cost;
	int min = -1;
	int temp = 0;
	int ggaps = 0;
	int h;
	bool set = false;
	for(int i = 2; i <= int(puzzle.size); i++)
	{
		ggaps = gaps;
		if(i == parent)
		{
			continue;
		}
		trans_cost = puzzle.GetActionCost(state, i);
		puzzle.ApplyAction2(state, i);
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
		h = ggaps + gap_decreasing_moves_no_inv_l_FG2(state, ggaps);
		if(h - ggaps == 2)
		{
			top_counter++;
		}
		counter++;
		temp = Search_L_No_Inverse_FG(state, g + trans_cost, h, depth, i, ggaps);
		if(temp < 0)
		{
			path.push_back(i);
			puzzle.ApplyAction2(state, i);
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

int IDA::Search_Inverse_FG(PancakeState& state, int g, int heur, int depth, int parent, int gaps) {
	assert(puzzle.size == state.puzzle.size());
	if(puzzle.GoalTest(state))
	{
		return -g;
	}
	int f = g + heur;
	if(f > depth)
	{
		return f;
	}
	int trans_cost;
	int min = -1;
	int temp = 0;
	int ggaps = 0;
	int h;
	bool set = false;
	for(int i = 2; i <= int(puzzle.size); i++)
	{
		ggaps = gaps;
		if(i == parent)
		{
			continue;
		}
		trans_cost = puzzle.GetActionCost(state, i);
		puzzle.ApplyAction(state, i);
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
		counter++;
		temp = Search_Inverse_FG(state, g + trans_cost, h, depth, i, ggaps);
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

int IDA::Solve_Gap(PancakeState& state) {
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
	t1=clock();
	while (1)
	{
		temp = Search_Gap(state, 0, h, depth, -1, gaps/*t_path*/, t1);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
	}
}

int IDA::Solve_Gap_EC(PancakeState& state) {
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
	int depth = gaps;
	int h = depth;
	int temp = 0;
	clock_t t1;
	t1 = clock();
	while (1)
	{
		temp = Search_Gap_EC(state, 0, h, depth, -1, gaps/*t_path*/, t1);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
	}
}

int IDA::Solve_Inverse(PancakeState& state) {
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
	int depth = gaps + TopHeurWithInverseAndL(state, gaps);
	int h = depth;
	int temp = 0;
	while (1)
	{
		temp = Search_Inverse(state, 0, h, depth, -1, gaps /*t_path*/);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
	}
}

int IDA::Solve_Inverse_EC(PancakeState& state) {
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
	int gaps = GapHeur(state);
	int depth = gaps + TopHeurWithInverseAndL(state, gaps);
	int h = depth;
	int temp = 0;
	clock_t t1;
	t1 = clock();
	while (1)
	{
		temp = Search_Inverse_EC(state, 0, h, depth, -1, gaps /*t_path*/, t1);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
	}
}

int IDA::Solve_Inverse_FG_EC(PancakeState& state) {
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
	int gaps = GapHeur(state);
	int depth = gaps + TopHeurWithInverseAndLFG(state, gaps);
	int h = depth;
	int temp = 0;
	clock_t t1;
	t1 = clock();
	while (1)
	{
		temp = Search_Inverse_FG_EC(state, 0, h, depth, -1, gaps /*t_path*/, t1);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
	}
}

int IDA::Solve_Top_No_Inverse_EC(PancakeState& state) {
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
	int gaps = GapHeur(state);
	int depth = gaps + TopHeur(state, gaps);
	int h = depth;
	int temp = 0;
	clock_t t1;
	t1 = clock();
	while (1)
	{
		temp = Search_Top_No_Inverse_EC(state, 0, h, depth, -1, gaps /*t_path*/, t1);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
	}
}

int IDA::Solve_L_No_Inverse_EC(PancakeState& state) {
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
	int gaps = GapHeur(state);
	int depth = gaps + gap_decreasing_moves_no_inv_l(state, gaps);
	int h = depth;
	int temp = 0;
	clock_t t1;
	t1 = clock();
	while (1)
	{
		temp = Search_L_No_Inverse_EC(state, 0, h, depth, -1, gaps /*t_path*/, t1);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
	}
}

int IDA::Solve_L_No_Inverse_FG_EC(PancakeState& state) {
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
	int gaps = GapHeur(state);
	int depth = gaps + gap_decreasing_moves_no_inv_l_FG2(state, gaps);
	int h = depth;
	int temp = 0;
	clock_t t1;
	t1 = clock();
	while (1)
	{
		temp = Search_L_No_Inverse_FG_EC(state, 0, h, depth, -1, gaps /*t_path*/, t1);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
	}
}

int IDA::Solve_Top_With_Inverse_EC(PancakeState& state) {
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
	int gaps = GapHeur(state);
	int depth = gaps + TopHeurWithInverse(state, gaps);
	int h = depth;
	int temp = 0;
	clock_t t1;
	t1 = clock();
	while (1)
	{
		temp = Search_Top_With_Inverse_EC(state, 0, h, depth, -1, gaps /*t_path*/, t1);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
	}
}

int IDA::Solve_Top_With_Inverse(PancakeState& state) {
	assert(puzzle.size == state.puzzle.size());
	
	counter = 0;
	top_counter = 0;
	action_counter=0;
	iterations = 0;
	number_of_solutions = 0;
	flag = 0;
	path.clear();
	if(puzzle.size == 0)
	{
		return 0;
	}
	int gaps = GapHeur(state);
	int depth = gaps + TopHeurWithInverse(state, gaps);
	int h = depth;
	int temp = 0;
	while (1)
	{
		temp = Search_Top_With_Inverse(state, 0, h, depth, -1, gaps/*t_path*/);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
	}
}

int IDA::Solve_Top_No_Inverse(PancakeState& state) {
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
	int depth = gaps + TopHeur(state, GapHeur(state));
	int h = depth;
	int temp = 0;
	while (1)
	{
		temp = Search_Top_No_Inverse(state, 0, h, depth, -1, gaps /*t_path*/);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
	}
}

int IDA::Solve_L_No_Inverse(PancakeState& state) {
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
	int depth = gaps + gap_decreasing_moves_no_inv_l(state, gaps);
	int h = depth;
	int temp = 0;
	while (1)
	{
		temp = Search_L_No_Inverse(state, 0, h, depth, -1, gaps /*t_path*/);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
	}
}

int IDA::Solve_L_No_Inverse_FG(PancakeState& state) {
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
	int gaps = GapHeur(state);
	int depth = gaps + gap_decreasing_moves_no_inv_l_FG2(state, gaps);
	int h = depth;
	int temp = 0;
	while (1)
	{
		temp = Search_L_No_Inverse_FG(state, 0, h, depth, -1, gaps /*t_path*/);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
	}
}

int IDA::Solve_Inverse_FG(PancakeState& state) {
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
	int gaps = GapHeur(state);
	int depth = gaps + TopHeurWithInverseAndLFG(state, gaps);
	int h = depth;
	int temp = 0;
	while (1)
	{
		temp = Search_Inverse_FG(state, 0, h, depth, -1, gaps /*t_path*/);
		if(temp <= 0 || flag != 0)
		{
			return abs(temp);
		}
		depth = temp;
		iterations++;
	}
}
