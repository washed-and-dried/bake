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
        map<string, vector<content>> map{};

        string line;
        std::getline(in, line);
        while (!in.eof()) {
            // FIXME: everything in while loop
            int idx = next_word(line);
            if (idx == -1) { // exit condition
                return map;
            }

            // the idx would be the index at which the current word ends,
            // and the next word starts. We would match the this idx to ':' or
            // '=' to determine if its a recipe or an variable definition
            if (line[idx] == ':') {
                // FIXME: ... parse recipe
                string target = line.substr();
            } else if (line[idx] == '=') { // TODO: handle spaces like `VAR = some_value` otherwise it would brick
                // FIXME: ... parse variable
            }
        }

        return map;
    }

  private:
    // FIXME: implement
    int next_word(string &line) {
        // returns the index at which the current word ends and next starts
        return -1;
    }
};
