/*
Sample inputs:
Binary data word: 1011
Error bit position to corrupt (1-7): 3
*/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    string data = "1011";
    cout << "Enter 4-bit data word [default 1011]: ";
    string line;
    if (getline(cin, line) && !line.empty()) data = line;

    vector<int> d(4);
    for (int i = 0; i < 4; i++) d[i] = data[i] - '0';

    vector<int> c(8);
    c[3] = d[0];
    c[5] = d[1];
    c[6] = d[2];
    c[7] = d[3];

    c[1] = c[3] ^ c[5] ^ c[7];
    c[2] = c[3] ^ c[6] ^ c[7];
    c[4] = c[5] ^ c[6] ^ c[7];

    cout << "\n--- Single-Bit Error Correction ---\n";
    cout << "Original Data: " << data << "\n";
    cout << "Encoded Hamming Code (7-bit): ";
    for (int i = 1; i <= 7; i++) cout << c[i];
    cout << "\n";

    int corrupt_pos = 3;
    cout << "Enter 1-indexed bit position to corrupt (1-7) [default 3]: ";
    if (getline(cin, line) && !line.empty()) {
        try { corrupt_pos = stoi(line); } catch (...) { corrupt_pos = 3; }
    }

    vector<int> recv = c;
    if (corrupt_pos >= 1 && corrupt_pos <= 7) recv[corrupt_pos] ^= 1;

    cout << "Received Codeword: ";
    for (int i = 1; i <= 7; i++) cout << recv[i];
    cout << "\n";

    int s1 = recv[1] ^ recv[3] ^ recv[5] ^ recv[7];
    int s2 = recv[2] ^ recv[3] ^ recv[6] ^ recv[7];
    int s4 = recv[4] ^ recv[5] ^ recv[6] ^ recv[7];
    int syndrome = s1 + (s2 * 2) + (s4 * 4);

    if (syndrome == 0) {
        cout << "No error detected.\n";
    } else {
        cout << "Error detected at bit position: " << syndrome << "\n";
        recv[syndrome] ^= 1;
        cout << "Corrected Codeword: ";
        for (int i = 1; i <= 7; i++) cout << recv[i];
        cout << "\nRecovered Data: " << recv[3] << recv[5] << recv[6] << recv[7] << "\n";
    }

    cout << "\n--- Double-Bit Error Detection (SECDED) ---\n";
    int overall_p = 0;
    for (int i = 1; i <= 7; i++) overall_p ^= c[i];
    cout << "Transmitted 8-bit SECDED: ";
    for (int i = 1; i <= 7; i++) cout << c[i];
    cout << overall_p << "\n";

    vector<int> test2 = c;
    test2.push_back(overall_p); // index 8
    test2[3] ^= 1;
    test2[5] ^= 1;

    int sum_ones = 0;
    for (int i = 1; i <= 8; i++) sum_ones += test2[i];
    bool parity_ok = (sum_ones % 2 == 0);

    int syn2 = (test2[1] ^ test2[3] ^ test2[5] ^ test2[7]) +
               ((test2[2] ^ test2[3] ^ test2[6] ^ test2[7]) * 2) +
               ((test2[4] ^ test2[5] ^ test2[6] ^ test2[7]) * 4);

    cout << "Received with 2 errors (bits 3 & 5 flipped): ";
    for (int i = 1; i <= 8; i++) cout << test2[i];
    cout << "\n";

    if (syn2 != 0 && parity_ok) {
        cout << "Result: Double-bit error detected successfully (SECDED)!\n";
    } else if (syn2 != 0 && !parity_ok) {
        cout << "Result: Single-bit error detected.\n";
    } else {
        cout << "Result: No error.\n";
    }

    return 0;
}
