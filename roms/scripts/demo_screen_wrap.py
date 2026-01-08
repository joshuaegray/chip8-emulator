from pathlib import Path

def create_rom():
    rom = bytearray()
    
    # --- SETUP ---
    # V0 = X Position (Start 0)
    rom.extend([0x60, 0x00])
    # V1 = Y Position (Start 0)
    rom.extend([0x61, 0x00])
    # V2 = Velocity (1)
    rom.extend([0x62, 0x01])

    # --- MAIN LOOP (0x206) ---
    loop_start = 0x200 + len(rom)

    # 1. Clear Screen
    rom.extend([0x00, 0xE0])

    # 2. Set I = Font "F" (Just to have a visible sprite)
    rom.extend([0x63, 0x0F, 0xF3, 0x29])

    # 3. Draw Sprite at X(V0), Y(V1)
    rom.extend([0xD0, 0x15])

    # 4. Update Position: X = X + V2
    rom.extend([0x80, 0x24]) # V0 = V0 + V2
    # 5. Update Position: Y = Y + V2
    rom.extend([0x81, 0x24]) # V1 = V1 + V2
    
    # Note: CHIP-8 Registers naturally wrap 255->0, but screen is 64x32.
    # The Draw Opcode (DXYN) handles the wrapping on screen, but we want 
    # to keep our coordinate variables somewhat sane. 
    # Actually, for a simple demo, letting DXYN handle the wrap is the ultimate test 
    # of your Draw function's modulo arithmetic!

    # 6. Delay (So it doesn't fly too fast)
    # Set V4 = 2 (Small delay)
    rom.extend([0x64, 0x02])
    # Set Delay Timer = V4
    rom.extend([0xF4, 0x15])
    
    # 7. Wait for Timer (Tiny Spinlock)
    # Label: WAIT (Current Address)
    wait_start = 0x200 + len(rom)
    # Read Timer into V5
    rom.extend([0xF5, 0x07])
    # Skip if V5 == 0
    rom.extend([0x35, 0x00])
    # Jump back to WAIT
    high = (wait_start >> 8) & 0x0F
    low = wait_start & 0xFF
    rom.extend([0x10 | high, low])

    # 8. Jump to MAIN LOOP
    high = (loop_start >> 8) & 0x0F
    low = loop_start & 0xFF
    rom.extend([0x10 | high, low])

    out_dir = Path(__file__).resolve().parent / "../binaries"
    out_dir.mkdir(parents=True, exist_ok=True)
    out_path = out_dir / "demo_screen_wrap.ch8"
    
    with open(out_path, "wb") as f:
        f.write(rom)
    print(f"Created demo_screen_wrap.ch8")

if __name__ == "__main__":
    create_rom()