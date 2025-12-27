# sFileCore

A lightweight C library designed for storing and loading numerical data structures. **sFileCore** utilizes a minimalist, line-based format that bridges the gap between the readability of JSON and the simplicity of flat key-value lists.

---

## The Format

The library is optimized to handle numerical values with minimal overhead. While files typically use the **`.s`** extension, the library is flexible and supports any file naming convention.

### Syntax Rules:
- Each entry must be on a new line.
- Key and Value are separated by a colon (`:`).
- Every line **must** end with a comma (`,`).
- A newline character (`\n`) terminates the entry.

**Example of a `.s` file:**
```text
SCALE: 90,
FPS: 60,
VOLUME: 0.85,
DIFFICULTY: 3,
```
## Usage
The library can be used for excample simple settings, like storing the screen scale, the FPS, the volume, ...

### Code Excample
```code
#include <sFileCore.h>

int main(){
  appendFile("settings.s", "SCALE", 90);

  ...
  int scale = readValue("settings.s", "SCALE");

  screen.create(16*scale, 9*scale);

  return 0;
}
```

### List of Commands
| Syntax | Description |
|--------|-------------|
| void overWriteFile(char fileName[], char fileContent[]) | used to overwrite complete file |
| void appendFile(char fileName[], char key[], float value) | used to add an entry to the end of the file, with key as the identifier|
| void deleteEntry(char fileName[], char key[]) | deletes entry from the file, with key as the identifier | 
| void updateEntry(char fileName[], char key[], float value) | changes entry, with key as the identifiert |
| char *readFile(char fileName[], int stringSize) | return the whole file content as a string, stringSize is the amount of chars in the file |
| float readValue(char fileName[], char key[]) | return the value from an entry, using key as the identifier | 
