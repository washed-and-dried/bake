#include "util.cpp"
#include <cctype>
#include <iostream>
#include <istream>
#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

struct content {
    vector<string> deps;
    vector<string> orderOnlyDeps;
    vector<string> recipes;
    size_t startLine;
};

class Parser {
    std::istream &in;
    string cliArgs;

  public:
    Parser(std::istream &filein, const string &args)
        : in(filein), cliArgs(args) {}

    map<string, vector<content>> parse() {
        map<string, vector<content>> bakefile{};

        size_t lineCnt = 1; // We read one line before the while loop
        int currLine = -1;

        string line;
        std::getline(in, line);
        int start = 0;
        while (!in.eof()) {
            if (line.size() == 0) {
                if (!in.eof()) {
                    std::getline(in, line);
                    lineCnt++;
                }

                continue;
            }

            int idx = next_word(line, start);
            if (idx == -1) { // exit condition
                return bakefile;
            }

            // the idx would be the index at which the current word ends,
            // and the next word starts. We would match the this idx to ':' or
            // '=' to determine if its a recipe or an variable definition
            if (line[idx] == ':') {
                if (currLine == -1) currLine = lineCnt;

                string target = line.substr(start, idx - start);
                // printf("%s\n", target.c_str());

                idx++; // skip the colon

                idx = skipSpaces(line, idx);

                string deps = line.substr(idx);

                // '|' starts an orderonly deps
                int pipeIdx = deps.find('|');
                string leftDepsStr = deps.substr(0, pipeIdx);
                vector<string> leftDeps = split_words(leftDepsStr);

                string righDepsStr =
                    pipeIdx != -1 ? deps.substr(pipeIdx + 1) : "";

                vector<string> rightDeps = split_words(righDepsStr);

                // printf("%s\n", deps.c_str());
                // printf("%s\n", line.c_str());

                vector<string> commands;
                std::getline(in, line);
                lineCnt++;
                // check if line is the command block or start of new thing
                while (!in.eof()) {
                    if (line.size() == 0) {
                        std::getline(in, line);
                        lineCnt++;
                    }

                    int isStart = next_word(line, 0);
                    if (line[isStart] == ':') {
                        break;
                    }

                    int spaceSkip = skipSpaces(line, isStart);
                    if (line[spaceSkip] == '=') {
                        break;
                    }

                    commands.push_back(line);
                    std::getline(in, line);
                    lineCnt++;
                }

                bakefile[target].push_back({.deps = leftDeps,
                                            .orderOnlyDeps = rightDeps,
                                            .recipes = commands, .startLine = (size_t) currLine});
                currLine = -1;
            } else if (line[idx] ==
                       '=') { // TODO: handle spaces like `VAR = some_value`
                              // otherwise it would brick
                // FIXME: ... parse variable
            }
        }

        return bakefile;
    }

  private:
    // returns the index at which the current word ends and next starts
    int next_word(string &line, const int start) {
        int idx = start;

        for (size_t i = start; i < line.length(); i++) {
            if (line[i] == '\n') {
                return -1;
            }

            if (!std::isalpha(line[i])) {
                return idx;
            }
            idx++;
        }

        return idx;
    }
};
