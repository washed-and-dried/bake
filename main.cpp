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
typedef map<std::string, std::vector<content>> BAKEFILE;
typedef map<std::string, bool> FLAG;

void print_help() { printf("HELP TEXT"); }

bool checkIfDepsModify(const string &target, const vector<content> &content,
                       BAKEFILE& bakefile, FLAG& flags);

void checkOrderOnlyDeps(const string& target, const vector<content>& content, BAKEFILE& bakefile, FLAG& flags);

void execute_target(const string& target, BAKEFILE& bakefile, FLAG& flags);

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
    BAKEFILE bakefile = parser.parse();

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
    FLAG flags;
    vector<string> targets;

    for (int i = 2; i < argc; i++) {
        if (argv[i][0] == '-') {
            flags[string(argv[i])] =  true;
        } else {
            targets.push_back(string(argv[i]));
        }
    }

    for (const auto &target : targets) {
        execute_target(target, bakefile, flags);
    }
}

void execute_target(const string& target, BAKEFILE& bakefile, FLAG& flags) {
        if (bakefile.find(target) == bakefile.end()) {
            printf("No rule to make target '%s'. Stop.",
                    target.c_str());
            exit(1); // FIXME: Makefile just stops execution if target rule doesn't exist, but continues if target has empty recipe
        }

        const vector<content> &content = bakefile[target];

        // FIXME: Does this way of checking the flag introduces some anomalities?
        if (!checkIfDepsModify(target, content, bakefile, flags) && !flags["-B"]) {
            return;
        }

        // FIXME: also, orderonly deps can be normal deps for some other target, i'm assuming that works anyways?
        checkOrderOnlyDeps(target, content, bakefile, flags);

        int lineNo = content.front().startLine + 1;
        for (const auto &command : content.front().recipes) { // FIXME: only getting the first one for now
            int silent = skipSpaces(command, 0);

            const string c = command.substr(silent);
            silent = 0; // reset for matching @
            if (c[silent] == '@') { // silent execution
                silent = skipSpaces(c, silent + 1);

                string command = c.substr(silent);
                handleCommand(command.c_str(), lineNo);
                continue;
            }

            printf("%s\n", c.c_str());
            handleCommand(c.c_str(), lineNo);
            lineNo++;
        }
}

bool checkIfDepsModify(const string &target, const vector<content> &content,
                       BAKEFILE &bakefile, FLAG& flags) {
    // FIXME: considering only the first content

    // printf("checking for: %s\n", target.c_str());
    const vector<string> &deps = content.front().deps;

    const long long target_ts = get_timestamp(target.c_str());
    if (target_ts != -1) {
        return false;
    }

    // if the file with same name as target doesn't exist and it does not have any deps,
    // then it must be executed. This if statement is required because our logic handles
    // everythig in a for loop and it will just fall thourgh to the final return false,
    // if we don't handle empty dep case here
    if (target_ts == -1 && deps.size() == 0) {
        return true;
    }

    for (const auto &dep : deps) {
        const long long dep_ts = get_timestamp(dep.c_str());

        if (dep_ts < 0) {
            // FIXME: consider passing target to `execute_target` for better error printing
            execute_target(dep, bakefile, flags);
            return true; // FIXME: for now returning true in all cases | arsh: there `was` an else, check commit and remove this fixme
        }

        if (target_ts < dep_ts)
            return true;
    }

    return false;
}

void checkOrderOnlyDeps(const string& target, const vector<content>& content, BAKEFILE& bakefile, FLAG& flags) {
    // FIXME: considering only the first content
    const vector<string> &orderOnly = content.front().orderOnlyDeps;

    for (const auto& dep : orderOnly) {
        const long long dep_ts = get_timestamp(dep.c_str());
        if (dep_ts < 0) { // if the file of same name as target doesn't exist we execute the target
            if (bakefile.find(dep) == bakefile.end()) {
                printf("Specified order only dep `%s` for target `%s` does not exist\n",
                       dep.c_str(), target.c_str());
                return;
            } else {
                execute_target(dep, bakefile, flags);
            }
        }
    }
}
