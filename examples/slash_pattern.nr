print("=== Slash Pattern Generator ===")

seed = 37

rows = 25
width = 60

row = 0

while row < rows:
    col = 0
    line = ""

    while col < width:
        seed = (seed * 17 + 43) % 97

        if seed < 48:
            line = line + "/"
        else:
            line = line + "\\"

        col = col + 1

    print(line)
    row = row + 1

print("=== Done ===")