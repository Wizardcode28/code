/*
Sample inputs:
Total frames to send: 6
Window size for sliding window: 3
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <thread>
#include <cstdlib>

using namespace std;

void stop_and_wait(int total_frames) {
    cout << "\n============================================================\n";
    cout << "      STOP-AND-WAIT FLOW CONTROL PROTOCOL\n";
    cout << "============================================================\n";

    int frame = 0;
    while (frame < total_frames) {
        cout << "Sender: Sending Frame " << frame << "\n";
        this_thread::sleep_for(chrono::milliseconds(400));

        cout << "Sender: Waiting for ACK " << frame << "...\n";
        this_thread::sleep_for(chrono::milliseconds(400));

        // Simulate acknowledgment receipt with high success chance
        bool ack_received = ((rand() % 3) != 0);

        if (ack_received) {
            cout << "Receiver: Frame " << frame << " received successfully -> ACK " << frame << " sent\n";
            cout << "Sender: ACK " << frame << " received. Advancing to next frame.\n\n";
            frame++;
        } else {
            cout << "Receiver/Channel: ACK " << frame << " lost in transmission!\n";
            cout << "Sender: Timeout expired! Retransmitting Frame " << frame << "...\n\n";
        }
    }
    cout << "--> All " << total_frames << " frames transmitted and acknowledged successfully!\n\n";
}

void sliding_window(int total_frames, int window_size) {
    cout << "============================================================\n";
    cout << "   SLIDING WINDOW FLOW CONTROL (Window Size = " << window_size << ")\n";
    cout << "============================================================\n";

    int base = 0;
    while (base < total_frames) {
        int end = min(base + window_size, total_frames);
        cout << "Current Window: [";
        for (int i = base; i < end; i++) cout << i << (i == end - 1 ? "" : ", ");
        cout << "] (Buffer capacity: " << window_size << ")\n";

        for (int i = base; i < end; i++) {
            cout << "  Sender: Sending Frame " << i << "\n";
            this_thread::sleep_for(chrono::milliseconds(250));
        }

        cout << "  Receiver: Frames arrived in order. Processing batch...\n";
        this_thread::sleep_for(chrono::milliseconds(300));

        for (int i = base; i < end; i++) {
            cout << "  Receiver: ACK " << i << " generated\n";
            this_thread::sleep_for(chrono::milliseconds(150));
        }

        cout << "  Sender: Cumulative ACK " << end - 1 << " received. Sliding window forward by "
             << (end - base) << " frames.\n\n";
        base = end;
    }
    cout << "--> All " << total_frames << " frames transmitted and acknowledged successfully!\n\n";
}

int main() {
    srand(42);
    int total = 6, window = 3;
    string line;

    cout << "Enter total number of frames [default 6]: ";
    if (getline(cin, line) && !line.empty()) {
        try { total = stoi(line); } catch (...) { total = 6; }
    }

    cout << "Enter window size for sliding window [default 3]: ";
    if (getline(cin, line) && !line.empty()) {
        try { window = stoi(line); } catch (...) { window = 3; }
    }

    stop_and_wait(total);
    sliding_window(total, window);
    return 0;
}

/*
================================================================================
LAB EXPLANATION:
Lab 5 demonstrates Flow Control protocols at the Data Link and Transport layers.
Flow control prevents sender-receiver speed mismatch from exhausting buffer space.

1. Stop-and-Wait Flow Control:
   - The sender transmits 1 frame and suspends transmission until an ACK arrives.
   - If an ACK is lost or delayed, a timeout timer expires, triggering retransmission.
   - Low link utilization when Round-Trip Time (RTT) is large.

2. Sliding Window Flow Control:
   - Sender transmits up to 'W' frames concurrently without waiting for individual ACKs.
   - As cumulative ACKs arrive, the window boundary slides forward.
   - Dramatically increases channel efficiency and throughput (pipelining).

CODE EXPLANATION:
- 'stop_and_wait()': Simulates transmission delay, ACK waiting, timeout expiration,
  and retransmission of unacknowledged frames.
- 'sliding_window()': Maintains window pointers ('base', 'end'), batches transmissions,
  receives cumulative ACKs, and slides the window boundary forward.
================================================================================
*/
