# Sample inputs:
# Choice: 1 (for Linux/Unix network commands) or 2 (for Cisco switch configuration)

import platform
import subprocess

def run_command(cmd):
    try:
        res = subprocess.run(cmd, shell=True, text=True, capture_output=True, timeout=5)
        output = res.stdout if res.stdout else res.stderr
        print(output if output else "[Command executed with no output]")
    except Exception as e:
        print(f"Error running command: {e}")

def show_linux_basics():
    print("=" * 60)
    print("           LINUX / UNIX NETWORKING COMMANDS")
    print("=" * 60)
    print("Operating System:", platform.system(), platform.release())
    
    commands = {
        "1. IP Address Configuration": "ip addr || ifconfig || ipconfig",
        "2. Routing Table Information": "ip route || route -n || route print",
        "3. DNS Configuration": "cat /etc/resolv.conf || nslookup localhost",
        "4. Network Reachability / Ping Test": "ping -c 2 127.0.0.1 || ping -n 2 127.0.0.1",
        "5. Active Sockets & Port Status": "netstat -tuln || netstat -an"
    }

    for title, cmd in commands.items():
        print(f"\n--- {title} ---")
        print(f"Executing: {cmd}")
        run_command(cmd)

def print_switch_config():
    print("=" * 60)
    print("   CISCO PACKET TRACER - 2960 SWITCH CONFIGURATION")
    print("=" * 60)
    config = """
! --- Entering Privileged EXEC & Global Config Mode ---
enable
configure terminal
hostname CN-LAB-SW1
no ip domain-lookup

! --- Configuring Access Ports for End Devices (PCs) ---
interface range fastEthernet 0/1-4
 description Access ports connected to Lab PCs
 switchport mode access
 spanning-tree portfast
 no shutdown
exit

! --- Configuring Switch Virtual Interface (SVI / Management IP) ---
interface vlan 1
 description Management VLAN Interface
 ip address 192.168.1.2 255.255.255.0
 no shutdown
exit

! --- Default Gateway & Password Setup ---
ip default-gateway 192.168.1.1
line con 0
 password cisco
 login
exit

! --- Save Running Configuration to NVRAM ---
end
write memory
"""
    print(config.strip())
    print("\n[Packet Tracer Steps]")
    print("1. Drag a generic 2960 switch and 3 PCs onto the workspace.")
    print("2. Connect PC0 (FastEthernet 0) to Switch (Fa0/1) using Copper Straight-Through cable.")
    print("3. Repeat for PC1 -> Fa0/2, PC2 -> Fa0/3.")
    print("4. Assign IP addresses in Desktop -> IP Configuration (e.g., 192.168.1.10, 192.168.1.11, 192.168.1.12).")
    print("5. Open PC Command Prompt and verify connectivity: ping 192.168.1.11")

if __name__ == "__main__":
    choice = input("Enter choice (1: Linux Network Commands, 2: Cisco Switch Config) [default: 1]: ").strip() or "1"
    if choice == "1":
        show_linux_basics()
    elif choice == "2":
        print_switch_config()
    else:
        print("Invalid choice selected.")

"""
================================================================================
LAB EXPLANATION:
Lab 1 introduces fundamental Linux/Unix networking diagnostic utilities and basic
network topology design on Cisco Packet Tracer using Layer-2 switches.

1. Linux/Unix Networking Commands:
   - 'ip addr' / 'ifconfig': Inspects network interfaces, MAC addresses, and assigned IPv4/IPv6 addresses.
   - 'ip route' / 'netstat -r': Displays kernel routing tables and default gateway paths.
   - 'cat /etc/resolv.conf': Shows nameservers responsible for resolving hostnames to IP addresses.
   - 'ping': Uses ICMP Echo Request/Reply messages to test reachability and round-trip time.
   - 'netstat': Displays active connections, listening ports, and protocol statistics.

2. Switched Network Topology (Cisco Packet Tracer):
   - A Layer-2 switch operates at the Data Link layer, using MAC address tables (CAM tables)
     to forward Ethernet frames selectively to destination ports, eliminating collisions.
   - Connecting workstations with Copper Straight-Through cables into the same VLAN subnet
     allows direct point-to-point frame forwarding without needing a router.

CODE EXPLANATION:
- 'run_command(cmd)': Executes shell commands safely using Python's 'subprocess.run' with
  a 5-second timeout, cross-compatible with fallback commands (Linux and Windows).
- 'show_linux_basics()': Queries system parameters and runs diagnostic networking utilities.
- 'print_switch_config()': Outputs a complete, syntax-correct Cisco IOS configuration script
  for configuring access ports, VLAN 1 management interface, and spanning-tree portfast,
  along with step-by-step instructions for Packet Tracer GUI simulation.
- '__main__': Provides an interactive menu for running either command diagnostics or configuration output.
================================================================================
"""
