# Sample inputs:
# Total frames: 6
# Window size for Sliding Window: 3

def stop_and_wait(total_frames):
    print("\n--- Stop-and-Wait Flow Control ---")
    for i in range(total_frames):
        print(f"Sender: Transmitting Frame {i}")
        print(f"Receiver: Frame {i} received, ACK {i} sent back")
    print(f"Total {total_frames} frames transmitted successfully.")

def sliding_window(total_frames, window_size):
    print(f"\n--- Sliding Window Flow Control (Window Size = {window_size}) ---")
    base = 0
    while base < total_frames:
        end = min(base + window_size, total_frames)
        frames = list(range(base, end))
        print(f"Sender sends window: Frames {frames}")
        print(f"Receiver receives {frames}, sends cumulative ACK {end - 1}")
        base = end
    print(f"Total {total_frames} frames transmitted successfully.")

if __name__ == "__main__":
    n_in = input("Enter total number of frames to send [default: 6]: ").strip()
    total_frames = int(n_in) if n_in else 6
    w_in = input("Enter window size for sliding window [default: 3]: ").strip()
    window_size = int(w_in) if w_in else 3

    stop_and_wait(total_frames)
    sliding_window(total_frames, window_size)
