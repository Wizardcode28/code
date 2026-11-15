/*
Sample inputs:
Total frames: 6
Window size: 3
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

void stop_and_wait(int total_frames) {
    cout << "\n--- Stop-and-Wait Flow Control ---\n";
    for (int i = 0; i < total_frames; i++) {
        cout << "Sender: Sent Frame " << i << "\n";
        cout << "Receiver: Received Frame " << i << ", Sent ACK " << i << "\n";
    }
    cout << "All " << total_frames << " frames sent successfully.\n";
}

void sliding_window(int total_frames, int window_size) {
    cout << "\n--- Sliding Window Flow Control (Window Size = " << window_size << ") ---\n";
    int base = 0;
    while (base < total_frames) {
        int end = min(base + window_size, total_frames);

        cout << "Sender transmits frames: [";
        for (int i = base; i < end; i++) {
            cout << i << (i == end - 1 ? "" : ", ");
        }
        cout << "]\n";

        cout << "Receiver received frames, sent ACK " << end - 1 << "\n";
        base = end;
    }
    cout << "All " << total_frames << " frames sent successfully.\n";
}

int main() {
    int total = 6, window = 3;
    string line;

    cout << "Enter total frames [default 6]: ";
    if (getline(cin, line) && !line.empty()) {
        try { total = stoi(line); } catch (...) { total = 6; }
    }

    cout << "Enter window size [default 3]: ";
    if (getline(cin, line) && !line.empty()) {
        try { window = stoi(line); } catch (...) { window = 3; }
    }

    stop_and_wait(total);
    sliding_window(total, window);
    return 0;
}
