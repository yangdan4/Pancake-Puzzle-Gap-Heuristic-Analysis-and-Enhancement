#include "PancakePuzzle.h"
#include <cassert>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <cmath>

void PancakeState::set_g(double new_g)
{
	g = new_g;
}

double PancakeState::get_g() const
{
	return g;
}

void PancakeState::set_h(double new_h)
{
	h = new_h;
}

double PancakeState::get_h() const
{
	return h;
}

void PancakeState::set_action(int new_action)
{
	action = new_action;
}

int PancakeState::get_action() const
{
	return action;
}

void PancakeState::set_parent(int new_parent)
{
	parent = new_parent;
}
	
int PancakeState::get_parent() const
{
	return parent;
}

void PancakeState::set_position(int new_position)
{
	position = new_position;
}
	
int PancakeState::get_position() const
{
	return position;
}

double PancakeState::get_weighted_h()
{
	return weighted_h;
}
	
void PancakeState::set_weighted_h(double value)
{
	weighted_h = value;
}

PancakePuzzle::PancakePuzzle(unsigned num_pancakes, costType type) 
        : size(num_pancakes), cost_type(type){
}

PancakePuzzle::~PancakePuzzle() {
}

void PancakePuzzle::GetActions(const PancakeState& state, std::vector<unsigned>& actions) const {
    assert(size == state.puzzle.size());
    actions.clear();
    for(unsigned i = 1; i <= size; i++) {
        actions.push_back(i);
    }
}

double PancakePuzzle::GetActionCost(const PancakeState& state, unsigned action) const {
    assert(size == state.puzzle.size());
    assert(action <= size);
    
    if(cost_type == heavy) {
        double cost = 0.0;
        
        for(unsigned i = 0; i < action; i++) {
            cost += (i+1);
        }
        return cost;
    }
    
    return 1.0;
        
}

void PancakePuzzle::ApplyAction(PancakeState& state, unsigned action) const {
    assert(size == state.puzzle.size());
    assert(action >= 1 && action <= size);
    
    unsigned low_index = 0;
    unsigned high_index = action - 1;
    unsigned temp;
    
    while(low_index < high_index) {
        temp = state.puzzle[low_index];
        state.puzzle[low_index] = state.puzzle[high_index];
        state.inverse[state.puzzle[high_index]] = low_index;
        state.puzzle[high_index] = temp;
        state.inverse[temp] = high_index;
        low_index++;
        high_index--;
    }
}

void PancakePuzzle::ApplyAction2(PancakeState& state, unsigned action) const {
    assert(size == state.puzzle.size());
    assert(action >= 1 && action <= size);
    
    unsigned low_index = 0;
    unsigned high_index = action - 1;
    unsigned temp;
    
    while(low_index < high_index) {
        temp = state.puzzle[low_index];
        state.puzzle[low_index] = state.puzzle[high_index];
        state.puzzle[high_index] = temp;
        low_index++;
        high_index--;
    }
}

bool PancakePuzzle::GoalTest(const PancakeState& state) const {
    assert(size == state.puzzle.size());
    
    for(int i = 0; i < int(size); i++) {
        if(state.puzzle[i] != i)
            return false;
    }
    
    return true;
}

PancakeState PancakePuzzle::GetRandomState(unsigned size) {
    std::vector<int> permutation;

    for(unsigned i = 0; i < size; i++)
        permutation.push_back(i);
    
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

    PancakeState s(size);
    for(unsigned i = 0; i < size; i++) {
        s.puzzle[i] = permutation[i];
        s.inverse[permutation[i]] = i;
    }
    
    return s;
}
