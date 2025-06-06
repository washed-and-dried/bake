#include "parser.cpp"
#include "util.cpp"
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <stdio.h>
#include <unistd.h>
#include <vector>

using std::map;

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

    // if (strcmp(argv[1], "bake") != 0) {
    //     printf("Incorrect arguments\n");
    //     print_help();
    //     exit(1);
    // }

    printf("holy fuck man wtf was all that");
    printf(", also why is read_file somehow broken");
    printf("wait why tf is there a printf here");
    printf("why TF am i writing printfs");
    printf("fucking backslash n\n");
    printf("holy fucking airball\n");

    std::ifstream file("Bakefile");
    if (!file) {
        std::cerr << "couldnt open file" << std::endl;
        return -1;
    }

    Parser parser(file, "");
    std::map<std::string, std::vector<content>> bakefile = parser.parse();

    // execute
}
