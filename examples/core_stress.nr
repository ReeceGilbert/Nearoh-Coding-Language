print("=== Nearoh Core Stress Test ===")

class Player():
    def __init__(self, name, hp):
        self.name = name
        self.hp = hp
        self.inventory = []

    def damage(self, amount):
        self.hp = self.hp - amount

    def heal(self, amount):
        self.hp = self.hp + amount

    def add_item(self, item):
        self.inventory[len(self.inventory)] = item

    def status(self):
        print(self.name, "HP:", self.hp, "Items:", len(self.inventory))


def show_team(team):
    print("Team size:", len(team))

    for p in team:
        p.status()


def damage_all(team, amount):
    for p in team:
        p.damage(amount)


def heal_if_low(player):
    if player.hp < 50:
        player.heal(25)
        print(player.name, "was healed")
    else:
        print(player.name, "does not need healing")


p1 = Player("Reece", 100)
p2 = Player("Nearoh", 80)
p3 = Player("Bug Hunter", 40)

p1.add_item("compiler")
p1.add_item("debugger")
p2.add_item("runtime")
p3.add_item("coffee")

team = [p1, p2, p3]

show_team(team)

print("--- damage all ---")
damage_all(team, 35)
show_team(team)

print("--- healing pass ---")
for p in team:
    heal_if_low(p)

show_team(team)

stats = {
    "runs": 1,
    "players": len(team),
    "language": "Nearoh"
}

print("Stats:")
print(stats["language"])
print(stats["players"])
print(stats["runs"])

stats["runs"] = stats["runs"] + 1
print("Runs after update:", stats["runs"])

i = 0
while i < len(team):
    print("While loop player:", team[i].name)
    i = i + 1

print("=== Stress Test Complete ===")