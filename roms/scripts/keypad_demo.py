from pathlib import Path

def create_rom():
    rom = bytes([
        # --- INITIAL SETUP ---
        # 1. Clear Screen IMMEDIATELY (Fixes the "Junk" issue)
        0x00, 0xE0, 

        # --- MAIN LOOP (0x202) ---
        # 2. Wait for Key Press (Blocking)
        # Store result in V0
        0xF0, 0x0A, 

        # --- DRAW ROUTINE ---
        # 3. Clear Screen (Clear the previous letter)
        0x00, 0xE0,

        # 4. Set I = Font Address for character in V0
        0xF0, 0x29,

        # 5. Set Coordinates (Center: X=28, Y=12)
        0x61, 0x1C, # V1 = 28
        0x62, 0x0C, # V2 = 12

        # 6. Draw Sprite (5 pixels tall) at V1, V2
        0xD1, 0x25,

        # --- RELEASE LOCK (0x20E) ---
        # Address calculation for the Release Loop
        
        # 7. Skip next instruction if Key in V0 is NOT pressed (Released)
        0xE0, 0xA1, 

        # 8. Jump back to Release Lock (0x20E) - Key is still down
        0x12, 0x0E, 

        # 9. Jump back to Start of Loop (0x202) - Wait for new key
        0x12, 0x02 
    ])

    out_dir = Path(__file__).resolve().parent / "../binaries"
    out_dir.mkdir(parents=True, exist_ok=True)
    out_path = out_dir / "keypad_demo.ch8"
    
    with open(out_path, "wb") as f:
        f.write(rom)
    print(f"Created keypad_demo.ch8")

if __name__ == "__main__":
    create_rom()