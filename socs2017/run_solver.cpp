#include <stdio.h>
#include "PancakePuzzle.h"
#include "IDA.h"
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
#include <chrono>
#include <cassert>
#include <sstream>

using namespace std;

int main(int argc, char **argv) {

    srand ( time(NULL) );

    unsigned num_pancakes = 16;
    PancakePuzzle env(num_pancakes);
    IDA search(env, 1, false, false);

    string filename = "../problem_sets/random_permutations/16_pancake_rand_perms";
    string output_filename = "../results/16_pancake_rand_perms_results";

    vector<vector<int> > puzzles;
    read_in_permutations(filename.c_str(), puzzles);

    ofstream results_stream;
    results_stream.open(output_filename.c_str(), std::ofstream::out | std::ofstream::trunc);

    chrono::time_point<std::chrono::system_clock> start, end, init;
    chrono::duration<double> elapsed, total_elapsed;

    unsigned num_problems = 1000;
    PancakeState pancake_init(num_pancakes);

    init = chrono::system_clock::now();
    double elapsed_d = 0.0;

    int cost;

    for(unsigned i = 0; i < 1000; i++) {
        assert(num_pancakes == puzzles[i].size());

        // set problem
        for(unsigned j = 0; j < num_pancakes; j++)
            pancake_init.puzzle[j] = puzzles[i][j];

        // set inverse
        for(unsigned j = 0; j < num_pancakes; j++)
            pancake_init.inverse[pancake_init.puzzle[j]] = j;

        start = chrono::system_clock::now(); // start timer

        // runs solver
        // Uncomment to choose which heuristic you want to use
        //cost = search.Solve_Gap_EC(pancake_init); // Gap with early checking
        //cost = search.Solve_Top_No_Inverse_EC(pancake_init); // Lock detection (LD)
        //cost = search.Solve_L_No_Inverse_FG_EC(pancake_init); // Two-level lock detection (2LD)
        //cost = search.Solve_Top_With_Inverse_EC(pancake_init); // Lock detection using the inverse (LD^D)
        cost = search.Solve_Inverse_FG_EC(pancake_init); // Two-level lock detection using the inverse (2LD^D)

        end = chrono::system_clock::now(); // end timer

        elapsed = end - start;
        total_elapsed = end - init;
        elapsed_d = elapsed.count();

        // Truncates time information
        if(elapsed_d < 0.0001)
            elapsed_d = 0.0001;

        // Outputs to the results file in the following form
        // ProblemNum \t ActionsChecked \t LookaheadCalls \t ActionsApplied \t Runtime \t OptimalCost \t GapHeuristicValue
        // ActionsChecked is like nodes generated, while ActionsApplied is like node expansions
        results_stream << i << "\t" << search.GetCounter();
        results_stream << "\t" << search.GetTopCounter();
        results_stream << "\t" << search.GetActionCounter();
        results_stream << "\t" << elapsed_d << "\t" << cost;
        results_stream << "\t" << search.GapHeur(pancake_init) << endl;

        cerr << i << "\t" << elapsed_d << "\t" << total_elapsed.count() << endl;
    }

    results_stream.close();

    return 0;
}
