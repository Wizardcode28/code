# Computer Networks Lab Solutions (Python)

All Python lab solutions have been updated. Labs involving both client and server programs now have dedicated subfolders containing separate `client.py` and `server.py` files.

| S. No. in `.png` | Experiment Content | Python Source Location |
|:---:|---|---|
| **1** | Linux/Unix Basics & Cisco Switch Configuration | [lab1_linux_switch_topology.py](lab1_linux_switch_topology.py) |
| **2** | Socket Programming & Client-Server Binding | [lab2_socket_programming/](lab2_socket_programming/)<br>• `server.py`<br>• `client.py` |
| **3** | Client-Server Chat Application (Full Duplex) | [lab3_client_server_chat/](lab3_client_server_chat/)<br>• `server.py`<br>• `client.py` |
| **4** | PC-to-PC File Transfer over Serial Port (RS-232 / UART) | [lab4_serial_file_transfer/](lab4_serial_file_transfer/)<br>• `sender.py`<br>• `receiver.py` |
| **5** | Flow Control (Stop-and-Wait & Sliding Window with Delay & Loss) | [lab5_flow_control_stop_wait_sliding_window.py](lab5_flow_control_stop_wait_sliding_window.py) |
| **6** | Error Detection & Control (Parity, ARQs, VRC, LRC, CRC) | [lab6_error_detection_control_arq.py](lab6_error_detection_control_arq.py) |
| **7** | Hamming Code (Single-bit Correction & Multi-bit SECDED Detection) | [lab7_hamming_code.py](lab7_hamming_code.py) |
| **8** | CSMA/CD, Non-Persistent, 1-Persistent, P-Persistent CSMA | [lab8_csma_protocols.py](lab8_csma_protocols.py) |
| **9** | Routing Protocols (Distance Vector / RIP & Link State / OSPF) | [lab9_routing_rip_ospf.py](lab9_routing_rip_ospf.py) |
| **10** | TCP/IP Echo Server & Client with RTT Latency Calculation | [lab10_tcp_ip_echo/](lab10_tcp_ip_echo/)<br>• `server.py`<br>• `client.py` |

---

### Running Client-Server Labs:
1. Open **Terminal 1** and start the server:
   ```bash
   python solutions/"Code Solutions_python"/lab2_socket_programming/server.py
   ```
2. Open **Terminal 2** and run the client:
   ```bash
   python solutions/"Code Solutions_python"/lab2_socket_programming/client.py
   ```
*(Same procedure applies to `lab3_client_server_chat`, `lab4_serial_file_transfer`, and `lab10_tcp_ip_echo`).*
