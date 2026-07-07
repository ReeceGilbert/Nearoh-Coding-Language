class Box():
    def __init__(self, value):
        self.value = value
    def set(self, value):
        self.value = value
a = Box(1)
b = Box(2)
a.set(99)
print(a.value)
print(b.value)
