import sys
import os
import re
import subprocess
import struct
import zlib


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


def closest_color(pr, pg, pb, palette):
    best = palette[0]
    best_dist = float("inf")
    for cr, cg, cb in palette:
        dist = (pr - cr) ** 2 + (pg - cg) ** 2 + (pb - cb) ** 2
        if dist < best_dist:
            best_dist = dist
            best = (cr, cg, cb)
    return best


def main():
    if len(sys.argv) != 4:
        print(f"Usage: python {sys.argv[0]} <image> <palette> <output.png>")
        sys.exit(1)

    image_path = sys.argv[1]
    palette_path = sys.argv[2]
    output_path = sys.argv[3]

    print(f"image_path:{image_path}, palette_path:{palette_path}, output_path:{output_path}")

    palette = load_palette(palette_path)
    if not palette:
        print("Error: no colors found in palette file.")
        sys.exit(1)

    result = subprocess.run(["magick", image_path, "txt:-"], capture_output=True, text=True)

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

    # Build raw pixel rows with filter byte (0 = None) prepended
    raw = b""
    for y in range(height):
        raw += b"\x00"  # filter type None
        for x in range(width):
            pr, pg, pb = pixels.get((x, y), (0, 0, 0))
            cr, cg, cb = closest_color(pr, pg, pb, palette)
            raw += struct.pack("BBB", cr, cg, cb)

    # Write PNG manually
    def png_chunk(chunk_type, data):
        chunk = chunk_type + data
        return struct.pack(">I", len(data)) + chunk + struct.pack(">I", zlib.crc32(chunk) & 0xFFFFFFFF)

    with open(output_path, "wb") as f:
        # PNG signature
        f.write(b"\x89PNG\r\n\x1a\n")
        # IHDR: width, height, bit depth 8, color type 2 (RGB)
        ihdr_data = struct.pack(">IIBBBBB", width, height, 8, 2, 0, 0, 0)
        f.write(png_chunk(b"IHDR", ihdr_data))
        # IDAT: compressed pixel data
        compressed = zlib.compress(raw)
        f.write(png_chunk(b"IDAT", compressed))
        # IEND
        f.write(png_chunk(b"IEND", b""))

    print(f"Saved {output_path} ({width}x{height}, {len(palette)} palette colors)")


if __name__ == "__main__":
    main()
