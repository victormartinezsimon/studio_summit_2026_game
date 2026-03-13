import sys, subprocess, re

pixel_w = int(sys.argv[1])
pixel_h = int(sys.argv[2])
img_name = sys.argv[3]
csv_name = sys.argv[4]

result = subprocess.run(['magick', 'palette.png', 'txt:-'], capture_output=True, text=True)
palette_colors = []
for line in result.stdout.splitlines():
    m = re.search(r'#([0-9A-Fa-f]{6})', line)
    if m and m.group(1).upper() not in palette_colors:
        palette_colors.append(m.group(1).upper())

result2 = subprocess.run(['magick', img_name, 'txt:-'], capture_output=True, text=True)
rows = [[] for _ in range(pixel_h)]

def hex_to_rgb(h):
    return int(h[0:2],16), int(h[2:4],16), int(h[4:6],16)

for line in result2.stdout.splitlines():
    coord = re.match(r'(\d+),(\d+):', line)
    color = re.search(r'#([0-9A-Fa-f]{6})', line)
    if coord and color:
        x = int(coord.group(1))
        y = int(coord.group(2))
        hex_color = color.group(1).upper()
        if hex_color in palette_colors:
            idx = palette_colors.index(hex_color)
        else:
            pr, pg, pb = hex_to_rgb(hex_color)
            best, best_d = 0, float('inf')
            for i, pc in enumerate(palette_colors):
                cr, cg, cb = hex_to_rgb(pc)
                d = (pr-cr)**2 + (pg-cg)**2 + (pb-cb)**2
                if d < best_d:
                    best_d, best = d, i
            idx = best
        if 0 <= y < pixel_h:
            rows[y].append(str(idx))
with open(csv_name, 'w') as f:
    for row in rows:
        f.write(','.join(row))
        f.write(',\n')
print('CSV guardado: ' + csv_name)