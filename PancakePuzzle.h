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
	
	/*PancakeState(const PancakeState &obj)
	{
		puzzle.clear();
		std::cout << "nice" << std::endl;
		for(unsigned i = 0; i < obj.puzzle.size(); i++)
		{
			puzzle.push_back(obj.puzzle[i]);
		}
		set_g(obj.get_g());
		set_h(obj.get_h());
		set_action(obj.get_action());
		set_parent(*(obj.get_parent()));
		std::cout << "what" << std::endl;
		if(obj.get_parent() != NULL)
		{
			std::cout << "lala" << std::endl;
			for (unsigned int x = 0; x < (*(obj.get_parent())).puzzle.size(); x++)
				std::cout << (*(obj.get_parent())).puzzle[x] << " ";
		}
		std::cout << std::endl;
		if(get_parent() != NULL)
		{
			for (unsigned int x = 0; x < (*(get_parent())).puzzle.size(); x++)
				std::cout << (*(get_parent())).puzzle[x] << " ";
		}
		std::cout << std::endl;
		std::cout << "no" << std::endl;
		std::cout << "myself" << std::endl;
		for (unsigned int x = 0; x < puzzle.size(); x++)
			std::cout << puzzle[x] << " ";
		std::cout << std::endl;
		std::cout << "end" << std::endl;
		std::cout << "object" << std::endl;
		for (unsigned int x = 0; x < puzzle.size(); x++)
			std::cout << obj.puzzle[x] << " ";
		std::cout << std::endl;
		std::cout << "end object" << std::endl;
		std::cout << "address" << std::endl;
		std::cout << &obj << " ";
		std::cout << std::endl;
		std::cout << "end address" << std::endl;
	}*/
	
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
	
	/*PancakeState& operator=(const PancakeState& obj)
	{
		std::cout << "=" << std::endl;
		std::cout << "what" << std::endl;
		if(obj.get_parent() != NULL)
		{
			for (unsigned int x = 0; x < (*(obj.get_parent())).puzzle.size(); x++)
				std::cout << (*(obj.get_parent())).puzzle[x] << " ";
		}
		std::cout << std::endl;
		std::cout << "no" << std::endl;
		puzzle.clear();
		for(unsigned i = 0; i < obj.puzzle.size(); i++)
		{
			puzzle.push_back(obj.puzzle[i]);
		}
		std::cout << "say" << std::endl;
		if(obj.get_parent() != NULL)
		{
			for (unsigned int x = 0; x < (*(obj.get_parent())).puzzle.size(); x++)
				std::cout << (*(obj.get_parent())).puzzle[x] << " ";
		}
		std::cout << std::endl;
		std::cout << "yo" << std::endl;
		set_g(obj.get_g());
		set_h(obj.get_h());
		set_action(obj.get_action());
		set_parent(*(obj.get_parent()));
		std::cout << "oh" << std::endl;
		if(obj.get_parent() != NULL)
		{
			for (unsigned int x = 0; x < (*(obj.get_parent())).puzzle.size(); x++)
				std::cout << (*(get_parent())).puzzle[x] << " ";
		}
		std::cout << std::endl;
		std::cout << "wow" << std::endl;
		return *this;
	}*/
	
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
    
    //int counter;
    
    //std::vector<int> path;

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
    
    
    // Gap Heuristic
    //int GapHeur(PancakeState& state) const;
    
    //A*
    //int Search(PancakeState& state, int g, int depth, int parent);
    
    //ID
    //int Solve(PancakeState& state);
    
    unsigned size;
    costType cost_type;
};

#endif // PANCAKEPUZZLEDOMAIN_H
