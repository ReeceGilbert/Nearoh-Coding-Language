class Fighter():
    def __init__(self, name, hp, attack, defense, speed):
        self.name = name
        self.hp = hp
        self.maxHp = hp
        self.attack = attack
        self.defense = defense
        self.speed = speed
        self.alive = true
        self.wins = 0

class TeamStats():
    def __init__(self, name):
        self.name = name
        self.roundsWon = 0
        self.totalDamage = 0
        self.fightersAlive = 0

class BattleState():
    def __init__(self):
        self.round = 1
        self.turn = 1
        self.redDamage = 0
        self.blueDamage = 0
        self.finished = false


def printLine():
    print("------------------------------")


def printTitle():
    printLine()
    print("Nearoh Arena Showcase")
    print("Classes, lists, loops, fields, functions, and runtime logic")
    printLine()


def clampZero(value):
    if value < 0:
        return 0
    return value


def isAlive(fighter):
    if fighter.hp > 0:
        return true
    return false


def updateAlive(fighter):
    if fighter.hp <= 0:
        fighter.hp = 0
        fighter.alive = false
    else:
        fighter.alive = true


def resetFighter(fighter):
    fighter.hp = fighter.maxHp
    fighter.alive = true


def resetTeam(team):
    for fighter in team:
        resetFighter(fighter)


def countAlive(team):
    count = 0
    for fighter in team:
        if fighter.alive:
            count = count + 1
    return count


def teamHealth(team):
    total = 0
    for fighter in team:
        total = total + fighter.hp
    return total


def printFighter(fighter):
    print(fighter.name)
    print(fighter.hp)
    print(fighter.attack)
    print(fighter.defense)
    print(fighter.speed)


def printTeamStatus(name, team):
    printLine()
    print(name)
    for fighter in team:
        print(fighter.name)
        print(fighter.hp)
    printLine()


def printCompactTeamStatus(name, team):
    print(name)
    for fighter in team:
        print(fighter.name)
        print(fighter.hp)


def basicDamage(attacker, defender, turn):
    base = attacker.attack - defender.defense
    if base < 1:
        base = 1

    bonus = turn % 3
    damage = base + bonus

    if attacker.speed > defender.speed:
        damage = damage + 1

    if attacker.hp < 10:
        damage = damage + 2

    return damage


def applyDamage(defender, damage):
    defender.hp = defender.hp - damage
    updateAlive(defender)


def chooseTarget(team, seed):
    aliveCount = countAlive(team)

    if aliveCount <= 0:
        return team[0]

    index = seed % len(team)
    tries = 0

    while tries < len(team):
        candidate = team[index]
        if candidate.alive:
            return candidate

        index = index + 1
        if index >= len(team):
            index = 0

        tries = tries + 1

    return team[0]


def performAttack(state, attacker, defender, redStats, blueStats, attackingRed):
    if attacker.alive:
        if defender.alive:
            damage = basicDamage(attacker, defender, state.turn)
            applyDamage(defender, damage)

            if attackingRed:
                state.redDamage = state.redDamage + damage
                redStats.totalDamage = redStats.totalDamage + damage
            else:
                state.blueDamage = state.blueDamage + damage
                blueStats.totalDamage = blueStats.totalDamage + damage

            print(attacker.name)
            print("hits")
            print(defender.name)
            print(damage)

            if defender.alive == false:
                attacker.wins = attacker.wins + 1
                print(defender.name)
                print("falls")


def runRound(state, redTeam, blueTeam, redStats, blueStats):
    printLine()
    print("Round")
    print(state.round)
    printLine()

    state.turn = 1

    while state.turn <= 12:
        redAlive = countAlive(redTeam)
        blueAlive = countAlive(blueTeam)

        if redAlive <= 0:
            state.finished = true
            return none

        if blueAlive <= 0:
            state.finished = true
            return none

        redIndex = state.turn % len(redTeam)
        blueIndex = (state.turn + 1) % len(blueTeam)

        redAttacker = chooseTarget(redTeam, redIndex)
        blueTarget = chooseTarget(blueTeam, blueIndex)
        performAttack(state, redAttacker, blueTarget, redStats, blueStats, true)

        redAlive = countAlive(redTeam)
        blueAlive = countAlive(blueTeam)

        if blueAlive <= 0:
            state.finished = true
            return none

        blueAttacker = chooseTarget(blueTeam, blueIndex)
        redTarget = chooseTarget(redTeam, redIndex + 2)
        performAttack(state, blueAttacker, redTarget, redStats, blueStats, false)

        state.turn = state.turn + 1

    printCompactTeamStatus("Red after round", redTeam)
    printCompactTeamStatus("Blue after round", blueTeam)


def awardRound(redTeam, blueTeam, redStats, blueStats):
    redAlive = countAlive(redTeam)
    blueAlive = countAlive(blueTeam)
    redHp = teamHealth(redTeam)
    blueHp = teamHealth(blueTeam)

    redStats.fightersAlive = redAlive
    blueStats.fightersAlive = blueAlive

    if redAlive > blueAlive:
        redStats.roundsWon = redStats.roundsWon + 1
        print("Red wins the round by survival")
        return none

    if blueAlive > redAlive:
        blueStats.roundsWon = blueStats.roundsWon + 1
        print("Blue wins the round by survival")
        return none

    if redHp > blueHp:
        redStats.roundsWon = redStats.roundsWon + 1
        print("Red wins the round by health")
        return none

    if blueHp > redHp:
        blueStats.roundsWon = blueStats.roundsWon + 1
        print("Blue wins the round by health")
        return none

    print("Round is a draw")


def printScoreboard(redStats, blueStats):
    printLine()
    print("Scoreboard")
    print(redStats.name)
    print(redStats.roundsWon)
    print(redStats.totalDamage)
    print(redStats.fightersAlive)
    print(blueStats.name)
    print(blueStats.roundsWon)
    print(blueStats.totalDamage)
    print(blueStats.fightersAlive)
    printLine()


def printChampion(redStats, blueStats):
    printLine()
    print("Final Result")

    if redStats.roundsWon > blueStats.roundsWon:
        print("Red team wins the arena")
        return none

    if blueStats.roundsWon > redStats.roundsWon:
        print("Blue team wins the arena")
        return none

    if redStats.totalDamage > blueStats.totalDamage:
        print("Red team wins by total damage")
        return none

    if blueStats.totalDamage > redStats.totalDamage:
        print("Blue team wins by total damage")
        return none

    print("The arena ends in a draw")


def printMvp(team):
    best = team[0]

    for fighter in team:
        if fighter.wins > best.wins:
            best = fighter

    print("MVP")
    print(best.name)
    print(best.wins)


printTitle()

redTeam = [
    Fighter("Red Blade", 42, 10, 3, 7),
    Fighter("Red Forge", 55, 8, 6, 3),
    Fighter("Red Spark", 34, 12, 2, 10)
]

blueTeam = [
    Fighter("Blue Fang", 40, 11, 3, 8),
    Fighter("Blue Stone", 60, 7, 7, 2),
    Fighter("Blue Ghost", 32, 13, 1, 11)
]

redStats = TeamStats("Red Team")
blueStats = TeamStats("Blue Team")
state = BattleState()

printTeamStatus("Red Team Starting Stats", redTeam)
printTeamStatus("Blue Team Starting Stats", blueTeam)

while state.round <= 3:
    resetTeam(redTeam)
    resetTeam(blueTeam)
    state.finished = false
    runRound(state, redTeam, blueTeam, redStats, blueStats)
    awardRound(redTeam, blueTeam, redStats, blueStats)
    printScoreboard(redStats, blueStats)
    state.round = state.round + 1

printChampion(redStats, blueStats)
printMvp(redTeam)
printMvp(blueTeam)
printLine()
print("Showcase complete")
