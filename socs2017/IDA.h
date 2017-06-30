#include <vector>
#include <iostream>
#include <sstream>
#include <stdint.h>


/**
 * Defines a domain for the pancake puzzle
 */
class IDA {
public:
    
    IDA(PancakePuzzle& p, double weight, bool greater_pruning, bool equal_pruning);
    
    ~IDA();
    //SOLVE_TOP_NO_INVERSE AND EC: TOPHEUR, GAP_DECREASING_MOVES_NO_INV, TOPHEURBF GAP_DECREASING MOVES NO INVERSE FLIP,
    //SOLVE_TOP_WITH_INVERSE AND EC: TOPHEURWITHINVERSE, GAP_DECREASING MOVES, TOPHEURWITHINVERSEBF, GAP_DECREASING MOVES2
    //SOLVE_L_NO_INVERSE AND EC AND FG AND FGEC: TopHeurWithLNoInverse and fg and bf and fgbf GAP DECREASING MOVES NO INVERSE L AND FG2 AND LBF AND LBFFG
    //SOLVE_INVERSE AND FG AND EC AND FGEC: TOPHEURWITHINVERSEANDL, TOPHEURWITHINVERSEANDLBF, gap_decreasing_moves_with_inv_l, gap_decreasing_moves_with_inv_lbf, TOPHEURWITHINVERSEANDLFG, TopHeurWithInverseAndLFG_bf
    //Gap Heuristic
    int GapHeur(PancakeState& state) const;
    //Lock Heuristic
    int TopHeur(PancakeState& state, int gaps) const;
    //Lock Heuristic with early checking
    int TopHeurBF(PancakeState& state, int flip, int gaps) const;
    //Lock' Heuristic
    int TopHeurWithInverse(PancakeState& state, int gaps) const;
    //Lock' Heuristic with early checking
    int TopHeurWithInverseBF(PancakeState& state, int flip, int gaps) const;
    //Llock Heuristic - fg detection
    int TopHeurWithLNoInverse(PancakeState& state, int gaps) const;
    //Llock Heuristic - fg detection with early checking
    int TopHeurWithLNoInverseBF(PancakeState& state, int flip, int gaps) const;
    //Llock Heuristic
    int TopHeurWithLNoInverseFG(PancakeState& state, int gaps) const;
    //Llock Heuristic with early checking
    int TopHeurWithLNoInverseFG_bf(PancakeState& state, int flip, int gaps) const;
    //Llock' Heuristic - fg detection
    int TopHeurWithInverseAndL(PancakeState& state, int gaps) const;
    //Llock' Heuristic - fg detection with early checking
    int TopHeurWithInverseAndLBF(PancakeState& state, int gaps, int flip) const;
    //Llock' Heuristic
    int TopHeurWithInverseAndLFG(PancakeState& state, int gaps) const;
    //Llock' Heuristic with early checking
    int TopHeurWithInverseAndLFG_bf(PancakeState& state, int gaps, int flip) const;
    //helper functions used in the heuristics
    int gap_decreasing_moves(PancakeState& state, int inv) const;
    
    int gap_decreasing_moves2(PancakeState& state, int flip, int inv) const;
    
    int gap_decreasing_moves2bf(PancakeState& state, int ind, int flip, int inv) const;
    
    int gap_decreasing_moves2bf_v2(PancakeState& state, int ind, int flip, int inv) const;
    
    int gap_decreasing_moves2_v2(PancakeState& state, int flip, int inv) const;
    
    int gap_decreasing_moves_with_inv_l(PancakeState& state, int inv, int gaps) const;
    
    int gap_decreasing_moves_with_inv_l_FG(PancakeState& state, int inv, int gaps) const;
    
    int gap_decreasing_moves_with_inv_lbf(PancakeState& state, int inv, int gaps, int flip) const;
    
    int gap_decreasing_moves_no_inv_l(PancakeState& state, int gaps) const;
    
    int gap_decreasing_moves_no_inv_l_FG2(PancakeState& state, int gaps) const;
    
    int gap_decreasing_moves_no_inv_flip(PancakeState& state, int ind) const;
    
    int gap_decreasing_moves_no_inv_lbf(PancakeState& state, int gaps, int flip) const;
    
    int gap_decreasing_moves_no_inv_lbffg(PancakeState& state, int gaps, int flip) const;
    
    int gap_decreasing_moves_no_inv_flipbf(PancakeState& state, int flip, int ind) const;
    
    int gap_decreasing_moves_no_inv(PancakeState& state) const;
    
    //IDA* with gaps
    int Search_Gap(PancakeState& state, int g, int h, int depth, int parent, int gaps, clock_t &t1);
    //IDA* with gaps and early checking
    int Search_Gap_EC(PancakeState& state, int g, int h, int depth, int parent, int gaps, clock_t &t1);
    //IDA* with llock' - fg detection
    int Search_Inverse(PancakeState& state, int g, int h, int depth, int parent, int gaps);
    //IDA* with llock' - fg detection and early checking
    int Search_Inverse_EC(PancakeState& state, int g, int h, int depth, int parent, int gaps, clock_t &t1);
    //IDA* with llock' and early checking
    int Search_Inverse_FG_EC(PancakeState& state, int g, int h, int depth, int parent, int gaps, clock_t &t1);
    //IDA* with llock'
    int Search_Inverse_FG(PancakeState& state, int g, int heur, int depth, int parent, int gaps);
    //IDA* with lock'
    int Search_Top_With_Inverse(PancakeState& state, int g, int h, int depth, int parent, int gaps);
    //IDA* with lock
    int Search_Top_No_Inverse(PancakeState& state, int g, int h, int depth, int parent, int gaps);\
    //IDA* with lock and early checking
    int Search_Top_No_Inverse_EC(PancakeState& state, int g, int h, int depth, int parent, int gaps, clock_t &t1);
    //IDA* with lock' and early checking
    int Search_Top_With_Inverse_EC(PancakeState& state, int g, int h, int depth, int parent, int gaps, clock_t &t1);
    //IDA* with llock - fg detection and early checking
    int Search_L_No_Inverse_EC(PancakeState& state, int g, int h, int depth, int parent, int gaps, clock_t &t1);
    //IDA* with llock - fg detection
    int Search_L_No_Inverse(PancakeState& state, int g, int h, int depth, int parent, int gaps);
    //IDA* with llock
    int Search_L_No_Inverse_FG(PancakeState& state, int g, int h, int depth, int parent, int gaps);
    //IDA* with llock and early checking
    int Search_L_No_Inverse_FG_EC(PancakeState& state, int g, int h, int depth, int parent, int gaps, clock_t &t1);
    //get state from hash value
    void reverse_hash(PancakeState &s, unsigned int hash);
    //get hash value from state
    unsigned int hash_fn(std::vector<int>& list);
    //return factorial
    unsigned int factorial(unsigned short n);
    //Solve with gaps
    int Solve_Gap(PancakeState& state);
    //Solve with gaps and early checking
    int Solve_Gap_EC(PancakeState& state);
    //Solve with llock' - fg detection
    int Solve_Inverse(PancakeState& state);
    //Solve with llock'
    int Solve_Inverse_FG(PancakeState& state);
    //Solve with llock' - fg detection and early checking
    int Solve_Inverse_EC(PancakeState& state);
    //Solve with llock' and early checking
    int Solve_Inverse_FG_EC(PancakeState& state);
    //Solve with lock'
    int Solve_Top_With_Inverse(PancakeState& state);
    //Solve with lock
    int Solve_Top_No_Inverse(PancakeState& state);
    //Solve with lock and early checking
    int Solve_Top_No_Inverse_EC(PancakeState& state);
    //Solve with lock' and early checking
    int Solve_Top_With_Inverse_EC(PancakeState& state);
    //Solve with llock - fg detection
    int Solve_L_No_Inverse(PancakeState& state);
    //Solve with llock
    int Solve_L_No_Inverse_FG(PancakeState& state);
    //Solve with llock and early checking
    int Solve_L_No_Inverse_FG_EC(PancakeState& state);
    //Solve with llock - fg detection and early checking
    int Solve_L_No_Inverse_EC(PancakeState& state);
    //setters and getters
    void setThreshold(int new_threshold);
    
    int getThreshold();
    
    uint64_t GetCounter();
    
    uint64_t GetTopCounter();
    
    uint64_t GetActionCounter();
    
    void InitializeCounters();

    int GetIterations();
    
    int GetNumberOfSolutions();
    
    std::vector<int> GetPath();
    
    double GetWeight();
    
    //attributes
    std::vector<int> solution_points;
    
    std::vector<std::vector<int> > paths;
    	
    int move;
    
    
private:

	PancakePuzzle puzzle;
    
    	uint64_t counter;
    	
    	uint64_t top_counter;
    	
    	uint64_t action_counter;
    	
    	int solution_count;

	std::vector<int> path;

	int iterations;
	
	int number_of_solutions;
	
    	int flag;
    	
    	double weight;
    	
    	bool greater_pruning;
    	
    	bool equal_pruning;
    	
    	int threshold;
};
