#ifndef PANCAKEPUZZLEDOMAIN_H
#define PANCAKEPUZZLEDOMAIN_H

#include <vector>
#include <iostream>
#include <sstream>

/**
 * A class for pancake puzzle states.
 */
class PancakeState {
public:
	PancakeState() { puzzle.clear(); inverse.clear(); action = 0; g = 0; h = -1; parent = -1; position = -1;}

	PancakeState(unsigned int puzzle_size) {
		puzzle.resize(puzzle_size);
		inverse.resize(puzzle_size);
		for (unsigned int x = 0; x < puzzle.size(); x++)
		{
			puzzle[x] = x;
			inverse[x] = x;
		}
		action = 0; g = 0; h = -1;
		parent = -1;
		position = -1;
	}
	
	std::vector<int> puzzle;
	
	std::vector<int> inverse;
	
	void set_g(double new_g);
	
	double get_g() const;
	
	void set_h(double new_h);
	
	double get_h() const;
	
	void set_action(int new_action);
	
	int get_action() const;
	
	void set_parent(int new_parent);
	
	int get_parent() const;
	
	void set_position(int new_position);
	
	int get_position() const;
	
	double get_weighted_h();
	
	void set_weighted_h(double value);
	
	bool operator== (const PancakeState &s2) const
	{
		for(unsigned i = 0; i < puzzle.size(); i++)
		{
			if(puzzle[i] != s2.puzzle[i])
			{
				return false;
			}
		}
		return true;
	}
	
	bool operator< (const PancakeState &s2) const
	{
		for(unsigned i = 0; i < puzzle.size(); i++)
		{
			if(puzzle[i] != s2.puzzle[i])
			{
				return true;
			}
		}
		return false;
	}
	
private:

	double g;
	
	double h;
	
	int action;
	
	int parent;
	
	
	
	int position;
	
	double weighted_h;
};

static std::ostream& operator <<(std::ostream & out, const PancakeState &state)
{
	for (unsigned int x = 0; x < state.puzzle.size(); x++)
		out << state.puzzle[x] << " ";
	return out;
}
 
/*
 * Enumeration for different cost types.
 */
enum costType {
	unit,
	heavy
};

/**
 * Defines a domain for the pancake puzzle
 */
class PancakePuzzle {
public:
    // Constructor takes in number of pancakes and the cost type (unit by default)
    PancakePuzzle(unsigned num_pancakes, costType type=unit);
    
    ~PancakePuzzle();

    // Tests if the given state is a goal
    bool GoalTest(const PancakeState &state) const;
    
    // Gets the actions applicable in a given state
    void GetActions(const PancakeState &state, std::vector<unsigned> &actions) const;
    
    // Gets the cost of applying an action in a given state
    double GetActionCost(const PancakeState &state, unsigned action) const;
    
    // Applies the action to the given state
    void ApplyAction(PancakeState &state, unsigned action) const;
    
    void ApplyAction2(PancakeState &state, unsigned action) const;
    
    // Returns a random state
    static PancakeState GetRandomState(unsigned size);
    
    
    unsigned size;
    costType cost_type;
};

#endif // PANCAKEPUZZLEDOMAIN_H
