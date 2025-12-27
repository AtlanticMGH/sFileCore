#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *readFile(char fileName[], int fileSize){
    FILE *f = fopen(fileName, "r");

    if(f == NULL) return NULL;

    char *settings = malloc(fileSize);
    char temporary[32];

    while(fgets(temporary, sizeof temporary, f)){
        strncat(settings, temporary, fileSize - strlen(settings) - 1);
    }

    fclose(f);
    return settings;
}

float readValue(char fileName[], char key[], int fileSize) {
    char *s = readFile(fileName, fileSize);
    if (!s) return -1.0f;

    size_t klen = strlen(key);
    char *val = s;

    while ((val = strstr(val, key))) {
        if ((val == s || *(val - 1) == '\n') && val[klen] == ':')
            break;
        val += klen;
    }

    if (!val) {
        free(s);
        return -1.0f;
    }

    char *start = strchr(val, ':');
    if (!start) {
        free(s);
        return -1.0f;
    }
    start++;

    while (*start == ' ') start++;

    char *end = strchr(start, ',');
    if (!end) {
        free(s);
        return -1.0f;
    }

    char buf[64];
    size_t len = end - start;
    if (len >= sizeof(buf)) len = sizeof(buf) - 1;

    memcpy(buf, start, len);
    buf[len] = '\0';

    float num = strtof(buf, NULL);

    free(s);
    return num;
}
