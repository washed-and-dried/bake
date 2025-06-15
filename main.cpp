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

#define DEBUG 0

void print_help() { printf("HELP TEXT"); }

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

#if DEBUG
    // pretty printing for debugging
    for (const auto &[key, val] : bakefile) {
        printf("target: %s \n", key.c_str());
        printf("deps: \n");
        for (const auto &c : val.front().deps) {
            printf("%s\n", c.c_str());
        }

        printf("order-only-deps: \n");
        for (const auto &c : val.front().orderOnlyDeps) {
            printf("%s\n", c.c_str());
        }

        printf("commaands:\n");
        for (const auto &c : val.front().recipes) {
            printf("%s\n", c.c_str());
        }
    }
#endif // DEBUG

    // execute
    vector<string> targets;
    vector<string> flags;

    for (int i = 2; i < argc; i++) {
        if (argv[i][0] == '-') {
            flags.push_back(string(argv[i]));
        } else {
            targets.push_back(string(argv[i]));
        }
    }

    for (const auto& target : targets) {
        // FIXME: if target doesn't exist?

        const vector<content>& content = bakefile[target];

        for (const auto& c : content.front().recipes) { // FIXME: only getting the first one for now
            int silent = parser.skipSpaces(c, 0);
            if (c[silent] == '@') { // silent execution
                silent = parser.skipSpaces(c, silent + 1);

                string command = c.substr(silent);
                handleCommand(command.c_str());
                continue;
            }

            printf("%s\n", c.c_str());
            handleCommand(c.c_str());
        }
    }
}
