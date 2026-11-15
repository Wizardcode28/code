# Sample inputs:
# Choice: 1 (for Linux/Unix network commands) or 2 (for Cisco switch configuration)

import platform, subprocess

def run(cmd):
    try:
        res = subprocess.run(cmd, shell=True, text=True, capture_output=True, timeout=5)
        print(res.stdout or res.stderr)
    except Exception as e:
        print(f"Error: {e}")

def show_linux_basics():
    print("OS:", platform.platform())
    cmds = {
        "IP configuration": "ip addr",
        "Routing table": "ip route",
        "DNS configuration": "cat /etc/resolv.conf",
        "Reachability test": "ping -c 2 127.0.0.1",
    }
    for title, cmd in cmds.items():
        print(f"\n--- {title}: {cmd} ---")
        run(cmd)

def print_switch_config():
    print("""enable
configure terminal
hostname CN-LAB-SW1
no ip domain-lookup
interface range fastEthernet 0/1-4
 switchport mode access
 spanning-tree portfast
 no shutdown
exit
interface vlan 1
 ip address 192.168.1.2 255.255.255.0
 no shutdown
exit
ip default-gateway 192.168.1.1
end
write memory""")

if __name__ == "__main__":
    choice = input("Enter choice (1: Linux commands, 2: Switch config) [default: 1]: ").strip() or "1"
    if choice == "1":
        show_linux_basics()
    elif choice == "2":
        print_switch_config()
    else:
        print("Invalid choice.")
