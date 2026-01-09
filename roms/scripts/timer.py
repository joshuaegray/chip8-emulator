#Written by Gemini
#Tests Timers
#Should be a beep, then a 1 is drawn
from pathlib import Path

def create_rom():
    # Memory Map:
    # 0x200: Start / Clear Screen
    # 0x206: Wait Loop
    # 0x20C: Draw Routine
    # 0x218: Infinite Lock (Solid State)

    rom = bytes([
        # --- SETUP ---
        0x00, 0xE0, # [200] Clear Screen (Start clean)

        # 1. Set V0 = 60 (approx 1 second delay)
        0x60, 0x3C, # [202] V0 = 60
        # 2. Set Delay Timer = V0
        0xF0, 0x15, # [204] DT = V0

        # --- WAIT LOOP (0x206) ---
        # 3. Read Delay Timer into V1
        0xF1, 0x07, # [206] V1 = DT
        # 4. Skip next if V1 == 0 (Timer Finished)
        0x31, 0x00, # [208] Skip if V1 == 0
        # 5. Jump back to Wait Loop
        0x12, 0x06, # [20A] Jump to 0x206

        # --- SUCCESS (0x20C) ---
        # 6. Clear Screen (Ensure contrast)
        0x00, 0xE0, # [20C] CLS
        
        # 7. Setup Draw "1"
        0x60, 0x01, # [20E] V0 = 1
        0xF0, 0x29, # [210] I = Font "1"
        0x61, 0x00, # [212] V1 = 0 (X)
        0x62, 0x00, # [214] V2 = 0 (Y)

        # 8. Draw ONE TIME
        0xD1, 0x25, # [216] Draw at 0,0

        # --- INFINITE LOCK (0x218) ---
        # 9. Jump to THIS address (0x218)
        # Prevents flickering by stopping the CPU here.
        0x12, 0x18  # [218] Jump to 0x218
    ])

    out_dir = Path(__file__).resolve().parent / "../binaries"
    out_dir.mkdir(parents=True, exist_ok=True)
    out_path = out_dir / "timer.ch8"
    
    with open(out_path, "wb") as f:
        f.write(rom)
    print(f"Wrote ROM to {out_path}")

if __name__ == "__main__":
    create_rom()