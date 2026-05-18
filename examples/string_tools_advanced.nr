print("=== Advanced string tools ===")

text = "Nearoh lets you be near the code"

parts = split(text, " ")
print(parts)
print(len(parts))

joined = join(parts, "-")
print(joined)

fixed = replace(text, "near the code", "near the machine")
print(fixed)

csv = "red,green,blue"
colors = split(csv, ",")
print(colors)
print(join(colors, " | "))