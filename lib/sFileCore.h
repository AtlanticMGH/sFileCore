#ifndef SFILECORE_H
#define SFILECORE_H

char *readFile(char fileName[], int stringSize);
float readValue(char fileName[], char key[]);
void overWriteFile(char fileName[], char fileContent[]);
void appendFile(char fileName[], char key[], float value);
void deleteEntry(char fileName[], char key[]);
void updateEntry(char fileName[], char key[], float value);

#endif
