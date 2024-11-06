#include "A1-Task4-S9-S#-S#-20230584-20230461-20240831.h"
#include <iostream>
using namespace std;

int main() {
    Machine machine;
    int choice;
    string filename;

    do {
        cout << "\nMenu:\n";
        cout << "1. Load Program from File\n";
        cout << "2. Execute Program\n";
        cout << "3. Display Machine State\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter program file name: ";
                cin >> filename;
                try {
                    machine.loadProgramFile(filename);
                } catch (const exception& e) {
                    cerr << "Error: " << e.what() << endl;
                }
                break;
            case 2:
                try {
                    machine.executeProgram();
                } catch (const exception& e) {
                    cerr << "Error during execution: " << e.what() << endl;
                }
                break;
            case 3:
                machine.displayState();
                break;
            case 0:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice, please try again." << endl;
        }
    } while (choice != 0);

    return 0;
}