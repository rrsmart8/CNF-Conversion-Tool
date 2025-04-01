## Overview
This project is a **CNF Conversion Tool** that reads gate definitions from an input file and outputs a **Conjunctive Normal Form (CNF)** representation of the circuit. The tool handles **AND**, **OR**, and **NOT** gates, and generates the equivalent CNF formulas to be used in SAT solvers. The tool also includes a shell script to verify the output using **CaDiCaL**, a SAT solver.

## Files

- **main.c**: The main C source file that includes the implementation of reading gate inputs, generating CNF formulas, and writing them to an output file.
- **checker/check.sh**: A shell script that compiles the source, runs the tool on several test cases, and uses **CaDiCaL** to check the correctness of the output.

## Features

- **Gate Handling**: The tool supports **AND**, **OR**, and **NOT** gates.
- **CNF Conversion**: Converts logic gate definitions into CNF format for use in SAT solvers.
- **Verification**: The checker script verifies the CNF using **CaDiCaL**.
- **Error Handling**: Allocates and deallocates memory properly to prevent memory leaks.

## How to Build and Run

### Prerequisites

- **GCC** or any compatible C compiler.
- **Make** for build automation.
- **CaDiCaL SAT Solver** (included in the `checker` directory).

### Building the Project

To build the project, navigate to the `src` directory and run:

```sh
make build
```

This command will compile the source code and generate an executable.

### Running the Tool

To run the CNF conversion tool:

```sh
./main <input_file> <output_file>
```

- `<input_file>`: File containing gate definitions.
- `<output_file>`: File to write the CNF output.

### Running Tests

To run the tests provided in the `checker` directory, execute the following command in the root of the project:

```sh
./checker/check.sh
```

This script runs the executable on a set of predefined test cases and compares the results against reference outputs using **CaDiCaL**.

## Input Format

The input file should start with two numbers:

1. **Number of Inputs**: The number of input variables.
2. **Last Gate Number**: The highest gate number defined.

Each subsequent line should define a gate operation:

- **N X Y**: Represents a **NOT** gate with input `X` and output `Y`.
- **O X1 X2 ... Xn Y**: Represents an **OR** gate with inputs `X1, X2, ..., Xn` and output `Y`.
- **A X1 X2 ... Xn Y**: Represents an **AND** gate with inputs `X1, X2, ..., Xn` and output `Y`.

## Output Format

The output is written in **DIMACS** CNF format:

- The first line follows the convention:

  ```
  p cnf <number_of_variables> <number_of_clauses>
  ```

- Each subsequent line contains a clause followed by `0`.
- The last gate is written as an assertion.

## Example

### Input
```
3
4
N 1 4
A 1 2 3 5
O 4 5 6
```

### Output
```
p cnf 6 4
-1 -4 0
1 4 0
-1 -2 -3 5 0
-4 5 0
```

## Checker Script Details

The **checker/check.sh** script is used to run automated tests to validate the output. It consists of the following steps:

1. **Compilation**: Compiles the source code using `make build`.
2. **Execution**: Runs the executable against the input files located in `checker/in/`.
3. **Verification**: Uses **CaDiCaL** to verify if the output is satisfiable or unsatisfiable and compares it with reference outputs (`.ref` files).

The script outputs the result of each test (`PASS`, `FAIL`, `TLE` for timeout) and calculates the total score.

## Memory Management

The program ensures proper **memory allocation and deallocation**:

- The `gate_list` is allocated dynamically to hold gate definitions.
- Functions like `destroy_gate_list()` and `free()` are used to release the allocated memory to prevent memory leaks.

## Limitations and Future Improvements

- **Gate Limitations**: The number of gates is currently limited to **100** and inputs to **60**. This could be made dynamic based on input requirements.
- **Concurrency**: The current implementation processes gates sequentially. Future versions could introduce parallelism for more efficient handling of larger circuits.

## License

This project is open source and can be freely modified and distributed.

