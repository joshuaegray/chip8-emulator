#Created by Gemini
# Fixed Inverted Logic Test (Pass = "1", Fail = "0")
# Tests 8XY1 (OR) and 8XY6 (Shift Right + VF)

from pathlib import Path

def create_rom():
    rom = bytes([
        # -- SETUP (0x200) --
        0x00, 0xE0, # Clear Screen

        # -- TEST 1: BITWISE OR (0x202) --
        # 0xAA | 0x55 should equal 0xFF
        0x60, 0xAA, # V0 = 0xAA
        0x61, 0x55, # V1 = 0x55
        0x80, 0x11, # V0 |= V1
        
        # Check: If V0 == 0xFF, Skip the "Jump to Fail"
        0x30, 0xFF, 
        
        # If logic failed (V0 != FF), we hit this:
        0x12, 0x20, # Jump to FAIL (0x220)

        # -- TEST 2: SHIFT RIGHT (0x20C) --
        # Shift 0x01 right. VF should be 1.
        0x60, 0x01, # V0 = 1
        0x80, 0x06, # V0 >>= 1
        
        # Check: If VF == 1, Skip the "Jump to Fail"
        0x3F, 0x01, 
        
        # If logic failed (VF != 1), we hit this:
        0x12, 0x20, # Jump to FAIL (0x220)

        # -- JUMP TO SUCCESS (0x214) --
        # If we survived both checks, go to success.
        0x12, 0x18, 

        # -- PAD (0x216) --
        0x00, 0x00,

        # -- SUCCESS ROUTINE (0x218) --
        0xA0, 0x55, # I = Address of Sprite "1" (Pass)
        0x12, 0x24, # Jump to Draw (0x224)

        # -- PAD (0x21C, 0x21E) --
        0x00, 0x00, 0x00, 0x00, 

        # -- FAIL ROUTINE (0x220) --
        0xA0, 0x50, # I = Address of Sprite "0" (Fail)
        
        # -- PAD (0x222) --
        0x00, 0x00,

        # -- DRAW SHARED (0x224) --
        0x62, 0x00, # V2 = 0
        0xD2, 0x25, # Draw sprite at (0,0) with height 5
        
        # -- INFINITE LOOP (0x228) --
        0x12, 0x28  # Jump to self
    ])

    out_dir = Path(__file__).resolve().parent / "../binaries"
    out_dir.mkdir(parents=True, exist_ok=True)

    out_path = out_dir / "logic_inverted.ch8"
    with open(out_path, "wb") as f:
        f.write(rom)

    print(f"Wrote ROM to {out_path}")

def main():
    create_rom()

if __name__ == "__main__":
    main()