#Created by Gemini
# Fixed Inverted Shift Test (Pass = "1", Fail = "0")
# Tests 8XY6 (Right Shift) and 8XYE (Left Shift)


from pathlib import Path
def create_rom():

    rom = bytes([
        # -- SETUP --
        # Address 0x200
        0x00, 0xE0, # Clear Screen

        # -- TEST 1: RIGHT SHIFT (0x01 >> 1) --
        # Address 0x202
        0x60, 0x01, # V0 = 1
        # Address 0x204
        0x80, 0x06, # V0 >>= 1 (V0 becomes 0, VF should be 1)
        
        # -- CHECK 1 --
        # Address 0x206
        0x3F, 0x00, # Skip next if VF == 0 (Error case)
        # Address 0x208 (Executes if VF=1 / Logic Good)
        0x12, 0x0C, # Jump to 0x20C (Start Test 2)
        # Address 0x20A (Executes if VF=0 / Logic Bad)
        0x12, 0x1A, # Jump to 0x21A (FAIL Routine)

        # -- TEST 2: LEFT SHIFT (0x80 << 1) --
        # Address 0x20C
        0x60, 0x80, # V0 = 128 (0x80)
        # Address 0x20E
        0x80, 0x0E, # V0 <<= 1 (V0 becomes 0, VF should be 1)

        # -- CHECK 2 --
        # Address 0x210
        0x3F, 0x00, # Skip next if VF == 0 (Error case)
        # Address 0x212 (Executes if VF=1 / Logic Good)
        0x12, 0x16, # Jump to 0x216 (SUCCESS Routine)
        # Address 0x214 (Executes if VF=0 / Logic Bad)
        0x12, 0x1A, # Jump to 0x21A (FAIL Routine)

        # -- SUCCESS ROUTINE --
        # Address 0x216
        0xA0, 0x55, # I = Address of Sprite "1" (Pass)
        # Address 0x218
        0x12, 0x1C, # Jump to Draw

        # -- FAIL ROUTINE --
        # Address 0x21A
        0xA0, 0x50, # I = Address of Sprite "0" (Fail)

        # -- DRAW SHARED --
        # Address 0x21C
        0x62, 0x00, # V2 = 0
        # Address 0x21E
        0xD2, 0x25, # Draw sprite
        # Address 0x220
        0x12, 0x20  # Infinite Loop
    ])


    out_dir = Path(__file__).resolve().parent / "../binaries"
    out_dir.mkdir(parents=True, exist_ok=True)
    out_path = out_dir / "shift_inverted.ch8"
    with open(out_path, "wb") as f:
        f.write(rom)

    print(f"Wrote ROM to {out_path}")

def main():
    create_rom()

if __name__ == "__main__":
    main()