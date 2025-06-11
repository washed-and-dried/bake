#include "parser.cpp"
#include "util.cpp"

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <vector>

using namespace std;

void print_help() { printf("HELP TEXT"); }

struct Block {
    vector<char *> dependencies;
    vector<char *> command_block;
};

int main(int argc, const char **argv) {
    if (argc < 2) {
        printf("Not enough arguments\n");
        print_help();
        exit(1);
    }

    if (strcmp(argv[1], "bake") != 0) {
        printf("Incorrect arguments\n");
        print_help();
        exit(1);
    }

    std::ifstream file("Bakefile");
    if (!file) {
        std::cerr << "Error reading Bakefile: " << strerror(errno) << std::endl;
        return -1;
    }

    Parser parser(file, "");
    map<std::string, std::vector<content>> bakefile = parser.parse();

    // execute
}
