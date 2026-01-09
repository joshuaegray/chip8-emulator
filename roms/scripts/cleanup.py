#Written by Gemini
# Fixed Cleanup Test (Addresses Verified)
# Tests: 7XNN, 8XY7, 8XY2, 8XY3, 5XY0, 9XY0

from pathlib import Path

def create_rom():
    # Fail Address = 0x238
    # Success Address = 0x234
    # Draw Address = 0x23A

    rom = bytes([
        # -- SETUP (0x200) --
        0x00, 0xE0, # Clear Screen

        # -- TEST 1: 7XNN (Add Immediate) --
        # 0x202
        0x60, 0x05, # V0 = 5
        0x70, 0x05, # V0 += 5 (Should be 10 / 0x0A)
        0x30, 0x0A, # Skip next if V0 == 10 (Pass)
        0x12, 0x38, # Jump to FAIL (0x238)

        # -- TEST 2: 8XY7 (Reverse Sub: VY - VX) --
        # V0 is 10. Let's make V1 = 20.
        # V0 = V1 - V0  ->  10 = 20 - 10.
        # 0x20A
        0x61, 0x14, # V1 = 20 (0x14)
        0x80, 0x17, # V0 = V1 - V0. 
        # VF Logic for 8XY7: If V1 >= V0 (No Borrow), VF = 1.
        
        0x3F, 0x01, # Skip if VF == 1 (Pass)
        0x12, 0x38, # Jump to FAIL (0x238)
        
        0x30, 0x0A, # Skip if V0 == 10 (Pass)
        0x12, 0x38, # Jump to FAIL (0x238)

        # -- TEST 3: 8XY2 (AND) --
        # 0x0F (00001111) AND 0x0A (00001010) = 0x0A
        # 0x216
        0x60, 0x0F, # V0 = 0x0F
        0x61, 0x0A, # V1 = 0x0A
        0x80, 0x12, # V0 &= V1
        0x30, 0x0A, # Skip if V0 == 0x0A (Pass)
        0x12, 0x38, # Jump to FAIL (0x238)

        # -- TEST 4: 8XY3 (XOR) --
        # 0x0A (1010) XOR 0x0A (1010) = 0x00
        # V0 is already 0x0A from prev test
        # 0x220
        0x80, 0x13, # V0 ^= V1
        0x30, 0x00, # Skip if V0 == 0 (Pass)
        0x12, 0x38, # Jump to FAIL (0x238)

        # -- TEST 5: 5XY0 (Skip Reg Equal) --
        # 0x226
        0x60, 0x55, # V0 = 0x55
        0x61, 0x55, # V1 = 0x55
        0x50, 0x10, # Skip next if V0 == V1 (Should Skip)
        0x12, 0x38, # Jump to FAIL (0x238)

        # -- TEST 6: 9XY0 (Skip Reg Not Equal) --
        # 0x22E
        0x62, 0x99, # V2 = 0x99
        0x90, 0x20, # Skip next if V0 != V2 (Should Skip)
        0x12, 0x38, # Jump to FAIL (0x238)

        # -- SUCCESS ROUTINE (0x234) --
        0xA0, 0x55, # I = Sprite "1"
        0x12, 0x3C, # Jump to Draw (0x23C)

        # -- FAIL ROUTINE (0x238) --
        0xA0, 0x50, # I = Sprite "0"

        # -- DRAW SHARED (0x23A) --
        0x62, 0x00, # V2 = 0
        0xD2, 0x25, # Draw
        
        # -- LOOP (0x23E) --
        0x12, 0x3E 
    ])

    out_dir = Path(__file__).resolve().parent / "../binaries"
    out_dir.mkdir(parents=True, exist_ok=True)
    out_path = out_dir / "cleanup.ch8"
    with open(out_path, "wb") as f:
        f.write(rom)
    print(f"Wrote ROM to {out_path}")

def main():
    create_rom()

if __name__ == "__main__":
    main()