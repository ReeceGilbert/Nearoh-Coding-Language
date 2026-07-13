def make_closure(value):
    def get():
        return value
    return get
saved = make_closure(123)
live = [1, 2, 3]
i = 0
while i < 500:
    garbage = []
    garbage[0] = garbage
    garbage = none
    i = i + 1
print(saved())
print(live)
