#Created by Gemini
# Inverted alu: PASS = "1", FAIL = "0"

from pathlib import Path
def create_rom():
    rom = bytes([
        # -- SETUP --
        # Address 0x200
        0x00, 0xE0, # Clear Screen

        # -- TEST 1: ADDITION (0xFF + 0x01) --
        # Address 0x202
        0x60, 0xFF, # V0 = 255
        # Address 0x204
        0x61, 0x01, # V1 = 1
        # Address 0x206
        0x80, 0x14, # V0 += V1 (V0 becomes 0, VF should be 1)

        # -- CHECK 1 --
        # Address 0x208
        0x3F, 0x00, # Skip next instruction if VF == 0 (Error case)
        
        # Address 0x20A (Executes if VF=1 / Logic Good)
        0x12, 0x0E, # Jump to 0x20E (Start Test 2)
        
        # Address 0x20C (Executes if VF=0 / Logic Bad / Skipped to here)
        0x12, 0x1E, # Jump to 0x21E (FAIL Routine)

        # -- TEST 2: SUBTRACTION (0x05 - 0x02) --
        # Address 0x20E
        0x60, 0x05, # V0 = 5
        # Address 0x210
        0x61, 0x02, # V1 = 2
        # Address 0x212
        0x80, 0x15, # V0 -= V1 (V0 becomes 3, VF should be 1)

        # -- CHECK 2 --
        # Address 0x214
        0x3F, 0x00, # Skip next if VF == 0 (Error case)
        
        # Address 0x216 (Executes if VF=1 / Logic Good)
        0x12, 0x1A, # Jump to 0x21A (SUCCESS Routine)

        # Address 0x218 (Executes if VF=0 / Logic Bad)
        0x12, 0x1E, # Jump to 0x21E (FAIL Routine)

        # -- SUCCESS ROUTINE --
        # Address 0x21A
        0xA0, 0x55, # I = Address of Sprite "1" (Pass)
        # Address 0x21C
        0x12, 0x20, # Jump to Draw

        # -- FAIL ROUTINE --
        # Address 0x21E
        0xA0, 0x50, # I = Address of Sprite "0" (Fail)

        # -- DRAW SHARED --
        # Address 0x220
        0x62, 0x00, # V2 = 0 (X, Y coords)
        # Address 0x222
        0xD2, 0x25, # Draw sprite at (0,0)
        # Address 0x224
        0x12, 0x24  # Infinite Loop
    ])

    out_dir = Path(__file__).resolve().parent / "../binaries"
    out_dir.mkdir(parents=True, exist_ok=True)
    out_path = out_dir / "alu_inverted.ch8"
    with open(out_path, "wb") as f:
        f.write(rom)

    print(f"Wrote ROM to {out_path}")

def main():
    create_rom()

if __name__ == "__main__":
    main()