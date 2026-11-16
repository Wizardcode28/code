# Computer Networks Lab Solutions (C++ Language)

All C++ lab solutions have been updated. Labs involving client-server communication now have dedicated subfolders with separate `client.cpp` and `server.cpp` (or `sender.cpp` / `receiver.cpp`) source files.

| S. No. in `.png` | Experiment Content | C++ Source Location |
|:---:|---|---|
| **1** | Linux/Unix Basics & Cisco Switch Configuration | [lab1_linux_switch_topology.cpp](lab1_linux_switch_topology.cpp) |
| **2** | Socket Programming & Client-Server Binding | [lab2_socket_programming/](lab2_socket_programming/)<br>• `server.cpp`<br>• `client.cpp` |
| **3** | Client-Server Chat Application (Full Duplex) | [lab3_client_server_chat/](lab3_client_server_chat/)<br>• `server.cpp`<br>• `client.cpp` |
| **4** | PC-to-PC File Transfer over Serial Port (RS-232 / UART) | [lab4_serial_file_transfer/](lab4_serial_file_transfer/)<br>• `sender.cpp`<br>• `receiver.cpp` |
| **5** | Flow Control (Stop-and-Wait & Sliding Window with Delay & Loss) | [lab5_flow_control_stop_wait_sliding_window.cpp](lab5_flow_control_stop_wait_sliding_window.cpp) |
| **6** | Error Detection & Control (Parity, ARQs, VRC, LRC, CRC) | [lab6_error_detection_control_arq.cpp](lab6_error_detection_control_arq.cpp) |
| **7** | Hamming Code (Single-bit Correction & Multi-bit SECDED Detection) | [lab7_hamming_code.cpp](lab7_hamming_code.cpp) |
| **8** | CSMA/CD, Non-Persistent, 1-Persistent, P-Persistent CSMA | [lab8_csma_protocols.cpp](lab8_csma_protocols.cpp) |
| **9** | Routing Protocols (Distance Vector / RIP & Link State / OSPF) | [lab9_routing_rip_ospf.cpp](lab9_routing_rip_ospf.cpp) |
| **10** | TCP/IP Echo Server & Client with RTT Latency Calculation | [lab10_tcp_ip_echo/](lab10_tcp_ip_echo/)<br>• `server.cpp`<br>• `client.cpp` |

---

### Compilation with g++:
To compile standard single-file labs:
```bash
g++ -o lab5.exe lab5_flow_control_stop_wait_sliding_window.cpp
g++ -o lab6.exe lab6_error_detection_control_arq.cpp
g++ -o lab7.exe lab7_hamming_code.cpp
g++ -o lab8.exe lab8_csma_protocols.cpp
g++ -o lab9.exe lab9_routing_rip_ospf.cpp
```

To compile socket-based client/server programs on Windows (requires `-lws2_32`):
```bash
# Lab 2: Socket Programming
g++ -o server.exe lab2_socket_programming/server.cpp -lws2_32
g++ -o client.exe lab2_socket_programming/client.cpp -lws2_32

# Lab 3: Chat Application
g++ -o server.exe lab3_client_server_chat/server.cpp -lws2_32
g++ -o client.exe lab3_client_server_chat/client.cpp -lws2_32

# Lab 4: Serial Port File Transfer
g++ -o sender.exe lab4_serial_file_transfer/sender.cpp
g++ -o receiver.exe lab4_serial_file_transfer/receiver.cpp

# Lab 10: TCP Echo
g++ -o server.exe lab10_tcp_ip_echo/server.cpp -lws2_32
g++ -o client.exe lab10_tcp_ip_echo/client.cpp -lws2_32
```
