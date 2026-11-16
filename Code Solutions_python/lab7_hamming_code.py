# Sample inputs:
# Binary data word: 1011
# Error bit position to corrupt (1 to codeword length): 3

def calculate_redundant_bits(m):
    # Formula: 2^r >= m + r + 1
    for r in range(1, m + 10):
        if 2**r >= m + r + 1:
            return r
    return 0

def encode_hamming(data):
    m = len(data)
    r = calculate_redundant_bits(m)
    total_len = m + r
    
    # 1-indexed codeword initialized with '0'
    code = ['0'] * (total_len + 1)
    
    # Place data bits in non-power-of-two positions
    j = 0
    for pos in range(1, total_len + 1):
        if (pos & (pos - 1)) != 0:  # Not a power of 2
            code[pos] = data[j]
            j += 1
            
    # Calculate parity bits at powers of 2 (1, 2, 4, 8, ...)
    for i in range(r):
        p_pos = 2**i
        parity = 0
        for pos in range(1, total_len + 1):
            if pos & p_pos:
                parity ^= int(code[pos])
        code[p_pos] = str(parity)
        
    return "".join(code[1:]), r

def decode_hamming(codeword, r):
    n = len(codeword)
    code = ['0'] + list(codeword)
    syndrome = 0
    
    # Check parity bits to calculate error syndrome
    for i in range(r):
        p_pos = 2**i
        if p_pos > n:
            break
        parity = 0
        for pos in range(1, n + 1):
            if pos & p_pos:
                parity ^= int(code[pos])
        if parity != 0:
            syndrome += p_pos
            
    # Single-bit error correction
    if syndrome != 0 and syndrome <= n:
        print(f"  [Correction] Error detected at Bit Position {syndrome}! Inverting bit...")
        code[syndrome] = '1' if code[syndrome] == '0' else '0'
    elif syndrome == 0:
        print("  [Verification] Syndrome is 0 -> No bit errors present.")
    else:
        print(f"  [Warning] Syndrome {syndrome} out of bounds.")
        
    corrected = "".join(code[1:])
    # Extract original data bits (excluding power of 2 positions)
    data = "".join(bit for pos, bit in enumerate(corrected, 1) if (pos & (pos - 1)) != 0)
    return corrected, syndrome, data

def add_overall_parity(codeword):
    # Overall parity bit for SECDED (Single Error Correction, Double Error Detection)
    p = codeword.count('1') % 2
    return codeword + str(p)

def secded_check(block, r):
    codeword = block[:-1]
    p_bit = block[-1]
    
    corrected, syndrome, data = decode_hamming(codeword, r)
    total_ones = block.count('1')
    parity_valid = (total_ones % 2 == 0)
    
    if syndrome == 0 and parity_valid:
        return "No Error Detected", codeword, data
    elif syndrome != 0 and not parity_valid:
        return f"Single-Bit Error (Corrected at bit {syndrome})", corrected, data
    elif syndrome != 0 and parity_valid:
        return "DOUBLE-BIT ERROR DETECTED! (Uncorrectable)", codeword, None
    else:
        return "Parity Bit Itself Corrupted", codeword, data

if __name__ == "__main__":
    msg = input("Enter binary data word [default: 1011]: ").strip() or "1011"
    cw, r = encode_hamming(msg)
    
    print("\n" + "=" * 60)
    print(" 1. HAMMING CODE (7,4) SINGLE-BIT ERROR CORRECTION")
    print("=" * 60)
    print(f"Input Data Word (m={len(msg)}): {msg}")
    print(f"Number of Parity Bits Required (r): {r}")
    print(f"Encoded Hamming Codeword: {cw}")
    
    err_in = input(f"Enter 1-indexed bit to corrupt (1-{len(cw)}) [default: 3]: ").strip()
    err_pos = int(err_in) if err_in else 3
    
    corrupted = list(cw)
    if 1 <= err_pos <= len(cw):
        corrupted[err_pos - 1] = '1' if corrupted[err_pos - 1] == '0' else '0'
    corrupted_str = "".join(corrupted)
    
    print(f"\nTransmitted: {cw}")
    print(f"Received (Corrupted at Bit {err_pos}): {corrupted_str}")
    fixed, syndrome, recovered_data = decode_hamming(corrupted_str, r)
    print(f"Calculated Syndrome: {syndrome} (Points to erroneous bit)")
    print(f"Corrected Codeword:  {fixed}")
    print(f"Recovered Data Word: {recovered_data}")
    
    print("\n" + "=" * 60)
    print(" 2. SECDED (MULTI-BIT ERROR DETECTION)")
    print("=" * 60)
    secded_word = add_overall_parity(cw)
    print(f"Transmitted 8-bit SECDED Word (Hamming + Overall Parity): {secded_word}")
    
    # Simulate single bit flip
    sec_1err = secded_word[:2] + ('1' if secded_word[2] == '0' else '0') + secded_word[3:]
    status_1, _, _ = secded_check(sec_1err, r)
    print(f"\n[Test 1-Bit Error ({sec_1err})]: {status_1}")
    
    # Simulate double bit flip
    sec_2err = secded_word[:2] + ('1' if secded_word[2] == '0' else '0') + secded_word[3:5] + ('1' if secded_word[5] == '0' else '0') + secded_word[6:]
    status_2, _, _ = secded_check(sec_2err, r)
    print(f"[Test 2-Bit Error ({sec_2err})]: {status_2}")

"""
================================================================================
LAB EXPLANATION:
Lab 7 implements Hamming Code for forward error correction (FEC), specifically
single-bit error correction and multi-bit (double-bit) error detection (SECDED).

1. Hamming (7,4) Code Principle:
   - Encodes a 4-bit data word into a 7-bit codeword by adding 3 parity bits.
   - Parity bit count formula: 2^r >= m + r + 1 (For m=4, r=3, total n=7).
   - Bit positioning: Parity bits occupy power-of-two bit positions (P1 at 1, P2 at 2, P4 at 4).
     Data bits occupy remaining positions (D1 at 3, D2 at 5, D3 at 6, D4 at 7).
   - Parity assignments:
     P1 checks bits whose binary position has bit 0 set: positions 1, 3, 5, 7.
     P2 checks bits whose binary position has bit 1 set: positions 2, 3, 6, 7.
     P4 checks bits whose binary position has bit 2 set: positions 4, 5, 6, 7.

2. Syndrome Decoding:
   - The receiver recomputes the parity checks (S1, S2, S4).
   - The integer value Syndrome = S4*4 + S2*2 + S1*1 indicates the exact 1-indexed bit position of the error!
   - Inverting that specific bit repairs the codeword back to perfection.

3. SECDED (Single Error Correction, Double Error Detection):
   - Adds an 8th overall parity bit covering the whole 7-bit codeword.
   - If Syndrome != 0 and overall parity is invalid -> Exactly 1 bit error (Correctable).
   - If Syndrome != 0 and overall parity is valid -> Exactly 2 bit errors (Detectable, but Uncorrectable).

CODE EXPLANATION:
- 'calculate_redundant_bits(m)': Solves 2^r >= m + r + 1 for arbitrary data length.
- 'encode_hamming(data)': Places data bits, computes parity bits, returns encoded codeword.
- 'decode_hamming(codeword, r)': Computes syndrome integer and inverts the erroneous bit.
- 'secded_check(block, r)': Uses both syndrome and overall parity check to differentiate
  between 1-bit correctable errors and 2-bit detectable errors.
================================================================================
"""
