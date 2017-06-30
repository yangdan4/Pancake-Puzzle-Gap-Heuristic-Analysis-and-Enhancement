This repository has code for solving and analyzing the pancake puzzle. The included code was included in the paper "An Analysis and Improvement of the Gap Heuristic for the Pancake Puzzle" by Richard Valenzano and Danniel Sihui Yang. This paper appeared at the 2017 Symposium on Combinatorial Search (SoCS).

### Running the solver
To run the code, create folders "bin" and "obj" in the socs2017 directory, and then call "make" in the socs2017 directory. This will create binaries called "solver_debug" and "solver_release" in the bin folder which can be used to run the IDA*-based solver. The file "run_solver.cpp" can be modified as needed to use the heuristic, problem set, and output file that you want.

### Problem Sets

All the problem sets used in the SoCS 2017 paper have been included in the directory socs2017/problem_sets. For the bootstrapped problems, two sets of problems have been included for each tested pancake size. The files with names ending in "_all" contain all possible combinations of the problems that can be generated from the respective seed sets. The actual 1000 problems per size that were used for testing are contained in the files without this postfix.

### Generating New Problems

Code has also been included for generating self-inverse or problems with an arbitrary number of max_cycles. This can be done with the "generate_cycles.cpp" file which creates targets "generator_debug" and "generator_release."