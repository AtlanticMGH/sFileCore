#ifndef WRITEFILE_H
#define WRITEFILE_H

void overWriteFile(char fileName[], char fileContent[]);
void appendFile(char fileName[], char key[], float value);
void deleteEntry(char fileName[], char key[]);
void updateEntry(char fileName[], char key[], float value);

#endif
