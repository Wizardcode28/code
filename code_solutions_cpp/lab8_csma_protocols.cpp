/*
Sample inputs:
Number of stations: 5
Number of time slots: 10
Probability p for p-persistent: 0.5
*/

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <iomanip>

using namespace std;

void simulate_csma(const string& protocol, int stations, int slots, double p) {
    cout << "\n============================================================\n";
    cout << " " << protocol << " CSMA SIMULATION (Stations=" << stations << ", Slots=" << slots << ")\n";
    cout << "============================================================\n";

    int success = 0, collision = 0, idle = 0;
    srand(42);

    for (int slot = 0; slot < slots; slot++) {
        vector<int> contenders;
        for (int s = 0; s < stations; s++) {
            bool has_data = ((rand() % 100) < 40);
            if (!has_data) continue;

            if (protocol == "1-Persistent" || protocol == "CSMA/CD") {
                contenders.push_back(s);
            } else if (protocol == "Non-Persistent") {
                if ((rand() % 100) < 50) contenders.push_back(s);
            } else if (protocol == "P-Persistent") {
                if ((rand() % 100) < static_cast<int>(p * 100)) contenders.push_back(s);
            }
        }

        if (contenders.empty()) {
            idle++;
            cout << "Slot " << setw(2) << slot << ": [IDLE] No station transmitted.\n";
        } else if (contenders.size() == 1) {
            success++;
            cout << "Slot " << setw(2) << slot << ": [SUCCESS] Station " << contenders[0] << " seized channel and transmitted!\n";
        } else {
            collision++;
            cout << "Slot " << setw(2) << slot << ": [COLLISION] Multiple stations transmitted simultaneously!\n";
            if (protocol == "CSMA/CD") {
                cout << "         --> Collision Detected! Jam signal sent, stations aborted & backed off.\n";
            }
        }
    }

    double eff = (static_cast<double>(success) / slots) * 100.0;
    cout << "\nSummary for " << protocol << ":\n";
    cout << "  Successful Transmissions: " << success << "\n";
    cout << "  Collisions Encountered:   " << collision << "\n";
    cout << "  Idle Slots:               " << idle << "\n";
    cout << "  Channel Efficiency:       " << fixed << setprecision(1) << eff << "%\n";
}

void comparative_analysis(int stations = 8, int slots = 100, double p = 0.25) {
    cout << "\n============================================================\n";
    cout << " COMPARATIVE CSMA PROTOCOL PERFORMANCE (Monte Carlo 100 Slots)\n";
    cout << "============================================================\n";
    cout << left << setw(18) << "Protocol" << setw(10) << "Success" << setw(12) << "Collision" << setw(8) << "Idle" << "Efficiency\n";
    cout << string(58, '-') << "\n";

    vector<string> protos = {"Non-Persistent", "1-Persistent", "P-Persistent"};
    for (const auto& proto : protos) {
        srand(11);
        int success = 0, collision = 0, idle = 0;
        for (int i = 0; i < slots; i++) {
            vector<int> active;
            for (int s = 0; s < stations; s++) {
                if ((rand() % 100) < 20) {
                    if (proto == "Non-Persistent" && (rand() % 100) < 50) active.push_back(s);
                    else if (proto == "P-Persistent" && (rand() % 100) < static_cast<int>(p * 100)) active.push_back(s);
                    else if (proto == "1-Persistent") active.push_back(s);
                }
            }
            if (active.empty()) idle++;
            else if (active.size() == 1) success++;
            else collision++;
        }
        double eff = (static_cast<double>(success) / slots) * 100.0;
        cout << left << setw(18) << proto << setw(10) << success << setw(12) << collision << setw(8) << idle
             << fixed << setprecision(1) << eff << "%\n";
    }
}

int main() {
    int stations = 5, slots = 10;
    double p = 0.5;
    string line;

    cout << "Enter number of stations [default 5]: ";
    if (getline(cin, line) && !line.empty()) {
        try { stations = stoi(line); } catch (...) { stations = 5; }
    }

    cout << "Enter number of time slots [default 10]: ";
    if (getline(cin, line) && !line.empty()) {
        try { slots = stoi(line); } catch (...) { slots = 10; }
    }

    cout << "Enter probability p for p-persistent [default 0.5]: ";
    if (getline(cin, line) && !line.empty()) {
        try { p = stod(line); } catch (...) { p = 0.5; }
    }

    simulate_csma("CSMA/CD", stations, slots, p);
    simulate_csma("Non-Persistent", stations, slots, p);
    simulate_csma("1-Persistent", stations, slots, p);
    simulate_csma("P-Persistent", stations, slots, p);

    comparative_analysis();
    return 0;
}

/*
================================================================================
LAB EXPLANATION:
Lab 8 covers Multiple Access Protocols at the MAC sublayer:
Carrier Sense Multiple Access (CSMA) variants and Collision Detection (CSMA/CD).

1. Persistence Strategies:
   - 1-Persistent: Station transmits immediately with probability 1 if channel is idle.
   - Non-Persistent: If channel is busy, waits random backoff before sensing again.
   - P-Persistent: If channel is idle, transmits with probability p, defers with (1-p).
   - CSMA/CD: Listens while transmitting, aborts immediately on collision, broadcasts Jam signal.

CODE EXPLANATION:
- 'simulate_csma()': Simulates discrete time slots tracking contention and collision events.
- 'comparative_analysis()': Runs an automated 100-slot comparative benchmark across protocols.
================================================================================
*/
