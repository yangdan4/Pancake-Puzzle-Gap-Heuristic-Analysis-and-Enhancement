#ifndef INPUT_UTILS
#define INPUT_UTILS

#include <vector>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <fstream>

/** Splits a string into elements. The delimiter is selected by
	the user.
 **/
std::vector<std::string> split(const std::string &s, char delim);

/**
 * Reads in permutations from a given file. 
 */
bool read_in_permutations(const char *filename, 
    std::vector<std::vector<int> > &permutations);

/**
 * Checks to make sure a given list of integers is a valid
 * permutation.
 */
bool check_permutation(const std::vector<int> &to_check);

std::string int_to_string(int i);

#endif
