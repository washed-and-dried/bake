#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

void print_help(){
    printf("HELP TEXT");
}

char *read_file(const char *filename) {
    char *retBuff = 0;
    FILE *fp;
    long len;

    fp = fopen(filename, "rb");

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    retBuff = new char(sizeof(char) * (len + 1));
    if (retBuff) {
        fread(retBuff, 1, len, fp);
    }

    fclose(fp);
    retBuff[len] = '\0';
    return retBuff;
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
