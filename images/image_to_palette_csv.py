import sys
import os
import re
from PIL import Image


def load_palette(path):
    colors = []
    with open(path, "r") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            m = re.search(r"#([0-9A-Fa-f]{6})", line)
            if m:
                h = m.group(1)
                r, g, b = int(h[0:2], 16), int(h[2:4], 16), int(h[4:6], 16)
                if (r, g, b) not in colors:
                    colors.append((r, g, b))
    return colors


def closest_color_index(pixel, palette):
    r, g, b = pixel[0], pixel[1], pixel[2]
    best_idx = 0
    best_dist = float("inf")
    for i, (pr, pg, pb) in enumerate(palette):
        dist = (r - pr) ** 2 + (g - pg) ** 2 + (b - pb) ** 2
        if dist < best_dist:
            best_dist = dist
            best_idx = i
    return best_idx


def main():
    if len(sys.argv) != 4:
        print(f"Usage: python {sys.argv[0]} <image> <palette> <output.h>")
        sys.exit(1)

    image_path = sys.argv[1]
    palette_path = sys.argv[2]
    output_path = sys.argv[3]

    print(f"image_path:{sys.argv[1]}, palette_path:{sys.argv[2]}, outputpath: {sys.argv[3]}")

    key = os.path.splitext(os.path.basename(image_path))[0].upper()

    palette = load_palette(palette_path)
    if not palette:
        print("Error: no colors found in palette file.")
        sys.exit(1)

    img = Image.open(image_path).convert("RGB")
    width, height = img.size
    pixels = img.load()

    rows = []
    for y in range(height):
        row = []
        for x in range(width):
            idx = closest_color_index(pixels[x, y], palette)
            row.append(str(idx))
        rows.append(",".join(row))

    with open(output_path, "a") as f:
        f.write(f"constexpr unsigned int {key}_WIDTH={width};\n")
        f.write(f"constexpr unsigned int {key}_HEIGHT={height};\n")
        f.write(f"static const uint8_t sprite_{key}[{key}_WIDTH * {key}_HEIGHT]={{\n")
        for row in rows:
            f.write(row + ",\n")
        f.write("}};\n\n")

    print(f"Appended sprite_{key} to {output_path} ({width}x{height}, {len(palette)} palette colors)")


if __name__ == "__main__":
    main()
