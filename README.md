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
