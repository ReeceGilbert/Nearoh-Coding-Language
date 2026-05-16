class Box():
    def __init__(self):
        self.items = ["empty"]

    def set_item(self, value):
        self.items[0] = value

    def show(self):
        print(self.items[0])


box = Box()
box.show()
box.set_item("fixed")
box.show()