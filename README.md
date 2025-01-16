# SIC-XE-Assembler
This project implements a two-pass assembler for the SIC/XE architecture, converting assembly code into machine-readable object code. Pass 1 builds the Symbol Table, calculates addresses, and detects errors. Pass 2 generates object code using the OPTAB and SYMTAB, resolving addresses and formatting instructions for execution.

1. Overview of SIC/XE Assembly Language
SIC/XE is an extended version of the SIC architecture, featuring additional instructions, addressing modes, and registers.
Assembly programs written for SIC/XE use mnemonics (like ADD, LDA, COMP) and operands, which must be translated into machine instructions.
2. Functionality of the Assembler
The assembler operates in two passes to ensure proper translation and error handling:

Pass 1: Symbol Table Construction and Address Assignment
Objective: This phase scans the source code to define labels and calculate memory addresses.
Tasks:
Parse each line to identify labels, instructions, and operands.
Construct a Symbol Table (SYMTAB) to map labels to memory addresses.
Calculate the Location Counter (LOCCTR), which keeps track of the memory address for each instruction or data declaration.
Handle assembly directives (START, END, RESW, BYTE, etc.) for memory allocation.
Detect and report errors like duplicate labels, invalid instructions, or missing starting points.

Pass 2: Object Code Generation
Objective: Translate assembly instructions into object code and handle unresolved symbols.
Tasks:
Use the predefined Operation Table (OPTAB) to map mnemonics to their corresponding opcodes.
Resolve operand addresses using the Symbol Table.
Generate machine instructions in hexadecimal format, including indexed addressing when applicable.
Handle constants (WORD, BYTE) and format object code properly.
Detect and report errors such as unresolved symbols or invalid operands.

3. Object Program Formatting
After generating object code, the assembler organizes it into the standard object program format:
Header Record (H): Contains the program name, starting address, and program length.
Text Records (T): Contain the translated object code along with starting addresses and lengths.
End Record (E): Specifies the execution start address.

This project is essential for understanding how assembly language programs are converted into executable machine code and provides a foundation for learning compilers and machine-level programming.
