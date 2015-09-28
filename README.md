
# CS61C Fall 2013 Project 2: MIPS Instruction Set Emulator

## Goals

We hope this project will enhance your C programming skills, familiarize you with some of the details of MIPS, and prepare you for what's to come later in lecture when we dive into the processor.
Background

In this project, you will create an instruction interpreter for a subset of MIPS code. You'll provide the machinery to decode and execute a couple dozen MIPS instructions. You're creating what is effectively a miniature version of MARS!

The MIPS green sheet provides some information necessary for completing this project.
Getting started

    Note: This is an individual project. Do not show any code to other students, even for debugging.

Make sure you read through the entire project before beginning it.

To obtain the proj2 files, pull from the git repo at ~cs61c/proj/02. For example:

    $  mkdir ~/proj2 
    $  cd ~/proj2
    $  git init
    $  git pull ~cs61c/proj/02 master

The files you will need to modify and submit are:

    part1.c
    part2.c

You should definitely consult through the following, thoroughly:

    types.h: C header file for the data types you will be dealing with.
    Makefile: File which records all dependencies.
    mipscode/*: Various files to run tests.

You should not need to look at these files, but here they are anyway:

    mips.h: C header file for the functions you are implementing.
    mips.c: C source file for the project loader and main function.
    elf.h: C header file for ELF definitions (ELF is the standard executable file format in Linux).

## The MIPS Simulator

The files provided in the start kit comprise a framework for a MIPS simulator. You'll first add code to part1.c to print out the human-readable disassembly corresponding to the instruction's machine code. Next, you'll complete the program by adding code to part2.c to execute each instruction (including perform memory accesses). Your simulator must be able to handle the machine code versions of the following MIPS machine instructions. We've already implemented an example of an R-type, I-type, and J-type instruction.

It is critical that you read and understand the definitions in types.h before starting the project. If they look mysterious, consult chapter 6 of K&R, which covers structs, bitfields, and unions. Check yourself: why does sizeof(Instruction)==4?

    Instruction             Type        OpCode      Funct   Operation
    sll rd, rt, shamt       R           0x00        0x00    R[rd] ← R[rt] << shamt
    srl rd, rt, shamt                               0x02    R[rd] ← R[rt] >> shamt
    sra rd, rt, shamt                               0x03    R[rd] ← (signed)R[rt] >> shamt
    jr rs                                           0x08    PC ← R[rs]
    jalr rd, rs                                     0x09    tmp ← PC + 4
                                                            PC ← R[rs]
                                                            R[rd] ← tmp
    syscall                                         0x0c    (perform system call)
    mfhi rd                                         0x10    R[rd] ← RHI
    mflo rd                                         0x12    R[rd] ← RLO
    mult rs, rt                                     0x18    tmp ← (signed)R[rs] * (signed)R[rt]
                                                            RLO ← {lower 32 bits of tmp}
                                                            RHI ← {upper 32 bits of tmp}
    multu rs, rt                                    0x19    tmp ← R[rs] * R[rt]
                                                            RLO ← {lower 32 bits of tmp}
                                                           RHI ← {upper 32 bits of tmp}
    addu rd, rs, rt                                 0x21    R[rd] ← R[rs] + R[rt]
    subu rd, rs, rt                                 0x23    R[rd] ← R[rs] - R[rt]
    and rd, rs, rt                                  0x24    R[rd] ← R[rs] & R[rt]
    or rd, rs, rt                                   0x25    R[rd] ← R[rs] | R[rt]
    xor rd, rs, rt                                  0x26    R[rd] ← R[rs] ^ R[rt]
    nor rd, rs, rt                                  0x27    R[rd] ← ~(R[rs] | R[rt])
    slt rd, rs, rt                                  0x2a    R[rd] ← (signed)R[rs] < (signed)R[rt]
    sltu rd, rs, rt                                 0x2b    R[rd] ← R[rs] < R[rt]
    j address               J                       0x02    PC ← {(upper 4 bits of PC + 4), address*4}
    jal address                                     0x03    R[31] ← PC + 4
                                                            PC ← {(upper 4 bits of PC + 4), address*4}
    beq rs, rt, offset      I                       0x04    if(R[rs] == R[rt])
                                                            PC ← PC + 4 + SignExt(offset)*4
    bne rs, rt, offset                              0x05    if(R[rs] != R[rt])
                                                            PC ← PC + 4 + SignExt(offset)*4
    addiu rt, rs, imm                               0x09    R[rt] ← R[rs] + SignExt(imm)
    slti rt, rs, imm                                0x0a    R[rt] ← (signed)R[rs] < SignExt(imm)
    sltiu rt, rs, imm                               0x0b    R[rt] ← R[rs] < SignExt(imm)
    andi rt, rs, imm                                0x0c    R[rt] ← R[rs] & ZeroExt(imm)
    ori rt, rs, imm                                 0x0d    R[rt] ← R[rs] | ZeroExt(imm)
    xori rt, rs, imm                                0x0e    R[rt] ← R[rs] ^ ZeroExt(imm)
    lui rt, imm                                     0x0f    R[rt] ← imm << 16
    lb rt, offset(rs)                               0x20    R[rt] ← SignExt(LoadMem(R[rs] + SignExt(offset), byte))
    lh rt, offset(rs)                               0x21    R[rt] ← SignExt(LoadMem(R[rs] + SignExt(offset), half))
    lw rt, offset(rs)                               0x23    R[rt] ← LoadMem(R[rs] + SignExt(offset), word)
    lbu rt, offset(rs)                              0x24    R[rt] ← ZeroExt(LoadMem(R[rs] + SignExt(offset), byte))
    lhu rt, offset(rs)                              0x25    R[rt] ← ZeroExt(LoadMem(R[rs] + SignExt(offset), half))
    sb rt, offset(rs)                               0x28    StoreMem(R[rs] + SignExt(offset), byte, R[rt])
    sh rt, offset(rs)                               0x29    StoreMem(R[rs] + SignExt(offset), half, R[rt])
    sw rt, offset(rs)                               0x2b    StoreMem(R[rs] + SignExt(offset), word, R[rt])

For further reference, here are the bit lengths of the instruction components.

    R-TYPE  opcode  rs  rt  rd  shamt   funct
    Bits    6       5   5   5   5       6

    I-TYPE  opcode  rs  rt  imm
    Bits    6       5   5   16

    J-TYPE  opcode  addr
    Bits    6       26

The MIPS system you're implementing is little-endian. Look at P&H (4th edition) page B-43 for further information on endianness (byte order). We chose little endian because the host machines are little endian, and this avoids some complexity for sub-word loads and stores.

In our variant of MIPS, beq and bne are not delayed branches (if you don't know what that means, just ignore it; your default assumption about the behavior of branches is probably correct for this project; we'll talk about this topic later in the semester).

Initially, you will be able to run a program called simple, which only uses the instructions we implemented for you, by running the following commands:

    $ make
    $ ./mips mipscode/simple
    Hello, world!
    exiting the simulator

## The Framework Code

The framework code we've provided operates by doing the following.
1. It reads the executable program binary, whose filename is specified on the command line, into the simulated memory, starting at address 0x00001000.
2. It initializes all 32 MIPS registers to 0 and sets the program counter (PC) to 0x00001000.
3.  It sets flags that govern how the program interacts with the user. Depending on the options specified on the command line, the simulator will either show a dissassembly dump of the program on the command line, or it will execute the program.

It then enters the main simulation loop, which simply executes a single instruction repeatedly until the simulation is complete. Executing an instruction performs the following tasks:

1. It fetches an instruction from memory, using the PC as the address.
2. It examines the opcode to determine what instruction was fetched.
3. It executes the instruction and updates the PC.

The framework supports a handful of command-line options:
- -i runs the simulator in interactive mode, in which the simulator executes an instruction each time the Enter key is pressed. The disassembly of each executed instruction is printed.
- -t runs the simulator in tracing mode, in which each instruction executed is printed.
- -r instructs the simulator to print the contents of all 32 registers after each instruction is executed. It's most useful when combined with the -i flag.
- -d instructs the simulator to disassemble the entire program, then quit.

Your job is to complete the implementations of the decode_instruction() function in part1.c, and the execute_instruction(), check(), store(), load() functions in part2.c. By the time you're finished, they should handle all of the instructions in the table above.

## Part 1 (Due 10/06)
Your first task is to implement the disassembler by completing the decode_instruction() method in part1.c
1. Print the instruction name. If the instruction has arguments, print a tab (\t).
2. Print all arguments, following the order and formatting given in the INSTRUCTION column of the table above.
    - Arguments are generally comma-separated (lw/sw, however, also use parentheses), but are not separated by spaces.
    - Register arguments are printed as a $ followed by the register number, in decimal (e.g. $0 or $31).
    - Zero-extended immedates (e.g. for ori) are printed as lowercase hexadecimal numbers with a leading 0x but without extra leading zeros (e.g. 0x0 or 0xffff).
    - Jump addresses should be printed like zero-extended immediates, but multiplied by 4 first. (Assume the upper 4 bits of PC+4 are zero.)
    - Sign-extended immediates (e.g. for addiu, sw, or beq) are printed as signed decimal numbers (e.g. -12 or 48). For branch offsets, multiply the offset by 4.
    - Shift amounts (e.g. for sll) are printed as unsigned decimal numbers (e.g. 0 to 31).
3. Print a newline (\n) at the end of an instruction.
4. We will be using an autograder to grade this task. If your output differs from ours due to formatting errors, you will not receive credit.
5. We have provided a disassembly test for you. Since a test is only covering a subset of possible scenarios, however, passing this test does not mean that your code is bug free. You should identify the corner cases and test them yourself.

Run the disassembly test by typing in the following command:

    $ make disasmtest
    gcc -g -Wall -Werror -Wfatal-errors -O2 -o mips part1.c part2.c mips.c
    ./mips -d mipscode/insts > insts.dump
    DISASSEMBLY TEST PASSED!

## Part 2 (Due 10/13)
Your second task is to complete the simulator by implementing the execute_instruction(), check(), store(), and load() methods in part2.c

- execute_instruction() - grabs the next instruction from the address stored in the PC and simulates running it, updating the appropriate registers and making any necessary calls to memory.
- check() - takes the address of a memory access and the size of the value being returned and returns 1 if it is a valid access or 0 if not (explained in the next paragraph).
- store() - takes an address, a size, and a value and stores the first -size- bytes of the given value at the given address.
- load() - takes an address and a size and returns the next -size- bytes starting at the given address

The simulator should abort if a memory access is improperly aligned or outside of the range [1,MEM_SPACE). The same behavior should occur for misaligned loads and stores. lw and sw require 4-byte alignment, like the PC. lb, lbu, and sb are always properly aligned. For misaligned or out-of-range addresses (this includes the NULL address 0), you must have check() return 0 in part2.c, which will cause the simulator to print an error message and abort. This is analogous to the segmentation fault you may see when an x86 program does an illegal memory access.

mult and multu should split the upper and lower 32 bits of the 64 bit multiplication result.

We have provided a simple self-checking assembly test that tests several of the instructions. However, the test is not exhaustive and does not exercise every instruction. Here's how to run the test (the output is from a working processor).

    $ make runtest
    ./mips -r mipscode/insts > test.trace
    insts_test PASSED
    ./mips -r mipscode/rt3 > test.trace
    rt3_test PASSED
    ./mips -r mipscode/rt13 > test.trace
    rt13_test PASSED
    ./mips -r mipscode/rt25 > test.trace
    rt25_test PASSED
    Tests Complete

Most likely you will have bugs, so try the tracing mode or other debugging modes described in the Framework code section above.

    $ ./mips -t mipscode/simple
    00001000: ori   $4,$4,0x1014
    00001004: ori   $2,$0,0x4
    00001008: syscall
    Hello, world!
    0000100c: ori   $2,$0,0xa
    00001010: syscall
    exiting the simulator

We have provided a few more tests and the ability to write your own. We described simple above, which prints out Hello, world!, and is written in assembly. Here's an example of how to add an assembly test:
1. Create the new assembly file in the mipscode directory (use mipscode/simple.s as a template). Let's assume your test is in mipscode/foo.s.
2. Add the base name of the test to the list of ASM_TESTS in the Makefile. To do this, just add foo to the end of line 4.

Now build your assembly test, and then run it by typing in the following commands:

    $ make
    $ ./mips mipscode/foo

You can, and indeed should, write your own assembly tests to test specific instructions and their corner cases. Furthermore, you should be compiling and testing your code after each group of instructions you implement. It will be very hard to debug your project if you wait until the end to test.