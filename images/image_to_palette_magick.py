import sys
import os
import re
import subprocess


def load_palette(path):
    result = subprocess.run(["magick", path, "txt:-"], capture_output=True, text=True)
    colors = []
    for line in result.stdout.splitlines():
        m = re.search(r"#([0-9A-Fa-f]{6})", line)
        if m:
            h = m.group(1).upper()
            r, g, b = int(h[0:2], 16), int(h[2:4], 16), int(h[4:6], 16)
            if (r, g, b) not in colors:
                colors.append((r, g, b))
    return colors


def closest_color_index(pr, pg, pb, palette):
    best_idx = 0
    best_dist = float("inf")
    for i, (cr, cg, cb) in enumerate(palette):
        dist = (pr - cr) ** 2 + (pg - cg) ** 2 + (pb - cb) ** 2
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
    key = os.path.splitext(os.path.basename(image_path))[0].upper()

    palette = load_palette(palette_path)
    if not palette:
        print("Error: no colors found in palette file.")
        sys.exit(1)

    result = subprocess.run(["magick", image_path, "txt:-"], capture_output=True, text=True)

    # Parse image dimensions and pixels
    pixels = {}
    max_x = 0
    max_y = 0
    for line in result.stdout.splitlines():
        coord = re.match(r"(\d+),(\d+):", line)
        color = re.search(r"#([0-9A-Fa-f]{6})", line)
        if coord and color:
            x = int(coord.group(1))
            y = int(coord.group(2))
            h = color.group(1).upper()
            r, g, b = int(h[0:2], 16), int(h[2:4], 16), int(h[4:6], 16)
            pixels[(x, y)] = (r, g, b)
            if x > max_x:
                max_x = x
            if y > max_y:
                max_y = y

    width = max_x + 1
    height = max_y + 1

    rows = []
    for y in range(height):
        row = []
        for x in range(width):
            pr, pg, pb = pixels.get((x, y), (0, 0, 0))
            idx = closest_color_index(pr, pg, pb, palette)
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
