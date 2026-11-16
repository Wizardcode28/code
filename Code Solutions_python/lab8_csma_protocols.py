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
Lab 8 covers Multiple Access Protocols at the MAC (Medium Access Control) sublayer,
focusing on Carrier Sense Multiple Access (CSMA) variants and Collision Detection (CSMA/CD).

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

CODE EXPLANATION (DETAILED STEP-BY-STEP):
1. Function 'simulate_csma(protocol, stations, slots, p, seed)':
   - Takes 5 parameters:
     * 'protocol': Which MAC strategy to execute ('1-persistent', 'non-persistent', 'p-persistent', 'csma/cd').
     * 'stations': Total competing hosts attached to the shared physical bus (default 5).
     * 'slots': Total discrete time intervals modeled in the simulation (default 10).
     * 'p': Transmission probability parameter for p-persistent mode (default 0.5).
     * 'seed': Deterministic random seed ensuring repeatable results across runs.
   - Counters ('success', 'collision', 'idle') are initialized to 0 to track channel states.
   - Main Loop ('for slot in range(slots)'):
     * Simulates discrete time increments. For every slot, an empty list 'contenders' is reset.
     * Inner Station Contention Loop ('for s in range(stations)'):
       - Step A (Packet Arrival): 'random.random() < 0.40' models an independent 40% probability
         that station 's' has generated an outbound packet to send. Stations without packets skip.
       - Step B (Medium Sensing & Strategy Filter):
         * '1-persistent' & 'csma/cd': Appends station 's' to 'contenders' unconditionally (prob = 1.0).
         * 'non-persistent': 'random.random() < 0.50' models that a waiting station backs off with
           50% probability instead of jumping onto the wire simultaneously.
         * 'p-persistent': 'random.random() < p' checks a random draw against threshold 'p'.
           If true, station transmits; if false, it defers to the subsequent slot.
     * Slot Outcome Arbitration:
       - 'len(contenders) == 0': Channel is silent. Increments 'idle' counter.
       - 'len(contenders) == 1': Perfect transmission. Exactly one station transmitted cleanly.
         Increments 'success' counter.
       - 'len(contenders) > 1': Mutual interference / collision. Increments 'collision' counter.
         In 'csma/cd', the code additionally displays collision detection and abortion of transmission.
   - Efficiency Calculation:
     * 'throughput = (success / slots) * 100' computes the channel efficiency percentage.

2. Function 'comparative_analysis(stations, slots, p, seed)':
   - Runs a Monte Carlo benchmark over 100 slots comparing 'non-persistent', '1-persistent', and 'p-persistent'.
   - Resets 'random.seed(seed)' before each protocol run so all three strategies experience the exact
     same random packet arrival patterns.
   - Outputs a formatted comparative table demonstrating that Non-Persistent minimizes collisions
     at the expense of idle time, 1-Persistent maximizes collisions under load, and P-Persistent
     balances both.

3. '__main__' Block:
   - Takes interactive user inputs for stations, slots, and probability 'p' (with Enter defaults),
     executes step-by-step traces for all four protocols, and prints the 100-slot comparative table.
================================================================================
"""
