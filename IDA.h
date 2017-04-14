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
    
    // Gap Heuristic
    int GapHeur(PancakeState& state) const;
    //Top Heuristic
    int TopHeur(PancakeState& state, int gaps) const;
    //OUTDATED!!!!!!!!Top Heuristic with future lock look ahead
    int TopHeur2(PancakeState& state, int inv) const;
    //Taking max of regular and inverse topheur2
    int TopHeurWithInverse(PancakeState& state, int gaps) const;
    //TopHeur3
    int TopHeurWithNoInverse(PancakeState& state, int gaps) const;
    //top with no look ahead
    int TopHeur3(PancakeState& state, int inv) const;
    //Taking max of regular and inverse normal top use in early checking
    int TopHeurWithInverseNoL(PancakeState& state, int gaps, int flip) const;
    //Taking max of regular and inverse normal top use without early checking
    int TopHeurWithInverseNoL2(PancakeState& state, int gaps) const;
    //TopHeur2
    int TopHeurWithLNoInverse(PancakeState& state, int gaps) const;
    
    int TopHeurWithInverseAndLBF(PancakeState& state, int gaps, int flip) const;
    //check if locked
    bool locked_or_not(PancakeState& state) const;
    //check if locked and future locked
    bool locked_or_not2(PancakeState& state) const;
    //return if locked using inverse
    int gap_decreasing_moves(PancakeState& state, int inv) const;
    //return if locked using inverse before flip
    int gap_decreasing_moves2(PancakeState& state, int flip, int inv) const;
    
    int gap_decreasing_moves2bf(PancakeState& state, int ind, int flip, int inv) const;
    
    int gap_decreasing_moves2bf_v2(PancakeState& state, int ind, int flip, int inv) const;
    
    int gap_decreasing_moves2_v2(PancakeState& state, int flip, int inv) const;
    //fill moves with gap decreasing moves
    void gap_decreasing_moves3(PancakeState& state, std::vector<unsigned short int>& moves, int inv) const;
    //fill moves with gap decreasing moves before flip for the inverse, haven't finished implementing yet
    void gap_decreasing_moves4(PancakeState& state, std::vector<unsigned short int>& moves) const;
    
    int gap_decreasing_moves_with_inv_l(PancakeState& state, int inv, int gaps) const;
    
    int gap_decreasing_moves_with_inv_l_FG(PancakeState& state, int inv, int gaps) const;
    
    int gap_decreasing_moves_with_inv_lbf(PancakeState& state, int inv, int gaps, int flip) const;
    
    int gap_decreasing_moves_no_inv_l(PancakeState& state, int gaps) const;
    
    int gap_decreasing_moves_no_inv_l_FG(PancakeState& state, int gaps) const;
    
    int gap_decreasing_moves_no_inv_l_FG2(PancakeState& state, int gaps) const;
    
    int gap_decreasing_moves_no_inv_flip(PancakeState& state, int ind) const;
    
    int gap_decreasing_moves_no_inv_lbf(PancakeState& state, int gaps, int flip) const;
    
    int gap_decreasing_moves_no_inv_lbffg(PancakeState& state, int gaps, int flip) const;
    
    int gap_decreasing_moves_no_inv_flipbf(PancakeState& state, int flip, int ind) const;
    
    int TopHeurWithInverseAndL(PancakeState& state, int gaps) const;
    
    int TopHeurWithInverseAndLFG(PancakeState& state, int gaps) const;
    
    int TopHeurWithInverseAndLFG_bf(PancakeState& state, int gaps, int flip) const;
    
    int gap_decreasing_moves_no_inv(PancakeState& state) const;
    //moves that decrease the top heuristic, doesn't use inverse
    void top_decreasing_moves(PancakeState& state, std::vector<unsigned short int>& moves) const;
    //return the gap increasing moves
    void gap_creating_moves(PancakeState& state, std::vector<unsigned short int>& moves) const;
    //state has one gap decreasing move
    bool one_move_only(PancakeState& state) const;
    //state has two gap decreasing moves
    bool two_moves_only(PancakeState& state) const;
    //state is future locked given flip, uses inverse
    bool future_locked(PancakeState& state, int adj, int gaps, int inv) const;
    
    //IDA* with pruning
    int Search(PancakeState& state, int g, int h, int depth, int parent);
    //IDA* with gaps
    int Search3(PancakeState& state, int g, int h, int depth, int parent, int gaps, clock_t &t1);
    
    int Search3_Prune(PancakeState& state, int g, int h, int depth, int parent, int gaps);
    //IDA* with gaps while keeping around but not using inverse
    int Search3EC_Inverse(PancakeState& state, int g, int h, int depth, int parent, int gaps);
    
    int Search3EC_With_Inverse(PancakeState& state, int g, int h, int depth, int parent, int gaps);
    //IDA* with gaps no inverse and doing early checking
    int Search3_Early(PancakeState& state, int g, int h, int depth, int parent, int gaps, clock_t &t1);
    //IDA* while searching gap decreasing moves first
    int Search_Ordered(PancakeState& state, int g, int h, int depth, int parent, int iterations);
    //IDA* with if top is n then n-flip first
    int Search_Big(PancakeState& state, int g, int h, int depth, int parent);
    //IDA* using top heuristic
    int Search_Top(PancakeState& state, int g, int h, int depth, int parent);
    //IDA* using top with inverse and look ahead
    int Search_Inverse(PancakeState& state, int g, int h, int depth, int parent, int gaps);
    
    int Search_Inverse_EC(PancakeState& state, int g, int h, int depth, int parent, int gaps, clock_t &t1);
    
    int Search_Inverse_FG_EC(PancakeState& state, int g, int h, int depth, int parent, int gaps, clock_t &t1);
    
    int Search_Inverse_FG(PancakeState& state, int g, int heur, int depth, int parent, int gaps);
    
    int Search_Top_No_Inverse_EC(PancakeState& state, int g, int h, int depth, int parent, int gaps, clock_t &t1);
    
    int Search_Top_With_Inverse_EC(PancakeState& state, int g, int h, int depth, int parent, int gaps, clock_t &t1);
    
    int Search_L_No_Inverse_EC(PancakeState& state, int g, int h, int depth, int parent, int gaps, clock_t &t1);
    //IDA* using top with inverse and no look ahead with early checking
    int Search_Inverse_NoL(PancakeState& state, int g, int h, int depth, int parent, int gaps, int flip);
    //IDA* using top with inverse and no look ahead and no early checking
    int Search_Inverse_NoL2(PancakeState& state, int g, int h, int depth, int parent, int gaps);
    //IDA* using top with no inverse
    int Search_Top_No_Inverse(PancakeState& state, int g, int h, int depth, int parent, int gaps);
    //IDA* using top with no inverse and with look ahead
    int Search_L_No_Inverse(PancakeState& state, int g, int h, int depth, int parent, int gaps);
    
    int Search_L_No_Inverse_FG(PancakeState& state, int g, int h, int depth, int parent, int gaps);
    
    int Search_L_No_Inverse_FG_EC(PancakeState& state, int g, int h, int depth, int parent, int gaps, clock_t &t1);
    //IDA* while remembering the history
    int Search2(PancakeState& state, int g, int h, int depth, int parent, std::vector<int>& history);
    //IDA* exploring reverse order of flips
    int Search_Reverse(PancakeState& state, int g, int depth, int parent);
    //IDA* with pruning
    int new_search(PancakeState& state, int g, int heur, int depth, int parent);
    //IDA* with pruning
    int new_search2(PancakeState& state, int g, int heur, int depth, int parent);
    //get state from hash value
    void reverse_hash(PancakeState &s, unsigned int hash);
    //get hash value from state
    unsigned int hash_fn(std::vector<int>& list);
    //flip through solution sequence
    void go_through(PancakeState& state, int heur, std::vector<int>& path);
    //return factorial
    unsigned int factorial(unsigned short n);
    
    int Solve(PancakeState& state);
    
    int Solve2(PancakeState& state);
    
    int Solve3(PancakeState& state);
    
    int Solve3_Prune(PancakeState& state);
    
    int Solve3EC_Inverse(PancakeState& state);
    
    int Solve3EC_With_Inverse(PancakeState& state);
    
    int Solve3_Early(PancakeState& state);
    
    int Solve_Ordered(PancakeState& state);
    
    int Solve_Big(PancakeState& state);
    
    int Solve_Inverse(PancakeState& state);
    
    int Solve_Inverse_FG(PancakeState& state);
    
    int Solve_Inverse_EC(PancakeState& state);
    
    int Solve_Inverse_FG_EC(PancakeState& state);
    
    int Solve_Inverse_NoL(PancakeState& state);
    
    int Solve_Inverse_NoL2(PancakeState& state);
    
    int Solve_Top_No_Inverse(PancakeState& state);
    
    int Solve_L_No_Inverse(PancakeState& state);
    
    int Solve_L_No_Inverse_FG(PancakeState& state);
    
    int Solve_L_No_Inverse_FG_EC(PancakeState& state);
    
    int Solve_Top_No_Inverse_EC(PancakeState& state);
    
    int Solve_Top_With_Inverse_EC(PancakeState& state);
    
    int Solve_L_No_Inverse_EC(PancakeState& state);
    
    int Solve_Top(PancakeState& state);
    
    int new_solve(PancakeState& state);
    
    int new_solve2(PancakeState& state);
    
    int Solve4(PancakeState& state);
    
    int alternate_solve(PancakeState& state);
    
    //setters and getters
    void setThreshold(int new_threshold);
    
    int getThreshold();
    
    uint64_t GetCounter();
    
    uint64_t GetTopCounter();
    
    uint64_t GetActionCounter();
    
    void SetCounter(uint64_t new_counter);
    
    int GetIterations();
    
    
    int GetNumberOfSolutions();
    
    std::vector<int> GetPath();
    
    double GetWeight();
    
    //attributes
    std::vector<int> solution_points;
    
    std::vector<std::vector<int> > paths;
    
    bool found;
    	
    int move;
    
    bool start;
    
    bool has;
    
    int depth_sum;
    
    int heur_sum;
    
    int leaf_sum;
    
    int zero_child;
    
    int one_child;
    
    int two_child;
    
    short unsigned int index;
    
    short unsigned int flip;
    
    //int child_count;
    
    //bool decreasing;
    
    
private:

	PancakePuzzle puzzle;
    
    	uint64_t counter;
    	
    	bool updated;
    	
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
