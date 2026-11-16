# Sample inputs:
# Number of stations: 5
# Number of time slots: 10
# Transmission probability p for p-persistent: 0.5

import random

def simulate_csma(protocol, stations=5, slots=10, p=0.5, seed=42):
    random.seed(seed)
    success = collision = idle = 0
    print("\n" + "=" * 60)
    print(f" {protocol.upper()} CSMA SIMULATION (Stations={stations}, Slots={slots})")
    print("=" * 60)
    
    for slot in range(slots):
        contenders = []
        for s in range(stations):
            # Check if station has a packet ready to transmit (e.g. 40% arrival rate)
            has_data = random.random() < 0.40
            if not has_data:
                continue
                
            # Channel is sensed idle at slot start; apply persistence logic:
            if protocol == "1-persistent":
                # Transmits with probability 1 as soon as channel is sensed idle
                contenders.append(s)
            elif protocol == "non-persistent":
                # If channel idle, transmits; if busy/wait, waits a random backoff time
                if random.random() < 0.50:
                    contenders.append(s)
            elif protocol == "p-persistent":
                # Transmits with probability p; defers with probability (1 - p) to next slot
                if random.random() < p:
                    contenders.append(s)
            elif protocol == "csma/cd":
                # 1-persistent sense, but detects collision mid-slot and aborts immediately
                contenders.append(s)
                
        if len(contenders) == 0:
            idle += 1
            print(f"Slot {slot:2d}: [IDLE] No station transmitted.")
        elif len(contenders) == 1:
            success += 1
            print(f"Slot {slot:2d}: [SUCCESS] Station {contenders[0]} seized channel and transmitted!")
        else:
            collision += 1
            print(f"Slot {slot:2d}: [COLLISION] Multiple stations {contenders} transmitted simultaneously!")
            if protocol == "csma/cd":
                print("         --> Collision Detected! Jam signal broadcasted, stations aborted & backed off.")
                
    throughput = (success / slots) * 100
    print(f"\nSummary for {protocol.upper()}:")
    print(f"  Successful Transmissions: {success}")
    print(f"  Collisions Encountered:   {collision}")
    print(f"  Idle Slots:               {idle}")
    print(f"  Channel Efficiency:       {throughput:.1f}%\n")
    return success, collision, idle

def comparative_analysis(stations=8, slots=100, p=0.25, seed=11):
    print("=" * 60)
    print(f" COMPARATIVE CSMA PROTOCOL PERFORMANCE (Monte Carlo 100 Slots)")
    print("=" * 60)
    print(f"{'Protocol':<18} {'Success':<10} {'Collision':<12} {'Idle':<8} {'Efficiency'}")
    print("-" * 58)
    
    for proto in ("non-persistent", "1-persistent", "p-persistent"):
        random.seed(seed)
        success = collision = idle = 0
        for _ in range(slots):
            active = [s for s in range(stations) if random.random() < 0.20]
            if proto == "non-persistent":
                active = [s for s in active if random.random() < 0.50]
            elif proto == "p-persistent":
                active = [s for s in active if random.random() < p]
                
            if len(active) == 0:
                idle += 1
            elif len(active) == 1:
                success += 1
            else:
                collision += 1
                
        eff = (success / slots) * 100
        print(f"{proto:<18} {success:<10} {collision:<12} {idle:<8} {eff:.1f}%")

if __name__ == "__main__":
    st_in = input("Enter number of stations [default: 5]: ").strip()
    stations = int(st_in) if st_in else 5
    sl_in = input("Enter number of time slots [default: 10]: ").strip()
    slots = int(sl_in) if sl_in else 10
    p_in = input("Enter probability p for p-persistent [default: 0.5]: ").strip()
    p = float(p_in) if p_in else 0.5

    for proto in ("csma/cd", "non-persistent", "1-persistent", "p-persistent"):
        simulate_csma(proto, stations, slots, p)

    comparative_analysis()

"""
================================================================================
LAB EXPLANATION:
Lab 8 covers Multiple Access Protocols at the MAC sublayer, focusing on Carrier
Sense Multiple Access (CSMA) variants and Collision Detection (CSMA/CD).

1. Carrier Sense Principle ("Listen Before Talk"):
   - Stations listen to the physical medium (carrier) before transmitting.
   - If medium is busy, the station defers to prevent collision.

2. Persistence Strategies:
   - 1-Persistent CSMA: When channel is idle, station transmits immediately with probability 1.
     High throughput at low loads, but suffers severe collisions when multiple stations wait for
     a busy channel to become free and transmit simultaneously.
   - Non-Persistent CSMA: If channel is idle, transmits. If busy, does NOT continuously listen;
     instead, waits a random backoff time before sensing again. Reduces collisions significantly,
     but increases channel idle time.
   - P-Persistent CSMA (Used in slotted channels): If channel is idle, transmits with probability 'p',
     and defers to the next slot with probability '(1 - p)'. Balances collisions and idle delay.

3. CSMA/CD (Carrier Sense Multiple Access with Collision Detection):
   - Used in classic Ethernet (IEEE 802.3).
   - "Listen while talking": Station continues monitoring the channel during transmission.
   - If signal voltage doubles or distortion is detected, a collision occurred. Station immediately
     aborts transmission, sends a Jam signal, and executes Exponential Backoff algorithm.

CODE EXPLANATION:
- 'simulate_csma()': Simulates discrete time slots where stations randomly generate frames.
  Evaluates contending stations based on the chosen persistence rule, tracking Success, Collision, and Idle.
- 'comparative_analysis()': Runs an extended 100-slot comparative benchmark displaying a formatted
  performance comparison table of throughput and collision rates across persistence types.
================================================================================
"""
