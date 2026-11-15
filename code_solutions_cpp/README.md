# Computer Networks Lab Solutions (C++ Language)

All C++ solutions mapped strictly 1-to-1 matching the 10 rows of `lab_experiments.png`:

| S. No. | Lab Manual Topic | Manual Lab # | C++ Source File |
|---|---|---|---|
| 1 | Basics of Linux/Unix and Configuration of a simple network topology using switch over Cisco Packet Tracer | 1 | `lab1_linux_switch_topology.cpp` |
| 2 | Basics of socket programming and binding of client server machines | 2 | `lab2_socket_programming_binding.cpp` |
| 3 | Client server application for chat (exchange of messages) | 3 | `lab3_client_server_chat.cpp` |
| 4 | PC to PC file transfer using serial port | 4 | `lab4_serial_file_transfer.cpp` |
| 5 | Implementation of flow control protocols (Stop and Wait, Sliding Window) | 5 | `lab5_flow_control_stop_wait_sliding_window.cpp` |
| 6 | Implementation of error control & detection protocols (Parity, Stop-and-Wait ARQ, GBN ARQ, Selective-Reject ARQ, VRC, LRC, CRC) | 6-7, 8 | `lab6_error_detection_control_arq.cpp` |
| 7 | Implementation of Hamming code for multi-bit error detection and single-bit error correction | 9 | `lab7_hamming_code.cpp` |
| 8 | Implementation of CSMA/CD, non-persistent, 1-Persistent and P-Persistent CSMA protocols | 10-11 | `lab8_csma_protocols.cpp` |
| 9 | Implementation of various routing protocols (RIP and OSPF) | 12-13 | `lab9_routing_rip_ospf.cpp` |
| 10 | Implementation of TCP/IP echo | 14 | `lab10_tcp_ip_echo.cpp` |

---

### Compilation:
To compile any program using `g++`:
```bash
# Standard programs
g++ -o lab5.exe lab5_flow_control_stop_wait_sliding_window.cpp

# Socket-based programs on Windows (requires -lws2_32)
g++ -o lab2.exe lab2_socket_programming_binding.cpp -lws2_32
g++ -o lab3.exe lab3_client_server_chat.cpp -lws2_32
g++ -o lab10.exe lab10_tcp_ip_echo.cpp -lws2_32
```
Each C++ program takes interactive input from the user with standard defaults and contains sample input comments at the top.
