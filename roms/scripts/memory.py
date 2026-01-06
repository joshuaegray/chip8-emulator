# Written by Gemini
# Bulletproof Memory Test
# Tests: FX55, FX65, FX1E, FX33, BNNN, FX29
# Pass = "1", Fail = "0"

from pathlib import Path

def create_rom():
    rom = bytes([
        # -- SETUP (0x200) --
        0x00, 0xE0, # Clear Screen

        # -- TEST 1: FX55/FX65 (Store/Load Registers) --
        # Address 0x202
        0x60, 0xAA, # V0 = 0xAA
        0x61, 0xBB, # V1 = 0xBB
        0xA3, 0x00, # I = 0x300 (Safe storage area)
        0xF1, 0x55, # Store V0-V1 at I (0x300)
        
        # CRITICAL FIX: Reset I to 0x300 manually.
        # This prevents bugs if your emulator increments I automatically.
        0xA3, 0x00, 
        
        0x60, 0x00, # Clear V0 (Verify we actually load it back)
        0x61, 0x00, # Clear V1
        0xF1, 0x65, # Load V0-V1 from I (0x300)
        
        0x30, 0xAA, # Skip next if V0 == 0xAA (Pass)
        0x12, 0x4C, # Jump to FAIL (0x24C)

        # -- TEST 2: FX1E (Add to Index) --
        # Address 0x214
        0xA3, 0x00, # I = 0x300
        0x60, 0x05, # V0 = 5
        0xF0, 0x1E, # I += V0 (I becomes 0x305)
        
        # Verify: Write V1(0xBB) to 0x305, then read it back.
        0x61, 0xBB, # V1 = 0xBB
        0xF1, 0x55, # Store V0-V1 at 0x305. (Mem[306] gets V1)
        
        0xA3, 0x06, # I = 0x306
        0xF0, 0x65, # Load V0 from 0x306. (V0 should become 0xBB)
        
        0x30, 0xBB, # Skip next if V0 == 0xBB (Pass)
        0x12, 0x4C, # Jump to FAIL (0x24C)

        # -- TEST 3: FX33 (Binary Coded Decimal) --
        # Address 0x224
        0x60, 0x7B, # V0 = 123 (Decimal)
        0xA3, 0x20, # I = 0x320
        0xF0, 0x33, # Store BCD of 123 at I, I+1, I+2
        # Mem[320]=1, Mem[321]=2, Mem[322]=3
        
        # Verify: Load them back
        0xF2, 0x65, # Load V0,V1,V2 from 0x320
        
        0x30, 0x01, # Skip if V0 == 1
        0x12, 0x4C, # Fail
        0x31, 0x02, # Skip if V1 == 2
        0x12, 0x4C, # Fail
        0x32, 0x03, # Skip if V2 == 3
        0x12, 0x4C, # Fail

        # -- TEST 4: BNNN (Jump with Offset) --
        # Address 0x238
        # Standard Behavior: Jump to NNN + V0
        0x60, 0x02, # V0 = 2
        0xB2, 0x42, # Jump to 0x242 + V0 (0x244)
        
        # 0x240: Trap (If BNNN failed completely)
        0x12, 0x4C, 
        
        # 0x242: Trap (If BNNN ignored V0)
        0x12, 0x4C,

        # 0x244: Success Landing
        0x12, 0x46, # Jump to SUCCESS

        # -- SUCCESS ROUTINE (0x246) --
        # Uses FX29 (Font Character)
        0x60, 0x01, # V0 = 1
        0xF0, 0x29, # Set I to address of Sprite "1"
        0x12, 0x52, # Jump to Draw

        # -- FAIL ROUTINE (0x24C) --
        0x60, 0x00, # V0 = 0
        0xF0, 0x29, # Set I to address of Sprite "0"

        # -- DRAW SHARED (0x252) --
        0x62, 0x00, # V2 = 0 (X)
        0x63, 0x00, # V3 = 0 (Y)
        0xD2, 0x35, # Draw sprite at (0,0), Height 5
        
        # -- INFINITE LOOP (0x258) --
        0x12, 0x58
    ])

    out_dir = Path(__file__).resolve().parent / "../binaries"
    out_dir.mkdir(parents=True, exist_ok=True)
    out_path = out_dir / "memory.ch8"
    with open(out_path, "wb") as f:
        f.write(rom)

    print(f"Wrote ROM to {out_path}")

def main():
    create_rom()

if __name__ == "__main__":
    main()