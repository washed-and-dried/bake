#include "util.cpp"
#include <cstdlib>
#include <cstring>
#include <map>
#include <stdio.h>
#include <unistd.h>

using std::map;

using namespace std;

void print_help(){
    printf("HELP TEXT");
}

struct Block {
    vector<char*> dependencies;
    vector<char*> command_block;
};

int main(int argc, const char** argv) {
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

    char filename[] = "Bakefile";
    char *file_content = read_file(filename);
    printf("%s", file_content);

    map<char*, Block> bakefile;

    // parse file and execute command

    free(file_content);
}
