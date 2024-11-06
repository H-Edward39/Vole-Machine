#include "A1-Task4-S9-S#-S#-20230584-20230461-20240831.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

Memory::Memory() {
    for (int i = 0; i < size; i++) {
        memory[i] = "0000";
    }
}

string Memory::getCell(int address) const {
    if (address >= 0 && address < size) {
        return memory[address];
    }
    throw runtime_error("Memory address out of bounds: " + to_string(address));
}

void Memory::setCell(int address, const string& value) {
    if (address >= 0 && address < size) {
        string paddedValue = value;
        while (paddedValue.length() < 4) paddedValue = "0" + paddedValue;
        memory[address] = paddedValue.substr(0, 4);
    } else {
        throw runtime_error("Memory address out of bounds: " + to_string(address));
    }
}

void Memory::displayMemory() const {
    cout << "\nMemory Contents:\n";
    cout << "Addr\t+0\t+1\t+2\t+3\t+4\t+5\t+6\t+7\n";
    for (int i = 0; i < size; i += 8) {
        cout << hex << uppercase << setw(3) << setfill('0') << i << "\t";
        for (int j = 0; j < 8 && (i + j) < size; ++j) {
            cout << memory[i + j] << "\t";
        }
        cout << endl;
    }
    cout << dec;
}

Register::Register() {
    for (int i = 0; i < size; i++) {
        registers[i] = 0;
    }
}

int Register::getCell(int address) const {
    if (address >= 0 && address < size) {
        return registers[address];
    }
    throw runtime_error("Register index out of bounds: " + to_string(address));
}

void Register::setCell(int address, int value) {
    if (address >= 0 && address < size) {
        registers[address] = value;
    } else {
        throw runtime_error("Register index out of bounds: " + to_string(address));
    }
}

void Register::displayRegisters() const {
    cout << "\nRegister Contents:\n";
    for (int i = 0; i < size; i++) {
        cout << "R" << hex << uppercase << i << ": 0x"
             << setw(4) << setfill('0') << registers[i] << dec << endl;
    }
}

int ALU::hexToDec(const string& hexStr) {
    return stoi(hexStr, nullptr, 16);
}

string ALU::decToHex(int value) {
    stringstream ss;
    ss << hex << uppercase << setw(4) << setfill('0') << value;
    return ss.str();
}

bool ALU::isValidHex(const string& str) {
    if (str.empty() || str.length() > 4) return false;
    return all_of(str.begin(), str.end(), ::isxdigit);
}

int ALU::add(int a, int b) {
    return a + b;
}

void CU::rxy(char opcode, int regIdx, int memAddr, Register& reg, Memory& mem, int& pc) {
    switch (opcode) {
        case '1':  // LOAD R, (XY)
            // Load the register with the value from memory address XY
            reg.setCell(regIdx, ALU::hexToDec(mem.getCell(memAddr)));
            break;
        case '2':
            reg.setCell(regIdx, memAddr & 0xFF);
            break;
        case '3':
            mem.setCell(memAddr, ALU::decToHex(reg.getCell(regIdx)));
            break;
        case 'B':
            if (reg.getCell(regIdx) == 0) {
                pc = memAddr - 1; // Subtract 1 because PC will be incremented after instruction
            }
            break;
        default:
            cout << "Unhandled RXY opcode: " << opcode << endl;
    }
}

void CU::rst(char opcode, int reg1, int reg2, int destReg, Register& reg) {
    switch (opcode) {
        case '5': {
            int val1 = reg.getCell(reg1);
            int val2 = reg.getCell(reg2);
            int result = ALU::add(val1, val2);
            reg.setCell(destReg, result);
            break;
        }
        case '6': {
            // Print logic
            cout << "Result: " << reg.getCell(reg1) << endl;
            break;
        }
        default:
            cout << "Unhandled RST opcode: " << opcode << endl;
    }
}

CPU::CPU() : programCounter(0) {}

int CPU::getProgramCounter() const { return programCounter; }
string CPU::getInstructionRegister() const { return instructionRegister; }
const Register& CPU::getRegisterBank() const { return registerBank; }

bool CPU::executeNextInstruction(Memory& memory) {
    // Fetch instruction
    instructionRegister = memory.getCell(programCounter);

    // Check for HALT
    if (instructionRegister == "C000") return false;

    if (instructionRegister.length() == 4) {
        char opcode = instructionRegister[0];
        string operand = instructionRegister.substr(1, 3);
        int addr = stoi(operand, nullptr, 16);

        // Handle RXY format instructions
        if (opcode == '1' || opcode == '2' || opcode == '3' || opcode == 'B') {
            int regIdx = (addr >> 8) & 0xF;
            int memAddr = addr & 0xFF;
            controlUnit.rxy(opcode, regIdx, memAddr, registerBank, memory, programCounter);
        }
            // Handle RST format instructions
        else if (opcode == '5' || opcode == '6') {
            int reg1 = (addr >> 8) & 0xF;
            int reg2 = (addr >> 4) & 0xF;
            int destReg = addr & 0xF;
            controlUnit.rst(opcode, reg1, reg2, destReg, registerBank);
        }

        // Always increment PC after instruction execution
        programCounter++;
    }

    return true;
}

void Machine::loadProgramFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) throw runtime_error("Could not open file: " + filename);

    string instruction;
    int address = 0;

    while (getline(file, instruction)) {
        instruction.erase(remove_if(instruction.begin(), instruction.end(), ::isspace), instruction.end());
        transform(instruction.begin(), instruction.end(), instruction.begin(), ::toupper);
        if (!instruction.empty()) {
            memory.setCell(address, instruction);
            address++;
        }
    }
    instructionCount = address;  // Store the number of loaded instructions
    cout << instructionCount << " instructions loaded" << endl;
}

void Machine::executeProgram() {
    for (int i = 0; i < instructionCount; ++i) {  // Loop only through loaded instructions
        if (!processor.executeNextInstruction(memory)) {
            break;  // Stop execution if a HALT instruction is encountered
        }
        displayState();
    }
}

void Machine::displayState() const {
    cout << "\n=== Machine State ===\n";
    cout << "Program Counter: 0x" << hex << uppercase << setw(4) << setfill('0') << processor.getProgramCounter() << endl;
    cout << "Instruction Register: " << processor.getInstructionRegister() << endl;
    processor.getRegisterBank().displayRegisters();
    memory.displayMemory();
    cout << dec;  // Reset to decimal output
}
