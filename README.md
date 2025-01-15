# AVL tree

## Introduction

This project is an implementation of the matrix with determinant counting, addition and multiplication, and transpose.

## Build and Run

Cloning repository:
```
git clone https://github.com/ask0later/AVL-tree
```

If you want to build the project, write this in the project directory:
```
cmake -S . -B build
cmake --build build
```

After that, you can run main target program:

```
./build/src/main
```

## Tests
### Unit

If you want to run unit tests, generate Makefiles with the WITH_TESTS flag:
```
cmake [...] -DWITH_TESTS=1
```

Then build `tests` target:
```
cmake --build build --target tests
```

After that, run:
```
./build/tests/tests
```

### End to end

If you want to run end-to-end tests, type it:
```
python3 tests/check_end_to_end.py
```
