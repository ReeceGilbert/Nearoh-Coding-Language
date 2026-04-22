# Nearoh Coding Language

A Python-inspired programming language written in C with a focus on real usability, clean architecture, and long-term self-hosted growth.

Nearoh is not a toy parser or syntax gimmick. It is being built as a practical language/runtime I would genuinely want to use in daily workflow—while keeping full control over internals, performance paths, and future low-level expansion.

**Website:**  
https://nearoh-coding-language.base44.app

---

# Vision

Nearoh aims to combine the productivity of Python with the control and extensibility of a C-backed runtime.

## Core goals

- Familiar Python-style workflow
- Clean readable syntax
- Classes, objects, methods, and high-level productivity
- Runtime written in C for control and performance
- Expandable native bridge for machine-facing systems
- Future editor / IDE environment built around the language
- Strong foundations over rushed features

This project is being built to become a serious personal-use language, not just a demo.

---

# Current Status

## Major Milestone Reached

Nearoh now executes real multi-feature programs including:

- Classes
- Automatic `__init__` constructors
- Object field storage
- Object member access
- Methods with bound `self`
- Lists
- `for ... in` iteration over lists
- Variables / reassignment
- Arithmetic
- `if / else`
- `while`
- Functions
- Return values
- Scope environments
- Builtin `print`

---

# Example Working Program

```python
class Vector2():
    def __init__(self, x, y):
        self.x = x
        self.y = y

points = [Vector2(1, 2), Vector2(3, 4)]

for p in points:
    print(p.x)
    print(p.y)
