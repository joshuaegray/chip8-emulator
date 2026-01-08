#Written by Gemini
#Test random function
from pathlib import Path

def create_rom():
    # Memory Map:
    # 0x200: Setup
    # 0x204: Draw Loop
    # 0x214: Infinite Lock
    
    rom = bytes([
        # --- SETUP ---
        0x00, 0xE0, # Clear Screen
        0x60, 0x32, # V0 = 50 (Loop Counter)

        # --- DRAW LOOP (0x204) ---
        # 1. Generate Random X (0-63)
        0xC1, 0x3F, # V1 = Rand & 0x3F
        # 2. Generate Random Y (0-31)
        0xC2, 0x1F, # V2 = Rand & 0x1F
        
        # 3. Set I = Font "F"
        0x63, 0x0F, # V3 = 0xF
        0xF3, 0x29, # Set I to Font Char in V3

        # 4. Draw at (V1, V2)
        0xD1, 0x25, 

        # 5. Decrement Counter (V0)
        0x70, 0xFF, # V0 += 0xFF (Start of subtraction hack)
        
        # 6. Check Loop
        0x30, 0x00, # Skip Jump if V0 == 0
        0x12, 0x04, # Jump back to 0x204

        # --- INFINITE LOCK (0x214) ---
        # Stop here so the screen doesn't flicker or clear
        0x12, 0x14 
    ])

    out_dir = Path(__file__).resolve().parent / "../binaries"
    out_dir.mkdir(parents=True, exist_ok=True)
    out_path = out_dir / "random.ch8"
    
    with open(out_path, "wb") as f:
        f.write(rom)
    print(f"Wrote ROM to {out_path}")

if __name__ == "__main__":
    create_rom()