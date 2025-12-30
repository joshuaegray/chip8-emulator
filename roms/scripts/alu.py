#Created by Gemini
# Test 8XY4 (Add) and 8XY5 (Sub)
# 0: Pass
# 1: Fail

from pathlib import Path
def create_rom():
    rom = bytes([
        0x60, 0xFF, # V0 = 0xFF
        0x61, 0x01, # V1 = 0x01
        0x80, 0x14, # V0 += V1 (V0 becomes 0, VF should be 1)
        0x3F, 0x01, # If VF == 1, skip next instruction
        0x12, 0x12, # Jump to FAIL (Address 0x212)
        
        0x60, 0x05, # V0 = 5
        0x61, 0x02, # V1 = 2
        0x80, 0x15, # V0 -= V1 (V0 becomes 3, VF should be 1 because no borrow)
        0x3F, 0x01, # If VF == 1, skip
        0x12, 0x12, # Jump to FAIL (Address 0x212)

        # SUCCESS: Draw "0"
        0x00, 0xE0, # Clear
        0xA0, 0x50, # I = 0x050 (Address of '0' sprite)
        0x62, 0x00, # V2 = 0
        0xD2, 0x25, # Draw '0' at (0,0)
        0x12, 0x10, # Halt (Infinite loop at success)

        # FAIL: Draw "1"
        0x00, 0xE0, # Clear
        0xA0, 0x55, # I = 0x055 (Address of '1' sprite)
        0x62, 0x00, # V2 = 0
        0xD2, 0x25, # Draw '1' at (0,0)
        0x12, 0x12  # Halt (Infinite loop at fail)
    ])


    out_dir = Path(__file__).resolve().parent / "../binaries"
    out_dir.mkdir(parents=True, exist_ok=True)
    out_path = out_dir / "alu.ch8"
    with open(out_path, "wb") as f:
        f.write(rom)

    print(f"Wrote ROM to {out_path}")

def main():
    create_rom()

if __name__ == "__main__":
    main()