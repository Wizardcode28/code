# Sample inputs:
# Serial Port: COM1 (Windows) or /dev/ttyUSB0 (Linux)
# File Path: sample.txt (or press Enter for default)

import os
import struct
import time

try:
    import serial
except ImportError:
    serial = None

BAUD_RATE = 9600
CHUNK_SIZE = 1024

def send_file(port, filename):
    if not serial:
        print("[Error] pyserial is not installed. Install with: pip install pyserial")
        return
    if not os.path.exists(filename):
        print(f"[Error] Source file '{filename}' not found.")
        return
        
    file_size = os.path.getsize(filename)
    raw_name = os.path.basename(filename).encode('utf-8')[:255]
    name_len = len(raw_name)

    print(f"\n[Sender] Opening serial port {port} at {BAUD_RATE} baud...")
    with serial.Serial(port, BAUD_RATE, timeout=3) as ser:
        time.sleep(2)  # Wait for adapter reset
        
        # Header Protocol: [1 byte name_len] + [name bytes] + [8 bytes file_size]
        header = struct.pack("!B", name_len) + raw_name + struct.pack("!Q", file_size)
        print(f"[Sender] Sending header: File='{os.path.basename(filename)}', Size={file_size} bytes...")
        ser.write(header)
        
        # Await Handshake ACK from Receiver
        ack = ser.read(2)
        if ack != b"OK":
            print("[Sender] Error: Receiver did not respond with 'OK'.")
            return
            
        print("[Sender] Handshake confirmed! Transmitting file chunks...")
        bytes_sent = 0
        with open(filename, "rb") as f:
            while chunk := f.read(CHUNK_SIZE):
                ser.write(chunk)
                bytes_sent += len(chunk)
                progress = (bytes_sent / file_size) * 100
                print(f"\r[Sender] Progress: {bytes_sent}/{file_size} bytes ({progress:.1f}%)", end="")
        print("\n[Sender] File transfer finished successfully!")

if __name__ == "__main__":
    port = input("Enter sender serial port (e.g. COM1 or /dev/ttyUSB0) [default: COM1]: ").strip() or "COM1"
    filepath = input("Enter file path to send [default: sample.txt]: ").strip() or "sample.txt"
    
    if not os.path.exists(filepath):
        with open(filepath, "w") as f:
            f.write("Sample file for PC-to-PC Serial Transfer Lab.\n" * 20)
        print(f"[Info] Created dummy file '{filepath}' for testing.")
        
    send_file(port, filepath)

"""
================================================================================
LAB EXPLANATION:
Lab 4 deals with physical layer and data link layer asynchronous serial communication
(RS-232 standard / UART) to transfer binary or text files point-to-point between two PCs.

1. Sender Side Responsibilities:
   - Sets the baud rate, character length, parity, and stop bits on the serial interface.
   - Formulates a framing header packet containing metadata (filename length, filename, file size).
   - Awaits an acknowledgment ('OK') handshake from the receiver before releasing payload bytes.
   - Streams file bytes sequentially in fixed-size buffers (1024 bytes) until all bytes are transmitted.

CODE EXPLANATION:
- 'send_file(port, filename)': Opens serial port with 'serial.Serial', serializes the binary header
  using 'struct.pack("!B", ...)' in network byte order, verifies receiver readiness, and transmits chunks.
================================================================================
"""
