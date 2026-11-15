/*
Sample inputs:
Number of stations: 5
Number of time slots: 10
*/

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

void simulate(const string& name, int stations, int slots, int mode, double p) {
    cout << "\n--- " << name << " ---\n";
    int success = 0, collision = 0, idle = 0;
    srand(42);

    for (int slot = 0; slot < slots; slot++) {
        vector<int> contenders;
        for (int s = 0; s < stations; s++) {
            if ((rand() % 100) < 40) {
                if (mode == 1) { // 1-persistent or CSMA/CD
                    contenders.push_back(s);
                } else if (mode == 2) { // non-persistent
                    if ((rand() % 100) < 50) contenders.push_back(s);
                } else if (mode == 3) { // p-persistent
                    if ((rand() % 100) < static_cast<int>(p * 100)) contenders.push_back(s);
                }
            }
        }
        if (contenders.empty()) {
            idle++;
            cout << "Slot " << slot << ": Idle\n";
        } else if (contenders.size() == 1) {
            success++;
            cout << "Slot " << slot << ": Station " << contenders[0] << " transmitted successfully\n";
        } else {
            collision++;
            cout << "Slot " << slot << ": Collision among " << contenders.size() << " stations\n";
            if (mode == 1 && name[0] == 'C') {
                cout << "  -> Collision detected! Transmissions aborted.\n";
            }
        }
    }
    cout << "Result: Success=" << success << ", Collision=" << collision << ", Idle=" << idle << "\n";
}

int main() {
    int stations = 5, slots = 10;
    string line;

    cout << "Enter number of stations [default 5]: ";
    if (getline(cin, line) && !line.empty()) {
        try { stations = stoi(line); } catch (...) { stations = 5; }
    }

    cout << "Enter number of slots [default 10]: ";
    if (getline(cin, line) && !line.empty()) {
        try { slots = stoi(line); } catch (...) { slots = 10; }
    }

    simulate("CSMA/CD", stations, slots, 1, 0.5);
    simulate("Non-Persistent CSMA", stations, slots, 2, 0.5);
    simulate("1-Persistent CSMA", stations, slots, 1, 0.5);
    simulate("P-Persistent CSMA", stations, slots, 3, 0.5);
    return 0;
}
