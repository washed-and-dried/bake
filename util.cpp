#pragma once

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <string>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
using std::vector;
using std::string;

vector<char *> const parseCommand(const char *const prompt) {
    const size_t n = strlen(prompt);
    char *dup = new char[n];
    memcpy(dup, prompt, n);
    vector<char *> da;
    char *tok = strtok(dup, " ");
    if (strcmp(tok, "exit") == 0) {
        exit(0);
    }

    da.push_back(tok);

    while ((tok = strtok(NULL, " ")) != NULL) {
        da.push_back(tok);
    }

    return da;
}

void handleCommand(const char *const prompt) {
    vector<char *> command = parseCommand(prompt);
    command.push_back(NULL); // execvp requires null terminated list

    const int cpid = fork();

    if (cpid < 0) { // error
        printf("Fork error: %s\n", strerror(errno));
        return;
    }

    if (cpid == 0) { // child
        execvp(command[0], command.data());
        perror("execvp failed");

        // FIXME: write line no at which error occured.
        exit(1);
    } else { // Parent process
        int wstatus;
        waitpid(cpid, &wstatus, 0);

        if (!WIFEXITED(wstatus)) {
            printf("Child process exited abnormally!\n");
        }
    }
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

long long get_timestamp(const char *filename) {
    struct stat sb;

    if (stat(filename, &sb) == -1) {
        return -1; // no such file exists
    }

    return sb.st_mtime;
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

int skipSpaces(const string &line, int start) {
    while (isspace(line[start])) {
        start++;
    }

    return start;
}
