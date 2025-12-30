# Created by Gemini
# Test AND, OR, XOR, and SHIFTS
# Pass: 0
#Fail: 1
from pathlib import Path
def create_rom():
    rom = bytes([
        0x60, 0xAA, # V0 = 10101010
        0x61, 0x55, # V1 = 01010101
        0x80, 0x11, # V0 |= V1 (V0 should be 0xFF)
        0x30, 0xFF, # Skip if V0 == 0xFF
        0x12, 0x18, # Jump to FAIL

        0x60, 0x01, # V0 = 0x01
        0x80, 0x0E, # V0 <<= 1 (V0 becomes 0x02, VF should be 0)
        0x30, 0x02, # Skip if V0 == 0x02
        0x12, 0x18, # Jump to FAIL
        0x3F, 0x00, # Skip if VF == 0
        0x12, 0x18, # Jump to FAIL

        # SUCCESS
        0x00, 0xE0, 0xA0, 0x50, 0x62, 0x00, 0xD2, 0x25, 0x12, 0x12,
        
        # FAIL (Starts at 0x218)
        0x00, 0xE0, 0xA0, 0x55, 0x62, 0x00, 0xD2, 0x25, 0x12, 0x1C 
    ])

    out_dir = Path(__file__).resolve().parent / "../binaries"
    out_dir.mkdir(parents=True, exist_ok=True)
    out_path = out_dir / "logic.ch8"
    with open(out_path, "wb") as f:
        f.write(rom)

    print(f"Wrote ROM to {out_path}")

def main():
    create_rom()

if __name__ == "__main__":
    main()