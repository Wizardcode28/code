# Sample inputs:
# Mode: send or receive
# Serial Port: COM1 (Windows) or /dev/ttyUSB0 (Linux)
# File path: sample.txt or received.txt

import os, struct, time

try:
    import serial
except ImportError:
    serial = None

BAUD = 9600
CHUNK = 1024

def send_file(port, filename):
    if not serial:
        print("Please install pyserial first: pip install pyserial")
        return
    if not os.path.exists(filename):
        print(f"File {filename} does not exist.")
        return
    size = os.path.getsize(filename)
    name = os.path.basename(filename).encode()[:255]
    with serial.Serial(port, BAUD, timeout=2) as ser:
        time.sleep(2)
        ser.write(struct.pack("!B", len(name)) + name + struct.pack("!Q", size))
        if ser.read(2) != b"OK":
            print("Receiver did not respond with OK")
            return
        with open(filename, "rb") as f:
            while chunk := f.read(CHUNK):
                ser.write(chunk)
        print("File sent successfully.")

def receive_file(port, output_file):
    if not serial:
        print("Please install pyserial first: pip install pyserial")
        return
    with serial.Serial(port, BAUD, timeout=10) as ser:
        print(f"Waiting for incoming file on {port}...")
        hdr = ser.read(1)
        if not hdr:
            print("Timed out waiting for file header")
            return
        name_len = struct.unpack("!B", hdr)[0]
        ser.read(name_len)
        size = struct.unpack("!Q", ser.read(8))[0]
        ser.write(b"OK")
        received = 0
        with open(output_file, "wb") as f:
            while received < size:
                chunk = ser.read(min(CHUNK, size - received))
                if not chunk:
                    break
                f.write(chunk)
                received += len(chunk)
        print(f"File received and saved to {output_file}")

if __name__ == "__main__":
    mode = input("Enter mode (send/receive) [default: send]: ").strip().lower() or "send"
    port = input("Enter serial port (e.g. COM3 or /dev/ttyUSB0) [default: COM1]: ").strip() or "COM1"
    filepath = input("Enter file path [default: sample.txt]: ").strip() or "sample.txt"
    if mode == "send":
        send_file(port, filepath)
    else:
        receive_file(port, filepath)
