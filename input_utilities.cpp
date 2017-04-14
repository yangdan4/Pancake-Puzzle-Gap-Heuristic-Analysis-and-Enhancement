#include "input_utilities.h"

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

bool read_in_permutations(const char *filename, std::vector<std::vector<int> > &permutations) {

    std::ifstream ifs(filename, std::ios::in);

    if(ifs.fail()) {
        fprintf(stderr, "File Reading Failed\n");
        return false;
    }

	std::string s, temp;

	std::vector<int> puzz_ints;

    while(!ifs.eof()) {
        puzz_ints.clear();

        getline(ifs, s);
        // indicates are starting new permutation
        temp.clear();
        
        // scans through line character by character
        for(unsigned int i = 0; i < s.length(); i++) {
            if(s.at(i) == ' ' || s.at(i) == '\t') {
                if(temp.length() > 0) {
                    puzz_ints.push_back(atoi(temp.c_str()));
                    temp.clear();
                }
            } else {
                temp.push_back(s.at(i));
            }
        }

        // Gets last int
        if(temp.length() > 0) {
            puzz_ints.push_back(atoi(temp.c_str()));
            temp = "";
        }

        // ensure is proper permutation, otherwise don't add it to the list
        if(check_permutation(puzz_ints)) {
            permutations.push_back(puzz_ints);
        }
    }

    ifs.close();

    return true;
}

bool check_permutation(const std::vector<int> &to_check) {
    unsigned size = to_check.size();

    std::vector<bool> in_puzzle(size);

    for(unsigned i = 0; i < size; i++) {
        in_puzzle[i] = false;
    }

    for(unsigned i = 0; i < size; i++) {
        in_puzzle[to_check[i]] = true;
    }

    for(unsigned i = 0; i < size; i++) {
        if(!in_puzzle[i])
            return false;
    }

    return true;
}
