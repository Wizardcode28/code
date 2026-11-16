# Sample inputs:
# Total frames: 6
# Window size for Sliding Window: 3

import time
import random

def stop_and_wait(total_frames):
    print("\n" + "=" * 55)
    print("      STOP-AND-WAIT FLOW CONTROL PROTOCOL")
    print("=" * 55)
    
    frame = 0
    while frame < total_frames:
        print(f"Sender: Sending Frame {frame}")
        time.sleep(0.5)
        
        print(f"Sender: Waiting for ACK {frame}...")
        time.sleep(0.5)
        
        # Simulate transmission acknowledgment check
        ack_received = random.choice([True, True, False])  # High probability of success for demo
        
        if ack_received:
            print(f"Receiver: Frame {frame} received successfully -> ACK {frame} sent")
            print(f"Sender: ACK {frame} received. Advancing to next frame.\n")
            frame += 1
        else:
            print(f"Receiver/Channel: ACK {frame} lost in transmission!")
            print(f"Sender: Timeout expired! Retransmitting Frame {frame}...\n")
            # Loop repeats with same frame
            
    print(f"--> All {total_frames} frames transmitted and acknowledged successfully!\n")

def sliding_window(total_frames, window_size):
    print("=" * 55)
    print(f"   SLIDING WINDOW FLOW CONTROL (Window Size = {window_size})")
    print("=" * 55)
    
    base = 0
    while base < total_frames:
        end = min(base + window_size, total_frames)
        current_window = list(range(base, end))
        
        print(f"Current Window: {current_window} (Buffer capacity: {window_size})")
        
        for f in current_window:
            print(f"  Sender: Sending Frame {f}")
            time.sleep(0.3)
            
        print("  Receiver: Frames arrived in order. Processing batch...")
        time.sleep(0.5)
        
        for f in current_window:
            print(f"  Receiver: ACK {f} generated")
            time.sleep(0.2)
            
        print(f"  Sender: Cumulative ACK {end - 1} received. Sliding window forward by {len(current_window)} frames.\n")
        base = end
        
    print(f"--> All {total_frames} frames transmitted and acknowledged successfully!\n")

if __name__ == "__main__":
    n_in = input("Enter total number of frames to send [default: 6]: ").strip()
    total_frames = int(n_in) if n_in else 6
    w_in = input("Enter window size for sliding window [default: 3]: ").strip()
    window_size = int(w_in) if w_in else 3

    stop_and_wait(total_frames)
    sliding_window(total_frames, window_size)

"""
================================================================================
LAB EXPLANATION:
Lab 5 demonstrates Flow Control protocols at the Data Link and Transport layers.
Flow control ensures a fast sender does not overwhelm a slow receiver with more
frames than its internal buffer can process.

1. Stop-and-Wait Flow Control:
   - Principle: The sender transmits exactly ONE frame and halts all further transmissions.
     It starts a timer and waits for the receiver to process the frame and return an Acknowledgment (ACK).
   - Only after receiving the ACK does the sender transmit the next sequential frame.
   - If an ACK is lost or delayed, a timeout expires and the sender retransmits the frame.
   - Efficiency: Low channel utilization when Round-Trip Time (RTT) is high, because the link
     sits completely idle during propagation.

2. Sliding Window Flow Control:
   - Principle: Allows the sender to transmit multiple consecutive frames up to a predefined limit
     (the 'Window Size') before requiring an acknowledgment.
   - As ACKs are received, the lower boundary of the sender's window slides forward, opening up space
     to transmit newer frames without waiting for the entire batch to complete.
   - Efficiency: Significantly increases link utilization and throughput (Pipelining).

CODE EXPLANATION:
- 'stop_and_wait(total_frames)': Loops through each frame. Simulates transmission delay,
  waiting on ACK, randomized ACK loss/timeout with retransmission, and loop increment upon successful ACK.
- 'sliding_window(total_frames, window_size)': Uses pointer 'base' and 'end' to maintain active window
  range. Transmits window frames sequentially, receives cumulative ACK, and slides the window forward.
================================================================================
"""
