#include "util.cpp"
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <unistd.h>

using namespace std;

void print_help(){
    printf("HELP TEXT");
}

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

    // ...parse file here and execute target

    free(file_content);
}
