# sFileCore

A lightweight C library for storing and loading numerical data structures.  
**sFileCore** uses a minimalist, line-based file format that sits between the readability of JSON and the simplicity of flat key–value lists.

---

## File Format

The library is optimized for numerical values with minimal overhead.  
Files usually use the **`.s`** extension, but any filename is supported.

### Syntax Rules

- One entry per line
- Key and value are separated by a colon (`:`)
- Every line **must** end with a comma (`,`)
- Each entry is terminated by a newline (`\n`)

### Example `.s` File

```text
SCALE: 90,
FPS: 60,
VOLUME: 0.85,
DIFFICULTY: 3,

```
## Usage
sFileCore is ideal for simple configuration data such as screen scaling, frame rate limits, volume levels, or difficulty settings

### Code Excample (C)
```code
#include "sFileCore.h"

int main() {
    // Add a setting
    appendFile("settings.s", "SCALE", 90);

    // Read the value back
    flaot scale = readValue("settings.s", "SCALE");
    if(scale == getFailValue) return 1;

    // Example usage
    // screen.create(16 * (int)scale, 9 * (int)scale);

    return 0;
}

```
#### When compiling
```
gcc main.c sFileCore.c -o main
```


### List of Commands
| Function | Description |
|--------|-------------|
| void setFailValue(float v) | overwrites sFileCore's fail float (standard: -1.52123674234472f) |
| float getFailValue() | returns fail float |
| void overWriteFile(char fileName[], char fileContent[]) | used to overwrite complete file |
| void appendFile(char fileName[], char key[], float value) | used to add an entry to the end of the file, with key as the identifier|
| void deleteEntry(char fileName[], char key[]) | deletes entry from the file, with key as the identifier | 
| void updateEntry(char fileName[], char key[], float value) | changes entry, with key as the identifiert |
| char *readFile(char fileName[], int stringSize) | return the whole file content as a string, stringSize is the amount of chars in the file |
| float readValue(char fileName[], char key[]) | return the value from an entry, using key as the identifier | 
