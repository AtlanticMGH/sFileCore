#ifndef SFILECORE_H
#define SFILECORE_H

char *readFile(char fileName[], int stringSize);
float readValue(char fileName[], char key[]);
void overWriteFile(char fileName[], char fileContent[]);
void appendFile(char fileName[], char key[], float value);
void deleteEntry(char fileName[], char key[]);
void updateEntry(char fileName[], char key[], float value);

#endif

#ifdef SFILECORE_IMPLEMENTATION
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *readFile(char fileName[], int fileSize){
    FILE *f = fopen(fileName, "r");
    if(f == NULL) return NULL;

    char *settings = malloc(fileSize + 1);
    if(!settings) { fclose(f); return NULL; }
    settings[0] = '\0';

    char temporary[32];
    while(fgets(temporary, sizeof temporary, f)){
        strncat(settings, temporary, fileSize - strlen(settings));
    }

    fclose(f);
    return settings;
}

float readValue(char fileName[], char key[]) {
    FILE *f = fopen(fileName, "r");
    if (!f) return -1.0f;

    fseek(f, 0, SEEK_END);
    int fileSize = ftell(f);
    fclose(f);

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

void overWriteFile(char fileName[], char fileContent[]){
    FILE *f_check = fopen(fileName, "rb");
    if(f_check == NULL) return;
    fclose(f_check);

    FILE *f = fopen(fileName, "w");
    if(f) {
        fprintf(f, "%s", fileContent);
        fclose(f);
    }
}

void appendFile(char fileName[], char key[], float value){
    FILE *f_size = fopen(fileName, "rb");
    if(f_size == NULL) return;
    fclose(f_size);

    if(readValue(fileName, key) != -1.0f) return;

    FILE *f = fopen(fileName, "a");
    if(f) {
        fprintf(f, "%s: %f,\n", key, value);
        fclose(f);
    }
}

void deleteEntry(char fileName[], char key[]){
    FILE *f_size = fopen(fileName, "rb");
    if(f_size == NULL) return;
    fseek(f_size, 0, SEEK_END);
    int size = ftell(f_size);
    fclose(f_size);

    if(readValue(fileName, key) == -1.0f) return;

    FILE *f = fopen(fileName, "r");
    char *buf = malloc(size + 1);
    if(!buf){
        fclose(f);
        return;
    }

    fread(buf, 1, size, f);
    buf[size] = '\0';
    fclose(f);

    size_t klen = strlen(key);
    char *val = buf;

    while((val = strstr(val, key))){
        if((val == buf || *(val - 1) == '\n') && val[klen] == ':') break;
        val += klen;
    }

    if(!val){
        free(buf);
        return;
    }
    if (val > buf && *(val - 1) == '\n') val--;

    char *end = strchr(val, ',');
    if(!end) {
        free(buf);
        return;
    }
    end++;
    if (*end == '\n') end++;

    memmove(val, end, strlen(end) + 1);

    f = fopen(fileName, "w");
    if(!f){
        free(buf);
        return;
    }
    fputs(buf, f);

    fclose(f);
    free(buf);
}

void updateEntry(char fileName[], char key[], float value){
    FILE *f = fopen(fileName, "r");
    if (!f) return;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char *buf = malloc(size + 64);
    if (!buf) { fclose(f); return; }

    size_t bytesRead = fread(buf, 1, size, f);
    buf[bytesRead] = '\0';
    fclose(f);

    char *p = buf;
    char *match = NULL;
    size_t klen = strlen(key);

    while ((p = strstr(p, key))) {
        if ((p == buf || *(p - 1) == '\n') && p[klen] == ':') {
            match = p;
            break;
        }
        p += klen;
    }

    if (!match) {
        free(buf);
        return;
    }

    char *v_start = strchr(match, ':') + 1;
    while (*v_start == ' ') v_start++;

    char *v_end = strchr(v_start, ',');
    if (!v_end) { free(buf); return; }

    char newValStr[32];
    int newValLen = snprintf(newValStr, sizeof(newValStr), "%f", value);

    memmove(v_start + newValLen, v_end, strlen(v_end) + 1);
    memcpy(v_start, newValStr, newValLen);

    f = fopen(fileName, "w");
    if (!f) { free(buf); return; }
    fputs(buf, f);
    fclose(f);

    free(buf);
}
#endif
