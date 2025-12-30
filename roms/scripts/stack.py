# Created by Gemini
#Test Call and Return
#Expected outcome: Two 0s appear on screen
from pathlib import Path
def create_rom():
   # Test Call and Return
    rom = bytes([
        0x00, 0xE0, # Clear
        0x60, 0x00, # V0 = X position 0
        0x22, 0x0C, # CALL subroutine at 0x20C
        0x60, 0x08, # V0 = X position 8
        0x22, 0x0C, # CALL subroutine at 0x20C
        0x12, 0x0A, # Halt in place (Success)

        # Subroutine at 0x20C
        0xA0, 0x50, # I = 0x050
        0x61, 0x00, # V1 = Y position 0
        0xD0, 0x15, # Draw '0' at (V0, V1)
        0x00, 0xEE  # RETURN
    ])

    out_dir = Path(__file__).resolve().parent / "../binaries"
    out_dir.mkdir(parents=True, exist_ok=True)
    out_path = out_dir / "stack.ch8"
    with open(out_path, "wb") as f:
        f.write(rom)

    print(f"Wrote ROM to {out_path}")

def main():
    create_rom()

if __name__ == "__main__":
    main()