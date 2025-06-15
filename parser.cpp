#include <cctype>
#include <cstdio>
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
};

class Parser {
    std::istream &in;
    string cliArgs;

  public:
    Parser(std::istream &filein, const string &args)
        : in(filein), cliArgs(args) {}

    map<string, vector<content>> parse() {
        map<string, vector<content>> bakefile{};

        string line;
        std::getline(in, line);
        int start = 0;
        while (!in.eof()) {
            if (line.size() == 0) {
                if (!in.eof()) {
                    std::getline(in, line);
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
                string target = line.substr(start, idx - start);
                // printf("%s\n", target.c_str());

                idx++; // skip the colon

                idx = this->skipSpaces(line, idx);

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
                // check if line is the command block or start of new thing
                while (!in.eof()) {
                    if (line.size() == 0) {
                        std::getline(in, line);
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
                }

                bakefile[target].push_back({.deps = leftDeps,
                                            .orderOnlyDeps = rightDeps,
                                            .recipes = commands});
            } else if (line[idx] ==
                       '=') { // TODO: handle spaces like `VAR = some_value`
                              // otherwise it would brick
                // FIXME: ... parse variable
            }
        }

        return bakefile;
    }

    int skipSpaces(const string &line, int start) {
        while (isspace(line[start])) {
            start++;
        }

        return start;
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

    vector<string> split_words(string &line) {
        vector<string> words;
        int len = line.size();
        int i = 0;

        while (i < len) {
            while (i < len && isspace(line[i]))
                i++;

            int start = i;

            while (i < len && !isspace(line[i]))
                i++;

            if (start < i) {
                words.push_back(line.substr(start, i - start));
            }
        }

        return words;
    }
};
