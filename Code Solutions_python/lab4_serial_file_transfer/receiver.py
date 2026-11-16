# Sample inputs:
# Serial Port: COM2 (Windows) or /dev/ttyUSB1 (Linux)
# Output File Path: received.txt (or press Enter for default)

import os
import struct

try:
    import serial
except ImportError:
    serial = None

BAUD_RATE = 9600
CHUNK_SIZE = 1024

def receive_file(port, output_filename):
    if not serial:
        print("[Error] pyserial is not installed. Install with: pip install pyserial")
        return
        
    print(f"\n[Receiver] Listening on serial port {port} at {BAUD_RATE} baud...")
    with serial.Serial(port, BAUD_RATE, timeout=15) as ser:
        print("[Receiver] Waiting for sender header packet...")
        raw_len = ser.read(1)
        if not raw_len:
            print("[Receiver] Timed out waiting for transmission header.")
            return
            
        name_len = struct.unpack("!B", raw_len)[0]
        orig_name = ser.read(name_len).decode('utf-8', errors='replace')
        raw_size = ser.read(8)
        file_size = struct.unpack("!Q", raw_size)[0]
        
        print(f"[Receiver] Incoming file detected: '{orig_name}', Size: {file_size} bytes")
        # Send confirmation ACK handshake
        ser.write(b"OK")
        
        received_bytes = 0
        with open(output_filename, "wb") as f:
            while received_bytes < file_size:
                needed = min(CHUNK_SIZE, file_size - received_bytes)
                chunk = ser.read(needed)
                if not chunk:
                    print("\n[Receiver] Connection lost before file completion.")
                    break
                f.write(chunk)
                received_bytes += len(chunk)
                progress = (received_bytes / file_size) * 100
                print(f"\r[Receiver] Progress: {received_bytes}/{file_size} bytes ({progress:.1f}%)", end="")
        print(f"\n[Receiver] File successfully received and saved as '{output_filename}'")

if __name__ == "__main__":
    port = input("Enter receiver serial port (e.g. COM2 or /dev/ttyUSB1) [default: COM2]: ").strip() or "COM2"
    out_file = input("Enter output file path to save [default: received.txt]: ").strip() or "received.txt"
    
    receive_file(port, out_file)

"""
================================================================================
LAB EXPLANATION:
Lab 4 deals with physical layer and data link layer asynchronous serial communication
(RS-232 standard / UART) to transfer binary or text files point-to-point between two PCs.

1. Receiver Side Responsibilities:
   - Synchronizes with the incoming bitstream using start and stop framing bits.
   - Decodes the initial 1-byte header length, extracts the filename, and unpacks the 64-bit integer file size.
   - Replies with a 2-byte 'OK' acknowledgment to signal readiness.
   - Receives and writes incoming payload chunks to disk until the exact expected byte count is fulfilled.

CODE EXPLANATION:
- 'receive_file(port, output_filename)': Listens on the configured COM port, reads and unpacks
  the metadata header via 'struct.unpack("!Q", ...)', writes incoming chunks to the output file,
  and displays reception progress.
================================================================================
"""
