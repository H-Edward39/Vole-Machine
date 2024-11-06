#ifndef FULL_A1_TASK4_S9_S_S_20230584_20230461_20240831_H
#define FULL_A1_TASK4_S9_S_S_20230584_20230461_20240831_H
#include <string>
using namespace std;

class Memory {
public:
    Memory();
    string getCell(int address) const;
    void setCell(int address, const string& value);
    void displayMemory() const;

private:
    string memory[256];
    int size = 256;
};

class Register {
public:
    Register();
    int getCell(int address) const;
    void setCell(int address, int value);
    void displayRegisters() const;

private:
    int registers[16];
    int size = 16;
};

class ALU {
public:
    static int hexToDec(const string& hexStr);
    static string decToHex(int value);
    static bool isValidHex(const string& str);
    static int add(int a, int b);
};

class CU {
public:
    void rxy(char opcode, int regIdx, int memAddr, Register& reg, Memory& mem, int& pc);
    void rst(char opcode, int reg1, int reg2, int destReg, Register& reg);
};

class CPU {
public:
    CPU();
    int getProgramCounter() const;
    string getInstructionRegister() const;
    const Register& getRegisterBank() const;
    bool executeNextInstruction(Memory& memory);

private:
    int programCounter;
    string instructionRegister;
    Register registerBank;
    CU controlUnit;
};

class Machine {
public:
    void loadProgramFile(const string& filename);
    void executeProgram();
    void displayState() const;

private:
    CPU processor;
    Memory memory;
    int instructionCount = 0;
};

#endif
