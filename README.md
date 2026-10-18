# TensorScript

TensorScript is a domain-specific language (DSL) and compiler for tensor arithmetic and matrix multiplication built using MLIR and LLVM.

## Example Program

```text
a = input(4x4)
b = input(4x4)
c = a + b
d = matmul(c, b)
return d
```

## What Works Now

- **Lexer & Scanner (`src/lexer.cpp`)**: Tokenizes source text, handles comments, identifiers, keywords (`input`, `return`, `matmul`), and shape literals (`4x4`, `128x64`).
- **AST (`include/ast.h`)**: Node hierarchy built with `std::unique_ptr` for clean memory ownership (`VarExprAST`, `BinaryExprAST`, `CallExprAST`, `AssignStmtAST`, `ReturnStmtAST`).
- **Parser (`src/parser.cpp`)**: Hand-written recursive descent parser with operator precedence (`*` before `+`/`-`).
- **Semantic Analysis (`src/sema.cpp`)**: Symbol table tracking shapes, static shape inference, and compile-time error reporting (detects undeclared variables and matmul dimension mismatches like `4x3` vs `4x4`).
- **Build System (`CMakeLists.txt`)**: CMake configuration linking against LLVM and MLIR 14 libraries (`MLIRIR`, `MLIRParser`, `MLIRTensor`, `MLIRLinalg`, `MLIRArithmetic`, `MLIRSupport`).

## Prerequisites

- C++17 compiler (`g++` or `clang++`)
- CMake 3.16+
- LLVM & MLIR 14 development libraries:
  ```bash
  sudo apt-get install -y libmlir-14-dev mlir-14-tools
  ```

## How to Build & Run Tests

```bash
mkdir -p build && cd build
cmake ..
cmake --build .
./tensorscript
```

### Expected Output

```text
--- Test 1: Valid Program ---
SUCCESS: Valid program parsed and verified!

--- Test 2: Intentionally Invalid Matmul (4x3 times 4x4) ---
CORRECTLY CAUGHT ERROR: Matmul inner dimension mismatch: [4x3] vs [4x4]

--- Test 3: Undeclared Variable ---
CORRECTLY CAUGHT ERROR: Use of undeclared variable: 'unknown_var'
```