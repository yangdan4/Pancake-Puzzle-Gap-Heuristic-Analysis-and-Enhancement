#include <vector>
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>
#include "PancakePuzzle.h"
#include <map>

class exhaustive_search
{

public:
    
	exhaustive_search(PancakePuzzle& p, unsigned short size);
	
	exhaustive_search(PancakePuzzle& p);
    
	~exhaustive_search();
	
	int gap_decreasing_moves_no_inv(PancakeState& state) const;
    	
    	void search();
    	
    	void search1000();
    	
    	void read_from_file();
    	
    	void read_from_file2();
    	
    	void read_from_file3();
    	
    	void read_from_file4(std::vector<PancakeState>& states);
    	
    	void read_from_file5();
    	
    	void read_from_file6(std::vector<PancakeState>& states);
    	
    	void read_from_file7(std::vector<PancakeState>& states);
    	
    	void write_to_file();
    	
    	void clear_hash(std::vector<unsigned short> &q);
    	
    	unsigned short GapHeur(PancakeState& state) const;
    	
    	int gap_decreasing_moves(PancakeState& state) const;
    	
    	unsigned int factorial(unsigned short n);
    	
    	unsigned int hash_fn(const PancakeState &s);
    	
    	std::map<unsigned short, unsigned int> size_map;
    	
    	std::map<unsigned short, unsigned short> max_map;
    	
    	std::map<unsigned short, unsigned short> min_map;
    	
    	std::map<unsigned short, double> average_map;
    	
    	std::vector<std::map<unsigned int, unsigned short> > difference_maps;
    	
    	std::map<unsigned int, unsigned short> four_map;
    	
    	std::vector<unsigned short> problems_array;
    	
    	std::vector<unsigned short> hash_array;
    	
    	std::vector<unsigned long int> hash_array0;
    	
    	std::vector<unsigned long int> hash_array1;
    	
    	std::vector<unsigned long int> hash_array2;
    	
    	//std::map<unsigned int, std::vector<unsigned short> > heur_map;
    	
    	//std::map<unsigned int, std::vector<unsigned short> > cost_map;
    	
    	//std::map<unsigned int, std::vector<unsigned short> > heur1_map;
    	
    	//std::map<unsigned int, std::vector<unsigned short> > heur2_map;
    	
    	//std::map<unsigned int, std::vector<unsigned short> > heur3_map;
    	
    	std::map<unsigned int, unsigned long int> gaps_map;
    	
    	std::map<unsigned int, unsigned long int> top_map;
    	
    	std::map<unsigned int, unsigned long int> top2_map;
    	
    	std::map<unsigned int, unsigned long int> ltop_map;
    	
    	std::map<unsigned int, unsigned long int> ltop2_map;
    	
    	std::map<unsigned int, unsigned long int> ltopfg_map;
    	
    	std::map<unsigned int, unsigned long int> ltopfg2_map;
    	
    	std::map<unsigned int, unsigned long int> num_map;
    	
    	void reverse_hash(PancakeState &s, unsigned int hash);
    
private:
	
	std::queue<unsigned int> open;
	
	unsigned int counter;
	
	unsigned int pancakes;
	
	unsigned int cost;
	
	unsigned int gaps;
	
	PancakePuzzle puzzle;
};
