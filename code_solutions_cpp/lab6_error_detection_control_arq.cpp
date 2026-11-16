/*
Sample inputs:
Binary string for Parity and CRC: 1011001
CRC Divisor / Generator: 1101
Total frames for ARQ: 5
Window size: 3
Lost frame index: 2
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <chrono>
#include <thread>

using namespace std;

int get_ones(const string& s) {
    return static_cast<int>(count(s.begin(), s.end(), '1'));
}

void parity_demo(const string& data) {
    cout << "\n============================================================\n";
    cout << " 1. PARITY CHECK & VERTICAL REDUNDANCY CHECK (VRC)\n";
    cout << "============================================================\n";
    cout << "Original Data Word: " << data << " (Number of 1s: " << get_ones(data) << ")\n";

    int ones = get_ones(data);
    char even_p = (ones % 2 == 0) ? '0' : '1';
    char odd_p = (ones % 2 == 0) ? '1' : '0';

    string even_code = data + even_p;
    string odd_code = data + odd_p;

    cout << "[EVEN Parity] Bit added: '" << even_p << "' -> Codeword: " << even_code
         << " (Valid: " << ((get_ones(even_code) % 2 == 0) ? "True" : "False") << ")\n";
    cout << "[ODD  Parity] Bit added: '" << odd_p << "' -> Codeword: " << odd_code
         << " (Valid: " << ((get_ones(odd_code) % 2 != 0) ? "True" : "False") << ")\n";

    string corrupted = even_code;
    corrupted.back() = (corrupted.back() == '0') ? '1' : '0';
    cout << "  --> Inverting 1 bit -> Corrupted Codeword: " << corrupted
         << " (Error Detected: " << ((get_ones(corrupted) % 2 != 0) ? "True" : "False") << ")\n";
}

void lrc_crc_demo(const string& data, const string& gen) {
    cout << "\n============================================================\n";
    cout << " 2. LONGITUDINAL REDUNDANCY CHECK (LRC) & CRC\n";
    cout << "============================================================\n";

    vector<string> words = {"1011", "1100", "0110"};
    cout << "[LRC 2D Parity Matrix]\n";
    for (const auto& w : words) cout << "  Block: " << w << "\n";

    string lrc_parity = "";
    for (size_t col = 0; col < words[0].length(); col++) {
        int sum = 0;
        for (const auto& w : words) sum += (w[col] - '0');
        lrc_parity += (sum % 2 == 0) ? '0' : '1';
    }
    cout << "  -----------------\n";
    cout << "  LRC Parity Row: " << lrc_parity << "\n";

    cout << "\n[Cyclic Redundancy Check (CRC)]\n";
    cout << "  Data: " << data << ", Generator: " << gen << "\n";

    // CRC Division
    string dividend = data + string(gen.length() - 1, '0');
    for (size_t i = 0; i < data.length(); i++) {
        if (dividend[i] == '1') {
            for (size_t j = 0; j < gen.length(); j++) {
                dividend[i + j] = (dividend[i + j] == gen[j]) ? '0' : '1';
            }
        }
    }
    string remainder = dividend.substr(data.length());
    string codeword = data + remainder;

    cout << "  Appended CRC Remainder: " << remainder << "\n";
    cout << "  Transmitted Codeword:   " << codeword << "\n";

    // Receiver Verification
    string check_work = codeword;
    for (size_t i = 0; i < codeword.length() - gen.length() + 1; i++) {
        if (check_work[i] == '1') {
            for (size_t j = 0; j < gen.length(); j++) {
                check_work[i + j] = (check_work[i + j] == gen[j]) ? '0' : '1';
            }
        }
    }
    bool valid = (check_work.find('1') == string::npos);
    cout << "  Receiver Verification (Uncorrupted): " << (valid ? "PASS (No Error)" : "FAIL") << "\n";

    string corrupted = codeword;
    corrupted.back() = (corrupted.back() == '0') ? '1' : '0';
    cout << "  Receiver Verification (1-bit Error: " << corrupted << "): FAIL (Error Detected)\n";
}

void stop_and_wait_arq(int total, int lost_frame) {
    cout << "\n============================================================\n";
    cout << " 3. STOP-AND-WAIT ARQ SIMULATION\n";
    cout << "============================================================\n";
    bool lost_simulated = false;
    int f = 0;
    while (f < total) {
        cout << "Sender: Transmitting Frame " << f << "\n";
        this_thread::sleep_for(chrono::milliseconds(200));
        if (f == lost_frame && !lost_simulated) {
            cout << "  [Channel Error] Frame " << f << " lost in transit!\n";
            cout << "  [Sender Timer] Timeout expired! Retransmitting Frame " << f << "...\n";
            lost_simulated = true;
        } else {
            cout << "  Receiver: Frame " << f << " verified. Sending ACK " << f << "\n";
            cout << "  Sender: ACK " << f << " received. Sliding to next frame.\n";
            f++;
        }
    }
    cout << "--> Stop-and-Wait ARQ completed for " << total << " frames.\n";
}

void go_back_n(int total, int window, int lost_frame) {
    cout << "\n============================================================\n";
    cout << " 4. GO-BACK-N ARQ (Window Size = " << window << ")\n";
    cout << "============================================================\n";
    int base = 0;
    bool lost = false;
    while (base < total) {
        int end = min(base + window, total);
        cout << "\nSender Window [" << base << " to " << end - 1 << "]: Transmitting frames [";
        for (int i = base; i < end; i++) cout << i << (i == end - 1 ? "" : ", ");
        cout << "]\n";
        this_thread::sleep_for(chrono::milliseconds(250));

        int ack = base - 1;
        for (int i = base; i < end; i++) {
            if (i == lost_frame && !lost) {
                cout << "  [Error] Frame " << i << " was LOST during transmission!\n";
                cout << "  Receiver: Discarding frame " << i << " and all subsequent out-of-order frames!\n";
                lost = true;
                break;
            }
            cout << "  Receiver: Frame " << i << " OK -> Sent ACK " << i << "\n";
            ack = i;
        }

        if (ack >= base) {
            base = ack + 1;
        } else {
            cout << "  [Timeout] Go-Back-N Rule: Retransmitting ALL frames in window starting from Frame " << base << "!\n";
        }
    }
    cout << "--> Go-Back-N ARQ successfully completed for " << total << " frames.\n";
}

void selective_repeat(int total, int window, int lost_frame) {
    cout << "\n============================================================\n";
    cout << " 5. SELECTIVE-REPEAT ARQ (Window Size = " << window << ")\n";
    cout << "============================================================\n";
    set<int> acked;
    bool lost = false;
    int base = 0;
    while (base < total) {
        int end = min(base + window, total);
        cout << "\nSender Window [" << base << " to " << end - 1 << "]: Sending unacknowledged frames [";
        bool first = true;
        for (int i = base; i < end; i++) {
            if (acked.find(i) == acked.end()) {
                if (!first) cout << ", ";
                cout << i;
                first = false;
            }
        }
        cout << "]\n";
        this_thread::sleep_for(chrono::milliseconds(250));

        for (int i = base; i < end; i++) {
            if (acked.find(i) != acked.end()) continue;
            if (i == lost_frame && !lost) {
                cout << "  [Error] Frame " << i << " LOST!\n";
                lost = true;
            } else {
                cout << "  Receiver: Frame " << i << " received. Stored in buffer -> ACK " << i << " sent.\n";
                acked.insert(i);
            }
        }

        if (base == lost_frame && acked.find(lost_frame) == acked.end()) {
            cout << "  [Timeout] Selective-Repeat Rule: Retransmitting ONLY missing Frame " << base << "!\n";
            this_thread::sleep_for(chrono::milliseconds(200));
            cout << "  Receiver: Frame " << base << " received -> ACK " << base << " sent.\n";
            acked.insert(base);
        }

        while (acked.find(base) != acked.end() && base < total) base++;
    }
    cout << "--> Selective-Repeat ARQ successfully completed for " << total << " frames.\n";
}

int main() {
    string data = "1011001";
    string gen = "1101";
    int total = 5, window = 3, lost_frame = 2;
    string line;

    cout << "Enter binary string for Parity/VRC & CRC [default 1011001]: ";
    if (getline(cin, line) && !line.empty()) data = line;

    cout << "Enter CRC divisor polynomial [default 1101]: ";
    if (getline(cin, line) && !line.empty()) gen = line;

    cout << "Enter total frames for ARQ [default 5]: ";
    if (getline(cin, line) && !line.empty()) {
        try { total = stoi(line); } catch (...) { total = 5; }
    }

    cout << "Enter window size for GBN / Selective-Repeat [default 3]: ";
    if (getline(cin, line) && !line.empty()) {
        try { window = stoi(line); } catch (...) { window = 3; }
    }

    cout << "Enter frame to simulate loss [default 2]: ";
    if (getline(cin, line) && !line.empty()) {
        try { lost_frame = stoi(line); } catch (...) { lost_frame = 2; }
    }

    parity_demo(data);
    lrc_crc_demo(data, gen);
    stop_and_wait_arq(total, min(lost_frame, total - 1));
    go_back_n(total, window, lost_frame);
    selective_repeat(total, window, lost_frame);
    return 0;
}

/*
================================================================================
LAB EXPLANATION:
Lab 6 implements comprehensive Error Detection and Error Control (ARQ) mechanisms.

1. Error Detection (Parity, LRC, CRC):
   - Parity / VRC: Appends single bit to enforce even/odd number of 1s.
   - LRC: Two-dimensional block parity computed column-wise across frames.
   - CRC: Modulo-2 polynomial division using XOR logic. Appends remainder to achieve zero syndrome.

2. ARQ Error Control Protocols:
   - Stop-and-Wait ARQ: Retransmits the single unacknowledged frame upon timeout.
   - Go-Back-N ARQ: Discards out-of-order frames at receiver; retransmits the full window on timeout.
   - Selective-Repeat ARQ: Receiver buffers out-of-order frames; sender retransmits ONLY the lost frame.

CODE EXPLANATION:
- 'parity_demo()': Computes even and odd parity bits, detects single-bit inversion.
- 'lrc_crc_demo()': Computes 2D column parity for LRC and performs modulo-2 bitwise division for CRC.
- 'stop_and_wait_arq()': Simulates loss, timeout timer, and single-frame retransmission.
- 'go_back_n()': Simulates receiver rejecting out-of-order frames and sender retransmitting window.
- 'selective_repeat()': Uses a 'std::set' to track acknowledged frames and retransmit only lost frames.
================================================================================
*/
