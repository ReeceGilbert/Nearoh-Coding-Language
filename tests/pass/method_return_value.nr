class Counter():
    def __init__(self, start):
        self.value = start
    def add(self, amount):
        self.value = self.value + amount
        return self.value
c = Counter(10)
print(c.add(5))
print(c.add(2))
print(c.value)
