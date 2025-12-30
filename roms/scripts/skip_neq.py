# Tests 4XNN, and VF Register
#Screen should output 0 if it passes and 1 if it fails

from pathlib import Path

def create_rom():
    rom = bytes([
        0x00, 0xE0, #Clear Screen
        0x60, 0x00, #V0 = 0
        0x61, 0x00, #V1 = 0
        0xA0, 0x50, #I = 0x050
        0xD0, 0X15, #Draw "0" To Screen
        0x4F, 0x00, #If VF != 0, skip
        0x12, 0x10, #Jump to 0x210 (OK so far)
        0x12, 0x20, #Jump to 0x222 (FAIL)

        #OK1 @ 0x210
        0xA0, 0x50, #I = 0x050
        0xD0, 0x15, #Draw 0
        0x4F, 0x01, #Skip if VF != 1
        0x12, 0x1A, #Jump to OK
        0x12, 0x22, #Jump to fail 

        #OK @ 0x21A 
        0x00, 0xE0, #Clear Screen 
        0xA0, 0x50, #I = 0x050
        0xD0, 0x15, #Draw 0
        0x12, 0x1A, #Jump back to start of OK

        #FAIL @0x222 
        0x00, 0xE0, #Clear Screen 
        0xA0, 0x55, #I = 0x055 
        0xD0, 0x15, #Draw 1
        0x12, 0x22 #Jump to Fail



    ])

    out_dir = Path(__file__).resolve().parent / "../binaries"
    out_dir.mkdir(parents=True, exist_ok=True)

    out_path = out_dir / "skip_neq.ch8"
    with open(out_path, "wb") as f:
        f.write(rom)

    print(f"Wrote ROM to {out_path}")

def main():
    create_rom()

if __name__ == "__main__":
    main()
