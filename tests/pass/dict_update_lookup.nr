data = {
    "name": "Nearoh",
    "runs": 1
}
print(data["name"])
print(data["runs"])
data["runs"] = data["runs"] + 1
data["status"] = "alive"
print(data["runs"])
print(data["status"])
print(has(data, "name"))
print(has(data, "missing"))
