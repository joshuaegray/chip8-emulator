#Created by Gemini
# Fixed Inverted Stack Test (Address Verified)
# Tests 2NNN (Call) and 00EE (Return)
# 1: Pass
# 0: Fail

from pathlib import Path

def create_rom():
    rom = bytes([
        # -- SETUP (0x200) --
        0x00, 0xE0, # Clear Screen

        # -- THE TEST (0x202) --
        # Call Subroutine at 0x20C
        0x22, 0x0C, 
        
        # -- RETURN LANDING SPOT (0x204) --
        # If RET works, we land here. Jump to SUCCESS (0x214).
        0x12, 0x14, 

        # -- TRAP (0x206) --
        # If we fall through, Jump to FAIL (0x218).
        0x12, 0x18, 

        # -- PAD (0x208, 0x20A) --
        0x00, 0x00, 0x00, 0x00,

        # -- SUBROUTINE (0x20C) --
        0x00, 0xEE, # RETURN immediately
        
        # -- SUBROUTINE TRAP (0x20E) --
        # If RET fails, Jump to FAIL (0x218)
        0x12, 0x18, 

        # -- PAD (0x210, 0x212) --
        0x00, 0x00, 0x00, 0x00,

        # -- SUCCESS ROUTINE (0x214) --
        0xA0, 0x55, # I = Address of Sprite "1" (Pass)
        0x12, 0x1A, # Jump to Draw (0x21A)

        # -- FAIL ROUTINE (0x218) --
        # Note: Valid code falls through to here from 0x216, 
        # but 0x216 is a jump, so we are safe.
        0xA0, 0x50, # I = Address of Sprite "0" (Fail)
        
        # -- DRAW SHARED (0x21A) --
        0x62, 0x00, # V2 = 0
        0xD2, 0x25, # Draw sprite at (0,0) with height 5
        
        # -- INFINITE LOOP (0x21E) --
        0x12, 0x1E  # Jump to self
    ])

    out_dir = Path(__file__).resolve().parent / "../binaries"
    out_dir.mkdir(parents=True, exist_ok=True)

    out_path = out_dir / "stack_inverted.ch8"
    with open(out_path, "wb") as f:
        f.write(rom)

    print(f"Wrote ROM to {out_path}")

def main():
    create_rom()

if __name__ == "__main__":
    main()