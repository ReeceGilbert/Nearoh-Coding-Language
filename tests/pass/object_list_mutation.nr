class Item():
    def __init__(self, name):
        self.name = name
items = [Item("a"), Item("b")]
print(items[0].name)
print(items[1].name)
items[0].name = "changed"
print(items[0].name)
print(items[1].name)
