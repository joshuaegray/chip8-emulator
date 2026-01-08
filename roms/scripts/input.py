#Written by Gemini
# Press 5: Should draw 1 if pass, F if another key is pressed
#Fail: Blank screen
from pathlib import Path

def create_rom():
    # Pass = "1", Fail = "F" (Solid, no flashing)
    
    # Memory Map:
    # 0x200: Start
    # 0x210: Fail Routine
    # 0x21A: Success Routine
    
    rom = bytes([
        # --- SETUP (0x200) ---
        0x00, 0xE0, # Clear Screen

        # 1. Draw "0" (Prompt)
        0x60, 0x00, # V0 = 0
        0xF0, 0x29, # I = Font "0"
        0xD0, 0x05, # Draw "0"

        # 2. Wait for Key (Result in V1)
        0xF1, 0x0A, 

        # 3. Check Input
        0x62, 0x05, # V2 = 5 (Key 'W')
        0x91, 0x20, # Skip next if V1 != V2
        
        # 4. Jump to Success (0x21A)
        # Only reached if V1 == V2
        0x12, 0x1A, 

        # --- FAIL ROUTINE (0x210) ---
        # 1. Clear Screen
        0x00, 0xE0, 
        # 2. Draw "F"
        0x60, 0x0F, # V0 = F
        0xF0, 0x29, # I = Font "F"
        0xD0, 0x05, # Draw "F"
        # 3. Infinite Loop (Solid State)
        # Jump to 0x218 (This instruction)
        0x12, 0x18, 

        # --- SUCCESS ROUTINE (0x21A) ---
        # 1. Clear Screen
        0x00, 0xE0,
        # 2. Draw "1"
        0x60, 0x01, # V0 = 1
        0xF0, 0x29, # I = Font "1"
        0xD0, 0x05, # Draw "1"
        # 3. Infinite Loop (Solid State)
        # Jump to 0x222 (This instruction)
        0x12, 0x22 
    ])

    out_dir = Path(__file__).resolve().parent / "../binaries"
    out_dir.mkdir(parents=True, exist_ok=True)
    out_path = out_dir / "input.ch8"
    
    with open(out_path, "wb") as f:
        f.write(rom)
    print(f"Wrote ROM to {out_path}")

if __name__ == "__main__":
    create_rom()