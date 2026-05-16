print("=== Builtins Test ===")

items = []
items = append(items, "compiler")
items = append(items, "debugger")

print(len(items))
print(items[0])
print(items[1])
print(type(items))
print(type(items[0]))
print(type(123))
print(type(true))
print(type(none))

nums = range(5)

for n in nums:
    print("range item:", n)

stats = {
    "runs": 2,
    "language": "Nearoh"
}

print(has(stats, "runs"))
print(has(stats, "missing"))

ks = keys(stats)

for k in ks:
    print("key:", k)

print("=== Builtins Test Complete ===")