# Created by Gemini
# Draws random symbols to screen
from pathlib import Path

def create_rom():
    rom = bytearray()
    
    # --- SETUP ---
    rom.extend([0x00, 0xE0]) # Clear Screen
    rom.extend([0x64, 0x00]) # V4 = 0 (Counter)

    # --- MAIN LOOP (0x204) ---
    loop_start = 0x200 + len(rom)

    # 1. Generate Random Character (0-F) in V0
    rom.extend([0xC0, 0x0F])

    # 2. Set I to Font Address for V0
    rom.extend([0xF0, 0x29])

    # 3. Generate Random X (V1) and Y (V2)
    rom.extend([0xC1, 0x3F]) # X (0-63)
    rom.extend([0xC2, 0x1F]) # Y (0-31)

    # 4. Draw it
    rom.extend([0xD1, 0x25])

    # --- DELAY LOGIC (Slow it down) ---
    # Set Delay Timer to 1 (Wait ~16ms)
    rom.extend([0x65, 0x01]) # V5 = 1
    rom.extend([0xF5, 0x15]) # DT = V5
    
    # Wait Loop for DT to hit 0
    wait_addr = 0x200 + len(rom)
    rom.extend([0xF6, 0x07]) # V6 = DT
    rom.extend([0x36, 0x00]) # Skip next if V6 == 0
    # Jump back to wait
    high = (wait_addr >> 8) & 0x0F
    low = wait_addr & 0xFF
    rom.extend([0x10 | high, low])

    # --- COUNTER & CLEAR LOGIC ---
    # 5. Increment Counter V4
    rom.extend([0x74, 0x01]) # V4 += 1

    # 6. Check if V4 == 32 (0x20)
    # If V4 != 32, Skip the Clear
    rom.extend([0x34, 0x20]) # Skip next if V4 == 32
    
    # Jump over the clear instructions (Forward 4 bytes)
    # Address of Jump = Current + 4 (Jump instruction size 2 + Clear instruction size 2)
    jump_over_addr = 0x200 + len(rom) + 6
    high = (jump_over_addr >> 8) & 0x0F
    low = jump_over_addr & 0xFF
    rom.extend([0x10 | high, low])

    # 7. Clear Screen (Only hits this if V4 was 32)
    rom.extend([0x00, 0xE0])
    # Reset Counter V4 to 0
    rom.extend([0x64, 0x00])

    # 8. Jump back to MAIN LOOP
    high = (loop_start >> 8) & 0x0F
    low = loop_start & 0xFF
    rom.extend([0x10 | high, low])

    out_dir = Path(__file__).resolve().parent / "../binaries"
    out_dir.mkdir(parents=True, exist_ok=True)
    out_path = out_dir / "art_demo.ch8"
    
    with open(out_path, "wb") as f:
        f.write(rom)
    print(f"Created art_demo.ch8")

if __name__ == "__main__":
    create_rom()