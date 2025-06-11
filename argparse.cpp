// gracefully stolen and ported to C++ from "git@github.com:bharatm29/parshec.git"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>

using std::vector;
using std::map;
using std::string;

char* resolve_tok(char* tok, bool resolution, map<string, string>& vars);

vector<char*> parse_command(char* command, bool resolution, map<string, string>& vars) {
    vector<char*> ds;

    char* tok = strtok(command, " ");
    ds.push_back(resolve_tok(tok, resolution, vars));

    while((tok = strtok(NULL, " ")) != NULL) {
        ds.push_back(resolve_tok(tok, resolution, vars));
    }

    return ds;
}

char* resolve_tok(char* tok, bool resolution, map<string, string>& vars) {
    if (tok == NULL) {
        printf("%s\n", "Error parsing the command");
    }

    if (tok[0] == '$' && resolution) {
        tok++;
        char* env_val = getenv(tok);
        if (env_val == NULL) {
            // check in manual env map
            const string key(tok);
            if (vars.find(key) != vars.end()) {
                return vars[key].data();
            }

            printf("Could not resolve env var: %s\n", tok);
            return tok;
        }

        return env_val;
    }

    return tok;
}

void set_evar(string key, string val, map<string, string>& vars) {
    vars[key] = val;
}

/*
* What we expect is: [ls, -alh, /dev]
*
* Other examples incldues:
*
* ls -alh $PWD -> running with resolve_evars on we should get -> [ls, -alh, /home/{...user}]
*
* ls -alh $VAR -> we can provide a var to the resolved such as set_evar("VAR", "some_value") -> [ls, -alh, some_value]

==> set_evar("VAR", "some_value", vars);
*/
vector<char*> argparse(char* command, bool resolution, map<string, string>& vars) {
    vector<char*> ds = parse_command(command, resolution, vars);

    // for debugging 🤮
    // printf("[ ");
    // for (size_t i = 0; i < ds.size(); i++) {
    //     printf("%s", ds[i]);
    //     if (i + 1 == ds.size()) { // if its the last element don't print a ,
    //         printf(" ]\n");
    //         break;
    //     }
    //     printf(", ");
    // }

    return ds;
}
