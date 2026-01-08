from pathlib import Path

def create_rom():
    # Start with Clear Screen
    rom = bytearray([0x00, 0xE0])

    # Grid Coordinates (Safe for 64x32 display)
    # X: 4 columns spaced 16 pixels apart (plus 2px margin)
    xs = [2, 18, 34, 50]
    # Y: 4 rows spaced 7 pixels apart (Font is 5px tall)
    ys = [2, 9, 16, 23]

    char_idx = 0

    # Loop through the grid 4x4
    for y in ys:
        for x in xs:
            # 1. Set V0 = Character Index (0 through F)
            rom.append(0x60)
            rom.append(char_idx)

            # 2. Set I = Font Address for Character in V0
            # Opcode: F029
            rom.extend([0xF0, 0x29])

            # 3. Set Draw Position V1=X, V2=Y
            # Opcode: 61XX (Set V1)
            rom.append(0x61)
            rom.append(x)
            # Opcode: 62YY (Set V2)
            rom.append(0x62)
            rom.append(y)

            # 4. Draw Sprite (5 pixels tall)
            # Opcode: D125
            rom.extend([0xD1, 0x25])

            char_idx += 1

    # --- Infinite Loop at the end ---
    # We calculate the current address to jump to itself
    # ROM starts at 0x200. len(rom) is current offset.
    jump_address = 0x200 + len(rom)
    
    # Split into high/low bytes for opcode 1NNN
    high_byte = (jump_address >> 8) & 0x0F
    low_byte = jump_address & 0xFF
    
    # Opcode: 1NNN (Jump to NNN)
    rom.append(0x10 | high_byte)
    rom.append(low_byte)

    # --- File Output Logic ---
    out_dir = Path(__file__).resolve().parent / "../binaries"
    out_dir.mkdir(parents=True, exist_ok=True)
    out_path = out_dir / "font_set.ch8"
    
    with open(out_path, "wb") as f:
        f.write(rom)
    print(f"Wrote ROM to {out_path} (Size: {len(rom)} bytes)")

if __name__ == "__main__":
    create_rom()