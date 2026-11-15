# Sample inputs:
# Binary data string for Parity and VRC: 1011001
# Equal-length words for LRC (comma-separated): 1011,1100,0110
# CRC generator polynomial: 1101
# Total frames for ARQ simulations: 5
# Window size for Go-Back-N / Selective-Repeat: 3
# Frame index to simulate loss: 2

import random

def parity_bit(data, mode="even"):
    ones = data.count("1")
    return str(ones % 2) if mode == "even" else str(1 - (ones % 2))

def check_parity(codeword, mode="even"):
    return parity_bit(codeword[:-1], mode) == codeword[-1]

def parity_demo(data):
    print("\n--- 1. Even and Odd Parity Check (VRC) ---")
    for mode in ("even", "odd"):
        code = data + parity_bit(data, mode)
        print(f"{mode.capitalize()} Parity -> Data: {data} -> Transmitted: {code} -> Valid: {check_parity(code, mode)}")
        corrupted = code[:-1] + ("1" if code[-1] == "0" else "0")
        print(f"  After 1-bit Error: {corrupted} -> Valid: {check_parity(corrupted, mode)}")

def lrc(datawords, mode="even"):
    result = ""
    for col in range(len(datawords[0])):
        ones = sum(int(word[col]) for word in datawords)
        result += str(ones % 2) if mode == "even" else str(1 - (ones % 2))
    return result

def crc_division(data, generator):
    msg = list(data)
    for i in range(len(data) - len(generator) + 1):
        if msg[i] == "1":
            for j in range(len(generator)):
                msg[i + j] = str(int(msg[i + j] != generator[j]))
    return "".join(msg[-(len(generator) - 1):])

def crc_encode(data, generator):
    rem = crc_division(data + "0" * (len(generator) - 1), generator)
    return data + rem

def crc_check(codeword, generator):
    rem = crc_division(codeword, generator)
    return "1" not in rem

def stop_and_wait_arq(total, loss_frame=1):
    print("\n--- 3. Stop-and-Wait ARQ ---")
    lost = False
    for f in range(total):
        attempt = 1
        while True:
            print(f"Sender sends Frame {f} (Attempt {attempt})")
            if f == loss_frame and not lost:
                print(f"  Frame {f} or ACK lost! Timeout occurred, retransmitting...")
                lost = True
                attempt += 1
            else:
                print(f"  Receiver got Frame {f}, ACK {f} received by Sender")
                break

def go_back_n_arq(total, window, loss_frame):
    print(f"\n--- 4. Go-Back-N ARQ (Window = {window}) ---")
    base = 0
    lost = False
    while base < total:
        end = min(base + window, total)
        print(f"Window [{base} to {end-1}]: Sending frames {list(range(base, end))}")
        ack = base - 1
        for f in range(base, end):
            if f == loss_frame and not lost:
                print(f"  Frame {f} LOST! Receiver discards all subsequent frames.")
                lost = True
                break
            print(f"  Frame {f} received -> ACK {f}")
            ack = f
        if ack >= base:
            base = ack + 1
        else:
            print(f"  Timeout! Go-Back-N retransmits all frames from Frame {base}")

def selective_repeat_arq(total, window, loss_frame):
    print(f"\n--- 5. Selective-Repeat ARQ (Window = {window}) ---")
    acked = set()
    lost = False
    base = 0
    while base < total:
        end = min(base + window, total)
        for f in range(base, end):
            if f in acked:
                continue
            if f == loss_frame and not lost:
                print(f"  Frame {f} LOST! Receiver buffers other frames.")
                lost = True
            else:
                print(f"  Frame {f} received and buffered -> ACK {f}")
                acked.add(f)
        if base == loss_frame and loss_frame not in acked:
            print(f"  Timeout! Retransmitting ONLY lost Frame {base}")
            print(f"  Frame {base} received -> ACK {base}")
            acked.add(base)
        while base in acked:
            base += 1

if __name__ == "__main__":
    msg = input("Enter binary string for Parity/VRC & CRC [default: 1011001]: ").strip() or "1011001"
    blocks_in = input("Enter comma-separated words for LRC [default: 1011,1100,0110]: ").strip()
    blocks = [b.strip() for b in blocks_in.split(",")] if blocks_in else ["1011", "1100", "0110"]
    gen = input("Enter CRC divisor polynomial [default: 1101]: ").strip() or "1101"

    parity_demo(msg)

    print("\n--- 2. Longitudinal Redundancy Check (LRC) & CRC ---")
    print(f"LRC Check Bits for {blocks}: {lrc(blocks, 'even')}")
    crc_code = crc_encode(msg, gen)
    print(f"CRC Codeword: {crc_code}")
    print(f"CRC Verification: {'Valid' if crc_check(crc_code, gen) else 'Error'}")

    tot_in = input("\nEnter total frames for ARQ simulations [default: 5]: ").strip()
    total = int(tot_in) if tot_in else 5
    win_in = input("Enter window size for GBN / Selective-Repeat [default: 3]: ").strip()
    window = int(win_in) if win_in else 3
    loss_in = input("Enter frame index to simulate loss [default: 2]: ").strip()
    loss_frame = int(loss_in) if loss_in else 2

    stop_and_wait_arq(total, min(1, total - 1))
    go_back_n_arq(total, window, loss_frame)
    selective_repeat_arq(total, window, loss_frame)
