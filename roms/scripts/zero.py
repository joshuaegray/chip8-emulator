#Simple test case to draw a zero to the screen

from pathlib import Path

def create_rom():
    rom = bytes([
        0x00, 0xE0, #clear screen
        0x60, 0x00,  #V0 = 0
        0x61, 0x00,  #V1 = 0
        0xA2, 0x0C, #I = 20C
        0xD0, 0x15, #Draw
        0x12, 0x00, #Jump back to 0x200
        0xF0, 0x90, 0x90, 0x90, 0xF0 #Sprite that looks like a zero
    ])

    out_dir = Path(__file__).resolve().parent / "../binaries"
    out_dir.mkdir(parents=True, exist_ok=True)

    out_path = out_dir / "zero.ch8"
    with open(out_path, "wb") as f:
        f.write(rom)

    print(f"Wrote ROM to {out_path}")

def main():
    create_rom()

if __name__ == "__main__":
    main()
