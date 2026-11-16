# Sample inputs:
# Binary data string for Parity and VRC: 1011001
# Equal-length words for LRC (comma-separated): 1011,1100,0110
# CRC generator polynomial: 1101
# Total frames for ARQ simulations: 5
# Window size for Go-Back-N / Selective-Repeat: 3
# Frame index to simulate loss: 2

import time

# -------------------------------------------------------------
# 1. PARITY CHECK & VERTICAL REDUNDANCY CHECK (VRC)
# -------------------------------------------------------------
def parity_bit(data, mode="even"):
    ones = data.count("1")
    return str(ones % 2) if mode == "even" else str(1 - (ones % 2))

def check_parity(codeword, mode="even"):
    return parity_bit(codeword[:-1], mode) == codeword[-1]

def parity_vrc_demo(data):
    print("\n" + "=" * 60)
    print(" 1. PARITY CHECK & VERTICAL REDUNDANCY CHECK (VRC)")
    print("=" * 60)
    print(f"Original Data Word: {data} (Number of 1s: {data.count('1')})")
    
    for mode in ("even", "odd"):
        p_bit = parity_bit(data, mode)
        codeword = data + p_bit
        valid = check_parity(codeword, mode)
        print(f"[{mode.upper()} Parity] Bit added: '{p_bit}' -> Transmitted Codeword: {codeword} (Valid: {valid})")
        
        # Simulate single-bit error
        corrupted = codeword[:-1] + ("1" if codeword[-1] == "0" else "0")
        detected = not check_parity(corrupted, mode)
        print(f"  --> Inverting 1 bit -> Corrupted Codeword: {corrupted} (Error Detected: {detected})")

# -------------------------------------------------------------
# 2. 2D PARITY (VRC ROW + LRC COLUMN) & CYCLIC REDUNDANCY CHECK (CRC)
# -------------------------------------------------------------
def compute_2d_parity(datawords, mode="even"):
    # Row-wise Parity (VRC for each dataword)
    rows_with_vrc = []
    for word in datawords:
        ones = word.count("1")
        p = str(ones % 2) if mode == "even" else str(1 - (ones % 2))
        rows_with_vrc.append(word + p)
        
    # Column-wise Parity (LRC block check across all rows including VRC column)
    lrc_row = ""
    col_len = len(rows_with_vrc[0])
    for col in range(col_len):
        col_ones = sum(int(row[col]) for row in rows_with_vrc)
        lrc_row += str(col_ones % 2) if mode == "even" else str(1 - (col_ones % 2))
        
    return rows_with_vrc, lrc_row

def crc_division(data, generator):
    msg = list(data)
    for i in range(len(data) - len(generator) + 1):
        if msg[i] == "1":
            for j in range(len(generator)):
                msg[i + j] = str(int(msg[i + j] != generator[j]))
    return "".join(msg[-(len(generator) - 1):])

def crc_encode(data, generator):
    dividend = data + "0" * (len(generator) - 1)
    remainder = crc_division(dividend, generator)
    return data + remainder

def crc_check(codeword, generator):
    remainder = crc_division(codeword, generator)
    return "1" not in remainder

def lrc_crc_demo(data, blocks, generator):
    print("\n" + "=" * 60)
    print(" 2. TWO-DIMENSIONAL PARITY (VRC + LRC) & CRC")
    print("=" * 60)
    
    rows_vrc, lrc_row = compute_2d_parity(blocks, "even")
    print("[2D Parity Matrix: Datawords + Row VRC]")
    for r in rows_vrc:
        print(f"  Data: {r[:-1]} | Row VRC Parity: {r[-1]}")
    print("  " + "-" * 32)
    print(f"  LRC Column Parity: {lrc_row[:-1]} | Corner Parity: {lrc_row[-1]}")
    print(f"  Full LRC Block Check: {lrc_row}")

    print(f"\n[Cyclic Redundancy Check (CRC)]")
    print(f"  Data: {data}, Divisor Generator: {generator}")
    crc_code = crc_encode(data, generator)
    print(f"  Appended CRC Remainder: {crc_code[len(data):]}")
    print(f"  Transmitted Codeword: {crc_code}")
    print(f"  Receiver Syndrome Check (Uncorrupted): {'PASS (No Error)' if crc_check(crc_code, generator) else 'FAIL'}")
    
    corrupted_crc = crc_code[:-1] + ("1" if crc_code[-1] == "0" else "0")
    print(f"  Receiver Syndrome Check (1-bit Error: {corrupted_crc}): {'PASS' if crc_check(corrupted_crc, generator) else 'FAIL (Error Detected)'}")

# -------------------------------------------------------------
# 3. STOP-AND-WAIT ARQ SIMULATION
# -------------------------------------------------------------
def stop_and_wait_arq(total, loss_frame=1):
    print("\n" + "=" * 60)
    print(" 3. STOP-AND-WAIT ARQ (AUTOMATIC REPEAT REQUEST)")
    print("=" * 60)
    lost_simulated = False
    f = 0
    while f < total:
        print(f"Sender: Transmitting Frame {f}")
        time.sleep(0.3)
        if f == loss_frame and not lost_simulated:
            print(f"  [Channel Error] Frame {f} corrupted/lost in transit!")
            print(f"  [Sender Timer] Timeout expired! Retransmitting Frame {f}...")
            lost_simulated = True
        else:
            print(f"  Receiver: Frame {f} verified. Sending ACK {f}")
            print(f"  Sender: ACK {f} received. Sliding to next frame.")
            f += 1
    print(f"--> Stop-and-Wait ARQ completed for {total} frames.")

# -------------------------------------------------------------
# 4. GO-BACK-N ARQ SIMULATION
# -------------------------------------------------------------
def go_back_n_arq(total, window, loss_frame):
    print("\n" + "=" * 60)
    print(f" 4. GO-BACK-N ARQ (Sliding Window Size = {window})")
    print("=" * 60)
    base = 0
    lost = False
    while base < total:
        end = min(base + window, total)
        current_batch = list(range(base, end))
        print(f"\nSender Window [{base} to {end - 1}]: Transmitting frames {current_batch}")
        time.sleep(0.3)
        
        ack = base - 1
        for f in current_batch:
            if f == loss_frame and not lost:
                print(f"  [Error] Frame {f} was LOST during transmission!")
                print(f"  Receiver: Discarding frame {f} and all subsequent out-of-order frames!")
                lost = True
                break
            print(f"  Receiver: Frame {f} OK -> Sent ACK {f}")
            ack = f
            
        if ack >= base:
            base = ack + 1
        else:
            print(f"  [Timeout] Go-Back-N Rule: Retransmitting ALL frames in window starting from Frame {base}!")
    print(f"--> Go-Back-N ARQ successfully completed for {total} frames.")

# -------------------------------------------------------------
# 5. SELECTIVE-REPEAT ARQ SIMULATION
# -------------------------------------------------------------
def selective_repeat_arq(total, window, loss_frame):
    print("\n" + "=" * 60)
    print(f" 5. SELECTIVE-REPEAT ARQ (Window Size = {window})")
    print("=" * 60)
    acked = set()
    lost = False
    base = 0
    while base < total:
        end = min(base + window, total)
        current_batch = [f for f in range(base, end) if f not in acked]
        print(f"\nSender Window [{base} to {end - 1}]: Sending unacknowledged frames {current_batch}")
        time.sleep(0.3)
        
        for f in range(base, end):
            if f in acked:
                continue
            if f == loss_frame and not lost:
                print(f"  [Error] Frame {f} LOST!")
                lost = True
            else:
                print(f"  Receiver: Frame {f} received. Stored in out-of-order buffer -> ACK {f} sent.")
                acked.add(f)
                
        if base == loss_frame and loss_frame not in acked:
            print(f"  [Timeout] Selective-Repeat Rule: Retransmitting ONLY missing Frame {base}!")
            time.sleep(0.3)
            print(f"  Receiver: Frame {base} received -> ACK {base} sent.")
            acked.add(base)
            
        while base in acked and base < total:
            base += 1
            
    print(f"--> Selective-Repeat ARQ successfully completed for {total} frames.")

if __name__ == "__main__":
    msg = input("Enter binary string for Parity/VRC & CRC [default: 1011001]: ").strip() or "1011001"
    blocks_in = input("Enter comma-separated words for LRC [default: 1011,1100,0110]: ").strip()
    blocks = [b.strip() for b in blocks_in.split(",")] if blocks_in else ["1011", "1100", "0110"]
    gen = input("Enter CRC divisor polynomial [default: 1101]: ").strip() or "1101"

    parity_vrc_demo(msg)
    lrc_crc_demo(msg, blocks, gen)

    tot_in = input("\nEnter total frames for ARQ simulations [default: 5]: ").strip()
    total = int(tot_in) if tot_in else 5
    win_in = input("Enter window size for GBN / Selective-Repeat [default: 3]: ").strip()
    window = int(win_in) if win_in else 3
    loss_in = input("Enter frame index to simulate loss [default: 2]: ").strip()
    loss_frame = int(loss_in) if loss_in else 2

    stop_and_wait_arq(total, min(loss_frame, total - 1))
    go_back_n_arq(total, window, loss_frame)
    selective_repeat_arq(total, window, loss_frame)

"""
================================================================================
LAB EXPLANATION:
Lab 6 implements comprehensive Error Detection and Error Control (ARQ) mechanisms
operating at the Data Link layer.

1. Error Detection Techniques:
   - Parity / VRC (Vertical Redundancy Check): Appends a single bit to make total 1s even or odd.
     Detects single-bit errors; fails for double-bit errors.
   - LRC (Longitudinal Redundancy Check): Organizes data into blocks and computes parity for each
     column (2D parity check), catching burst errors across rows.
   - CRC (Cyclic Redundancy Check): Polynomial division in modulo-2 arithmetic (XOR). The data is
     multiplied by 2^(n-1) and divided by generator G(x). The remainder is appended. If remainder
     at receiver is 0, transmission is error-free.

2. Automatic Repeat reQuest (ARQ) Protocols:
   - Stop-and-Wait ARQ: Sender transmits 1 frame and waits. On timeout/loss, retransmits that single frame.
   - Go-Back-N (GBN) ARQ: Uses a sliding window of size N. The receiver accepts ONLY in-order frames.
     If frame 'k' is lost, all subsequent frames (k+1, k+2...) are discarded by the receiver, forcing
     the sender upon timeout to roll back and retransmit the entire window starting from 'k'.
   - Selective-Repeat (SR) ARQ: Both sender and receiver maintain sliding windows of size N.
     The receiver buffers valid out-of-order frames. Upon timeout, the sender retransmits ONLY the
     specific frame that was lost, saving bandwidth.

CODE EXPLANATION:
- 'parity_vrc_demo()': Computes even/odd parity bit, constructs codeword, validates parity, and inverts 1 bit.
- 'lrc_crc_demo()': Computes column-wise parity for LRC and performs modulo-2 division for CRC encoding/checking.
- 'stop_and_wait_arq()': Simulates single-frame timeout and retransmission on loss.
- 'go_back_n_arq()': Simulates receiver rejecting out-of-order frames, forcing sender to retransmit the window.
- 'selective_repeat_arq()': Uses an acknowledgment set 'acked' and receiver buffer to resend only lost frames.
================================================================================
"""
