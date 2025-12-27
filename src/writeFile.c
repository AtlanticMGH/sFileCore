#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "readFile.h"

void checkFile(char fileName[]){
    FILE *f = fopen(fileName, "rb");
    if(f == NULL){
        fprintf(stderr, "\033[1;31mFatal Error: File \"%s\" could not be opened!\033[0m\n", fileName);
        return;
    }
    fclose(f);
}

void overWriteFile(char fileName[], char fileContent[]){
    FILE *f_check = fopen(fileName, "rb");
    if(f_check == NULL){
        fprintf(stderr, "\033[1;31mFatal Error: File \"%s\" could not be opened!\033[0m\n", fileName);
        return;
    }
    fclose(f_check);

    FILE *f = fopen(fileName, "w");
    if(f) {
        fprintf(f, "%s", fileContent);
        fclose(f);
    }
}

void appendFile(char fileName[], char key[], int value){
    FILE *f_size = fopen(fileName, "rb");
    if(f_size == NULL){
        fprintf(stderr, "\033[1;31mFatal Error: File \"%s\" could not be opened!\033[0m\n", fileName);
        return;
    }
    fseek(f_size, 0, SEEK_END);
    int size = ftell(f_size);
    fclose(f_size);

    printf("%d\n", size);

    if(readValue(fileName, key) != -1){
        fprintf(stderr, "\033[1;36mKey \"%s\" already used in \"%s\"! \nCannot be used again!\033[0m\n", key, fileName);
        return;
    }

    FILE *f = fopen(fileName, "a");
    if(f) {
        char appendContent[128];
        snprintf(appendContent, sizeof(appendContent), "%s: %d", key, value);

        char fullAppendContent[132];
        snprintf(fullAppendContent, sizeof(fullAppendContent), "%s,\n", appendContent);

        fprintf(f, "%s", fullAppendContent);
        fclose(f);
    }
}

void deleteEntry(char fileName[], char key[]){
    FILE *f_size = fopen(fileName, "rb");
    if(f_size == NULL){
        fprintf(stderr, "\033[1;31mFatal Error: File \"%s\" could not be opened!\033[0m\n", fileName);
        return;
    }
    fseek(f_size, 0, SEEK_END);
    int size = ftell(f_size);
    fclose(f_size);

    size += 4;
    printf("%d\n", size);

    if(readValue(fileName, key) == -1){
        fprintf(stderr, "\033[1;36mKey \"%s\" not found in \"%s\"! \nCannot be deleted!\033[0m\n", key, fileName);
        return;
    }

    FILE *f = fopen(fileName, "r");
    char *buf = malloc(size+1);
    if(!buf){
        fclose(f);
        return;
    }

    fread(buf, 1, size, f);
    buf[size] = '\0';
    fclose(f);

    size_t klen = strlen(key);
    char *val = buf;

    while((val = strstr(val,key))){
        if((val == buf || *(val - 1)== '\n') && val[klen]==':'){
            break;
        }
        val+=klen;
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
    memmove(val, end, strlen(end)+1);

    f = fopen(fileName, "w");
    if(!f){
        free(buf);
        return;
    }
    fprintf(f, "%s", buf);

    fclose(f);
    free(buf);
}

void updateEntry(char fileName[], char key[], int value){
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
        fprintf(stderr, "\033[1;36mKey \"%s\" not found in \"%s\"! \nCannot be changed!\033[0m\n", key, fileName);
        return;
    }

    char *v_start = strchr(match, ':') + 1;
    while (*v_start == ' ') v_start++;

    char *v_end = strchr(v_start, ',');
    if (!v_end) { free(buf); return; }

    char newValStr[32];
    int newValLen = snprintf(newValStr, sizeof(newValStr), "%d", value);

    memmove(v_start + newValLen, v_end, strlen(v_end) + 1);
    memcpy(v_start, newValStr, newValLen);

    f = fopen(fileName, "w");
    if (!f) { free(buf); return; }
    fputs(buf, f);
    fclose(f);

    free(buf);
}
