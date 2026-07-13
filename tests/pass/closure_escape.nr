def outer():
    x = 42
    def inner():
        return x
    return inner
f = outer()
print(f())
