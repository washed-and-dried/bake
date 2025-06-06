#include <iostream>
#include <istream>
#include <map>
#include <string>
#include <vector>

struct content {
    std::vector<std::string> deps;
    std::vector<std::string> orderOnlyDeps;
    std::vector<std::string> recipes;
};

class Parser {
    std::istream &in;
    std::string cliArgs;

  public:
    Parser(std::istream &filein, const std::string &args)
        : in(filein), cliArgs(args) {}

    std::map<std::string, std::vector<content>> parse() const {
        std::map<std::string, std::vector<content>> map{};

        std::string line;
        while (std::getline(in, line)) {
            std::cout << line << std::endl;
        }

        return map;
    }

  private:
};
