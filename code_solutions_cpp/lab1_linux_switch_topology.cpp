/*
Sample inputs:
Choice: 1 (for Linux/Unix network commands) or 2 (for Cisco switch configuration)
*/

#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

void show_linux_basics() {
    cout << "\n============================================================\n";
    cout << "           LINUX / UNIX NETWORKING COMMANDS\n";
    cout << "============================================================\n";
    
    cout << "\n--- 1. IP Address Configuration ---\n";
    system("ip addr || ifconfig || ipconfig");
    
    cout << "\n--- 2. Routing Table Information ---\n";
    system("ip route || route -n || route print");
    
    cout << "\n--- 3. DNS Configuration ---\n";
    system("cat /etc/resolv.conf || nslookup localhost");
    
    cout << "\n--- 4. Network Reachability / Ping Test ---\n";
    system("ping -c 2 127.0.0.1 || ping -n 2 127.0.0.1");
    
    cout << "\n--- 5. Active Sockets & Port Status ---\n";
    system("netstat -tuln || netstat -an");
}

void print_switch_config() {
    cout << "\n============================================================\n";
    cout << "   CISCO PACKET TRACER - 2960 SWITCH CONFIGURATION\n";
    cout << "============================================================\n";
    cout << "! --- Entering Privileged EXEC & Global Config Mode ---\n"
         << "enable\n"
         << "configure terminal\n"
         << "hostname CN-LAB-SW1\n"
         << "no ip domain-lookup\n\n"
         << "! --- Configuring Access Ports for End Devices (PCs) ---\n"
         << "interface range fastEthernet 0/1-4\n"
         << " description Access ports connected to Lab PCs\n"
         << " switchport mode access\n"
         << " spanning-tree portfast\n"
         << " no shutdown\n"
         << "exit\n\n"
         << "! --- Configuring Switch Virtual Interface (SVI / Management IP) ---\n"
         << "interface vlan 1\n"
         << " description Management VLAN Interface\n"
         << " ip address 192.168.1.2 255.255.255.0\n"
         << " no shutdown\n"
         << "exit\n\n"
         << "! --- Default Gateway & Password Setup ---\n"
         << "ip default-gateway 192.168.1.1\n"
         << "line con 0\n"
         << " password cisco\n"
         << " login\n"
         << "exit\n\n"
         << "! --- Save Running Configuration to NVRAM ---\n"
         << "end\n"
         << "write memory\n\n";

    cout << "[Packet Tracer Steps]\n"
         << "1. Drag a generic 2960 switch and 3 PCs onto the workspace.\n"
         << "2. Connect PC0 (Fa0) to Switch (Fa0/1) using Copper Straight-Through cable.\n"
         << "3. Repeat for PC1 -> Fa0/2, PC2 -> Fa0/3.\n"
         << "4. Assign IP addresses in Desktop -> IP Configuration (e.g., 192.168.1.10, 192.168.1.11).\n"
         << "5. Open PC Command Prompt and verify connectivity: ping 192.168.1.11\n";
}

int main() {
    cout << "Enter choice (1: Linux Network Commands, 2: Cisco Switch Config) [default 1]: ";
    string line;
    int choice = 1;
    if (getline(cin, line) && !line.empty()) {
        try { choice = stoi(line); } catch (...) { choice = 1; }
    }

    if (choice == 1) {
        show_linux_basics();
    } else if (choice == 2) {
        print_switch_config();
    } else {
        cout << "Invalid choice selected.\n";
    }
    return 0;
}

/*
================================================================================
LAB EXPLANATION:
Lab 1 introduces fundamental Linux/Unix networking diagnostic utilities and basic
network topology design on Cisco Packet Tracer using Layer-2 switches.

1. Linux/Unix Networking Commands:
   - 'ip addr' / 'ifconfig': Inspects network interfaces, MAC addresses, and assigned IP addresses.
   - 'ip route' / 'netstat -r': Displays kernel routing tables and default gateway paths.
   - 'cat /etc/resolv.conf': Shows DNS nameservers for hostname resolution.
   - 'ping': Uses ICMP Echo Request/Reply messages to test reachability and round-trip time.
   - 'netstat': Displays active connections, listening ports, and protocol statistics.

2. Switched Network Topology (Cisco Packet Tracer):
   - A Layer-2 switch operates at the Data Link layer, using MAC address tables (CAM tables)
     to forward Ethernet frames selectively to destination ports, eliminating collisions.
   - Workstations connected with Copper Straight-Through cables into the same VLAN subnet
     can communicate directly at line speed.

CODE EXPLANATION:
- 'show_linux_basics()': Executes diagnostic commands via system() calls with cross-platform fallbacks.
- 'print_switch_config()': Prints complete Cisco IOS commands for configuring VLAN 1, access ports,
  and provides step-by-step guidance for setting up the topology in Cisco Packet Tracer.
================================================================================
*/
