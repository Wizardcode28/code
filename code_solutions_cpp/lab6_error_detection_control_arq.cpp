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

using namespace std;

int get_ones(const string& s) {
    return count(s.begin(), s.end(), '1');
}

void parity_demo(const string& data) {
    cout << "\n--- 1. Even and Odd Parity Check (VRC) ---\n";
    int ones = get_ones(data);
    char even_p = (ones % 2 == 0) ? '0' : '1';
    char odd_p = (ones % 2 == 0) ? '1' : '0';

    string even_code = data + even_p;
    string odd_code = data + odd_p;

    cout << "Even Parity: Data=" << data << " -> Code=" << even_code << " -> Valid=" << ((get_ones(even_code) % 2 == 0) ? "Yes" : "No") << "\n";
    cout << "Odd Parity : Data=" << data << " -> Code=" << odd_code << " -> Valid=" << ((get_ones(odd_code) % 2 != 0) ? "Yes" : "No") << "\n";
}

void lrc_demo() {
    cout << "\n--- 2. Longitudinal Redundancy Check (LRC) ---\n";
    vector<string> words = {"1011", "1100", "0110"};
    string lrc_val = "";
    for (int col = 0; col < 4; col++) {
        int sum = 0;
        for (const auto& w : words) sum += (w[col] - '0');
        lrc_val += (sum % 2 == 0) ? '0' : '1';
    }
    cout << "Words: 1011, 1100, 0110 -> LRC Parity: " << lrc_val << "\n";
}

string crc_calculate(string data, const string& gen) {
    int data_len = data.length();
    int gen_len = gen.length();
    data.append(gen_len - 1, '0');

    for (int i = 0; i < data_len; i++) {
        if (data[i] == '1') {
            for (int j = 0; j < gen_len; j++) {
                data[i + j] = (data[i + j] == gen[j]) ? '0' : '1';
            }
        }
    }
    return data.substr(data_len);
}

void crc_demo(const string& data, const string& gen) {
    cout << "\n--- 3. Cyclic Redundancy Check (CRC) ---\n";
    string rem = crc_calculate(data, gen);
    string code = data + rem;
    cout << "Data: " << data << ", Generator: " << gen << " -> CRC Codeword: " << code << "\n";
}

void stop_and_wait_arq(int total, int lost_frame) {
    cout << "\n--- 4. Stop-and-Wait ARQ ---\n";
    bool lost = false;
    for (int i = 0; i < total; i++) {
        int attempt = 1;
        while (true) {
            cout << "Sender: Sending Frame " << i << " (Attempt " << attempt << ")\n";
            if (i == lost_frame && !lost) {
                cout << "  -> Frame " << i << " lost! Timeout, retransmitting...\n";
                lost = true;
                attempt++;
            } else {
                cout << "  -> Receiver got Frame " << i << ", ACK " << i << " received\n";
                break;
            }
        }
    }
}

void go_back_n(int total, int window, int lost_frame) {
    cout << "\n--- 5. Go-Back-N ARQ (Window = " << window << ") ---\n";
    int base = 0;
    bool lost = false;
    while (base < total) {
        int end = min(base + window, total);
        cout << "Window [" << base << " to " << end - 1 << "]: Sending frames... \n";
        int ack = base - 1;
        for (int i = base; i < end; i++) {
            if (i == lost_frame && !lost) {
                cout << "  -> Frame " << i << " LOST! Subsequent frames discarded.\n";
                lost = true;
                break;
            }
            cout << "  -> Frame " << i << " received, ACK " << i << " sent\n";
            ack = i;
        }
        if (ack >= base) base = ack + 1;
        else cout << "  -> Timeout! Retransmitting from Frame " << base << "\n";
    }
}

void selective_repeat(int total, int window, int lost_frame) {
    cout << "\n--- 6. Selective-Repeat ARQ (Window = " << window << ") ---\n";
    vector<bool> acked(total, false);
    bool lost = false;
    int base = 0;
    while (base < total) {
        int end = min(base + window, total);
        for (int i = base; i < end; i++) {
            if (acked[i]) continue;
            if (i == lost_frame && !lost) {
                cout << "  -> Frame " << i << " LOST! Receiver buffers other frames.\n";
                lost = true;
            } else {
                cout << "  -> Frame " << i << " received and buffered, ACK " << i << " sent\n";
                acked[i] = true;
            }
        }
        if (base == lost_frame && !acked[base]) {
            cout << "  -> Timeout! Retransmitting ONLY lost Frame " << base << "\n";
            cout << "  -> Frame " << base << " received, ACK " << base << " sent\n";
            acked[base] = true;
        }
        while (base < total && acked[base]) base++;
    }
}

int main() {
    string data = "1011001";
    string gen = "1101";
    int total = 5, window = 3, lost_frame = 2;
    string line;

    cout << "Enter binary data [default 1011001]: ";
    if (getline(cin, line) && !line.empty()) data = line;

    cout << "Enter CRC generator polynomial [default 1101]: ";
    if (getline(cin, line) && !line.empty()) gen = line;

    cout << "Enter total frames for ARQ [default 5]: ";
    if (getline(cin, line) && !line.empty()) {
        try { total = stoi(line); } catch (...) { total = 5; }
    }

    cout << "Enter window size [default 3]: ";
    if (getline(cin, line) && !line.empty()) {
        try { window = stoi(line); } catch (...) { window = 3; }
    }

    cout << "Enter frame to simulate loss [default 2]: ";
    if (getline(cin, line) && !line.empty()) {
        try { lost_frame = stoi(line); } catch (...) { lost_frame = 2; }
    }

    parity_demo(data);
    lrc_demo();
    crc_demo(data, gen);
    stop_and_wait_arq(total, min(lost_frame, total - 1));
    go_back_n(total, window, lost_frame);
    selective_repeat(total, window, lost_frame);
    return 0;
}
