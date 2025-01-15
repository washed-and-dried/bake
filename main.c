#include <stdio.h>
#include <stdlib.h>

// return is malloc()ed, needs free()ing
char *read_file(char *filename) {
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
    retBuff = malloc(sizeof(char) * (len + 1));
    if (retBuff) {
        fread(retBuff, 1, len, fp);
    }

    fclose(fp);
    retBuff[len] = '\0';
    return retBuff;
}

int main() {
    char *file_content = read_file("Bakefile");
    printf("%s", file_content);

    free(file_content);
}
