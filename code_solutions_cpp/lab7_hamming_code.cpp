/*
Sample inputs:
Binary data word: 1011
Error bit position to corrupt (1 to codeword length): 3
*/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int calculate_redundant_bits(int m) {
    for (int r = 1; r < m + 10; r++) {
        if ((1 << r) >= m + r + 1) return r;
    }
    return 0;
}

pair<string, int> encode_hamming(const string& data) {
    int m = data.length();
    int r = calculate_redundant_bits(m);
    int total_len = m + r;

    vector<char> code(total_len + 1, '0');
    int j = 0;
    for (int pos = 1; pos <= total_len; pos++) {
        if ((pos & (pos - 1)) != 0) { // Not power of 2
            code[pos] = data[j++];
        }
    }

    for (int i = 0; i < r; i++) {
        int p_pos = 1 << i;
        int parity = 0;
        for (int pos = 1; pos <= total_len; pos++) {
            if (pos & p_pos) {
                parity ^= (code[pos] - '0');
            }
        }
        code[p_pos] = static_cast<char>('0' + parity);
    }

    string res = "";
    for (int i = 1; i <= total_len; i++) res += code[i];
    return {res, r};
}

pair<string, int> decode_hamming(string codeword, int r) {
    int n = codeword.length();
    vector<char> code(n + 1, '0');
    for (int i = 1; i <= n; i++) code[i] = codeword[i - 1];

    int syndrome = 0;
    for (int i = 0; i < r; i++) {
        int p_pos = 1 << i;
        if (p_pos > n) break;
        int parity = 0;
        for (int pos = 1; pos <= n; pos++) {
            if (pos & p_pos) {
                parity ^= (code[pos] - '0');
            }
        }
        if (parity != 0) syndrome += p_pos;
    }

    if (syndrome != 0 && syndrome <= n) {
        cout << "  [Correction] Error detected at Bit Position " << syndrome << "! Inverting bit...\n";
        code[syndrome] = (code[syndrome] == '0') ? '1' : '0';
    } else if (syndrome == 0) {
        cout << "  [Verification] Syndrome is 0 -> No bit errors present.\n";
    }

    string corrected = "";
    for (int i = 1; i <= n; i++) corrected += code[i];
    return {corrected, syndrome};
}

string extract_data(const string& codeword) {
    string data = "";
    for (size_t pos = 1; pos <= codeword.length(); pos++) {
        if ((pos & (pos - 1)) != 0) {
            data += codeword[pos - 1];
        }
    }
    return data;
}

int count_ones(const string& s) {
    int c = 0;
    for (char ch : s) if (ch == '1') c++;
    return c;
}

int main() {
    string msg = "1011";
    cout << "Enter binary data word [default 1011]: ";
    string line;
    if (getline(cin, line) && !line.empty()) msg = line;

    auto [cw, r] = encode_hamming(msg);

    cout << "\n============================================================\n";
    cout << " 1. HAMMING CODE SINGLE-BIT ERROR CORRECTION\n";
    cout << "============================================================\n";
    cout << "Input Data Word (m=" << msg.length() << "): " << msg << "\n";
    cout << "Number of Parity Bits Required (r): " << r << "\n";
    cout << "Encoded Hamming Codeword: " << cw << "\n";

    int err_pos = 3;
    cout << "Enter 1-indexed bit to corrupt (1-" << cw.length() << ") [default 3]: ";
    if (getline(cin, line) && !line.empty()) {
        try { err_pos = stoi(line); } catch (...) { err_pos = 3; }
    }

    string corrupted = cw;
    if (err_pos >= 1 && err_pos <= static_cast<int>(cw.length())) {
        corrupted[err_pos - 1] = (corrupted[err_pos - 1] == '0') ? '1' : '0';
    }

    cout << "\nTransmitted: " << cw << "\n";
    cout << "Received (Corrupted at Bit " << err_pos << "): " << corrupted << "\n";

    auto [fixed, syndrome] = decode_hamming(corrupted, r);
    cout << "Calculated Syndrome: " << syndrome << " (Points to erroneous bit)\n";
    cout << "Corrected Codeword:  " << fixed << "\n";
    cout << "Recovered Data Word: " << extract_data(fixed) << "\n";

    cout << "\n============================================================\n";
    cout << " 2. SECDED (MULTI-BIT ERROR DETECTION)\n";
    cout << "============================================================\n";
    char overall_p = (count_ones(cw) % 2 == 0) ? '0' : '1';
    string secded_word = cw + overall_p;
    cout << "Transmitted 8-bit SECDED Word (Hamming + Overall Parity): " << secded_word << "\n";

    // 1-bit error test
    string sec_1err = secded_word;
    sec_1err[2] = (sec_1err[2] == '0') ? '1' : '0';
    auto [_, syn1] = decode_hamming(sec_1err.substr(0, cw.length()), r);
    bool parity1 = (count_ones(sec_1err) % 2 == 0);
    cout << "\n[Test 1-Bit Error (" << sec_1err << ")]: "
         << ((syn1 != 0 && !parity1) ? "Single-Bit Error Corrected!" : "Error") << "\n";

    // 2-bit error test
    string sec_2err = secded_word;
    sec_2err[2] = (sec_2err[2] == '0') ? '1' : '0';
    sec_2err[5] = (sec_2err[5] == '0') ? '1' : '0';
    auto [__, syn2] = decode_hamming(sec_2err.substr(0, cw.length()), r);
    bool parity2 = (count_ones(sec_2err) % 2 == 0);
    cout << "[Test 2-Bit Error (" << sec_2err << ")]: "
         << ((syn2 != 0 && parity2) ? "DOUBLE-BIT ERROR DETECTED! (Uncorrectable)" : "Error") << "\n";

    return 0;
}

/*
================================================================================
LAB EXPLANATION:
Lab 7 implements Hamming Code forward error correction (FEC) and SECDED.

1. Hamming (7,4) Code Principle:
   - Formulates parity bit count: 2^r >= m + r + 1.
   - Parity bits are placed at power-of-two positions (1, 2, 4, 8...).
   - Data bits fill remaining non-power-of-two positions.
   - Syndrome decoding isolates the exact bit position of an error by testing parity bit sets.

2. SECDED (Single Error Correction, Double Error Detection):
   - Adds an 8th overall parity bit.
   - If Syndrome != 0 and parity is odd -> 1-bit error (Correctable).
   - If Syndrome != 0 and parity is even -> 2-bit error (Double bit error detected).

CODE EXPLANATION:
- 'calculate_redundant_bits(m)': Solves 2^r >= m + r + 1.
- 'encode_hamming()': Places data bits and computes parity bits.
- 'decode_hamming()': Computes syndrome integer and inverts the erroneous bit.
- 'main()': Validates 1-bit correction and SECDED 2-bit error detection.
================================================================================
*/
