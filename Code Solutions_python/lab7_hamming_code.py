# Sample inputs:
# Binary data word: 1011
# Error bit position to corrupt (1 to codeword length, or 0 for none): 3

def encode_hamming(data):
    m = len(data)
    r = 0
    while 2**r < m + r + 1:
        r += 1
    n = m + r
    code = ["0"] * (n + 1)
    j = 0
    for pos in range(1, n + 1):
        if pos & (pos - 1):
            code[pos] = data[j]
            j += 1
    for i in range(r):
        p = 2**i
        parity = 0
        for pos in range(1, n + 1):
            if pos & p:
                parity ^= int(code[pos])
        code[p] = str(parity)
    return "".join(code[1:])

def decode_hamming(codeword):
    n = len(codeword)
    code = ["0"] + list(codeword)
    syndrome = 0
    p = 1
    while p <= n:
        parity = 0
        for pos in range(1, n + 1):
            if pos & p:
                parity ^= int(code[pos])
        if parity:
            syndrome += p
        p *= 2
    if 0 < syndrome <= n:
        code[syndrome] = "1" if code[syndrome] == "0" else "0"
    corrected = "".join(code[1:])
    data = "".join(bit for pos, bit in enumerate(corrected, 1) if pos & (pos - 1))
    return corrected, syndrome, data

def add_overall_parity(codeword):
    return codeword + str(codeword.count("1") % 2)

def secded_check(block):
    codeword, p_bit = block[:-1], int(block[-1])
    corrected, syn, data = decode_hamming(codeword)
    parity_valid = (block.count("1") % 2 == 0)
    if syn == 0 and parity_valid:
        return "No Error detected", codeword, data
    elif syn != 0 and not parity_valid:
        return f"Single-bit error corrected at bit position {syn}", corrected, data
    else:
        return "Double-bit error detected (Cannot be corrected)", codeword, None

if __name__ == "__main__":
    msg = input("Enter binary data word [default: 1011]: ").strip() or "1011"
    cw = encode_hamming(msg)
    print("\n--- Single-Bit Error Correction ---")
    print(f"Data: {msg}")
    print(f"Encoded Hamming Code: {cw}")

    err_in = input(f"Enter 1-indexed bit to corrupt (1-{len(cw)}) [default: 3]: ").strip()
    err_pos = int(err_in) if err_in else 3
    damaged = list(cw)
    if 1 <= err_pos <= len(cw):
        damaged[err_pos - 1] = "1" if damaged[err_pos - 1] == "0" else "0"
    damaged_str = "".join(damaged)
    fixed, detected_pos, recovered = decode_hamming(damaged_str)
    print(f"Received Codeword: {damaged_str}")
    print(f"Detected Error Position: Bit {detected_pos}")
    print(f"Corrected Codeword: {fixed}")
    print(f"Extracted Original Data: {recovered}")

    print("\n--- Multi-Bit Error Detection (SECDED with Overall Parity) ---")
    sec = add_overall_parity(cw)
    print(f"SECDED Transmitted Word (8-bit): {sec}")
    sec_1bit = sec[:2] + ("1" if sec[2] == "0" else "0") + sec[3:]
    status_1, _, _ = secded_check(sec_1bit)
    print(f"Testing 1-Bit Error ({sec_1bit}): {status_1}")
    sec_2bit = sec[:2] + ("1" if sec[2] == "0" else "0") + sec[3:5] + ("1" if sec[5] == "0" else "0") + sec[6:]
    status_2, _, _ = secded_check(sec_2bit)
    print(f"Testing 2-Bit Error ({sec_2bit}): {status_2}")
