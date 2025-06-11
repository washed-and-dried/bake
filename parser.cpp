#include <iostream>
#include <istream>
#include <map>
#include <string>
#include <vector>

using std::map;
using std::vector;
using std::string;

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

    map<string, vector<content>> parse() const {
        map<string, vector<content>> map{};

        string line;
        while (std::getline(in, line)) {
            std::cout << line << std::endl;
        }

        return map;
    }

  private:
};
