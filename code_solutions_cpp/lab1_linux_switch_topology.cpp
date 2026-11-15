/*
Sample inputs:
Choice: 1 (for Linux/Unix network commands) or 2 (for Cisco switch configuration)
*/

#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

void show_linux_basics() {
    cout << "\n--- Linux/Unix Network Commands ---\n";
    cout << "\n[IP Configuration]:\n";
    system("ip addr || ipconfig");
    cout << "\n[Routing Table]:\n";
    system("ip route || route print");
    cout << "\n[Ping Test]:\n";
    system("ping -c 2 127.0.0.1 || ping -n 2 127.0.0.1");
}

void print_switch_config() {
    cout << "\n--- Cisco Switch Configuration ---\n";
    cout << "enable\n"
         << "configure terminal\n"
         << "hostname CN-LAB-SW1\n"
         << "no ip domain-lookup\n"
         << "interface range fastEthernet 0/1-4\n"
         << " switchport mode access\n"
         << " spanning-tree portfast\n"
         << " no shutdown\n"
         << "exit\n"
         << "interface vlan 1\n"
         << " ip address 192.168.1.2 255.255.255.0\n"
         << " no shutdown\n"
         << "exit\n"
         << "ip default-gateway 192.168.1.1\n"
         << "end\n"
         << "write memory\n";
}

int main() {
    cout << "Enter choice (1: Linux commands, 2: Switch config) [default 1]: ";
    string line;
    int choice = 1;
    if (getline(cin, line) && !line.empty()) {
        try { choice = stoi(line); } catch (...) { choice = 1; }
    }

    if (choice == 1) {
        show_linux_basics();
    } else {
        print_switch_config();
    }
    return 0;
}
