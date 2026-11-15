# Sample inputs:
# Number of stations: 5
# Number of time slots: 10
# Probability p for p-persistent: 0.5

import random

def simulate_slot_trace(protocol, stations=5, slots=10, p=0.5, seed=4):
    random.seed(seed)
    success = collision = idle = 0
    print(f"\n--- {protocol.upper()} ---")
    for slot in range(slots):
        contenders = []
        for s in range(stations):
            if random.random() < 0.4:
                if protocol in ("1-persistent", "csma/cd"):
                    contenders.append(s)
                elif protocol == "non-persistent" and random.random() < 0.5:
                    contenders.append(s)
                elif protocol == "p-persistent" and random.random() < p:
                    contenders.append(s)
        if not contenders:
            idle += 1
            print(f"Slot {slot}: Idle")
        elif len(contenders) == 1:
            success += 1
            print(f"Slot {slot}: Station {contenders[0]} transmitted successfully")
        else:
            collision += 1
            print(f"Slot {slot}: Collision between stations {contenders}")
            if protocol == "csma/cd":
                print("  -> Collision detected! Transmissions aborted immediately.")
    print(f"Result: Success={success}, Collision={collision}, Idle={idle}")

def compare_protocols(stations=8, slots=100, p=0.25, seed=11):
    random.seed(seed)
    print(f"\n--- Comparative Performance (stations={stations}, slots={slots}) ---")
    print(f"{'Protocol':<16} {'Success':<10} {'Collision':<12} {'Idle':<6}")
    print("-" * 46)
    for proto in ("non-persistent", "1-persistent", "p-persistent"):
        random.seed(seed)
        success = collision = idle = 0
        for _ in range(slots):
            active = [i for i in range(stations) if random.random() < 0.2]
            if proto == "non-persistent":
                active = [i for i in active if random.random() < 0.5]
            elif proto == "p-persistent":
                active = [i for i in active if random.random() < p]
            if not active:
                idle += 1
            elif len(active) == 1:
                success += 1
            else:
                collision += 1
        print(f"{proto:<16} {success:<10} {collision:<12} {idle:<6}")

if __name__ == "__main__":
    st_in = input("Enter number of stations [default: 5]: ").strip()
    stations = int(st_in) if st_in else 5
    sl_in = input("Enter number of time slots [default: 10]: ").strip()
    slots = int(sl_in) if sl_in else 10
    p_in = input("Enter probability p for p-persistent [default: 0.5]: ").strip()
    p = float(p_in) if p_in else 0.5

    for proto in ("csma/cd", "non-persistent", "1-persistent", "p-persistent"):
        simulate_slot_trace(proto, stations, slots, p)

    compare_protocols()
