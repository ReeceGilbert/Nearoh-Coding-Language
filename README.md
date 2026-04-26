# Nearoh Coding Language

A Python-inspired programming language written in C with a focus on real usability, clean architecture, and long-term self-hosted growth.

Nearoh is not a toy parser or syntax gimmick. It is being built as a practical language/runtime I would genuinely want to use in daily workflow—while keeping full control over internals, performance paths, and future low-level expansion.

**Website:** [https://nearoh-coding-language.base44.app](https://nearoh-coding-language.base44.app)

---

# Vision

Nearoh aims to combine the productivity of Python with the control and extensibility of a C-backed runtime.

## Core Goals

* Familiar Python-style workflow
* Clean readable syntax
* Classes, objects, methods, and high-level productivity
* Runtime written in C for control and performance
* Expandable native bridge for machine-facing systems
* Future editor / IDE environment built around the language
* Strong foundations over rushed features

This project is being built to become a serious personal-use language, not just a demo.

---

# Current Status

## Major Milestone Reached

Nearoh now executes real multi-feature programs including:

* Classes
* Automatic `__init__` constructors
  n- Object field storage
* Object member access
* Methods with bound `self`
* Lists
* List indexing
* List index assignment
* `for ... in` iteration over lists
* Variables / reassignment
* Arithmetic
* Strings
* `if / else`
* `while`
* Functions
* Return values
* Scope environments
* Builtin `print`
* Builtin `len`
* CLI execution modes (`run`, `--tokens`, `--ast`, `--debug`)

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

items = [10, 20, 30]
items[1] = 99

print(items[1])
print(len(items))
print(len("Nearoh"))
```

Expected Output:

```text
1
2
3
4
99
3
6
```

---

# Why This Project Matters

Most hobby language projects stop at parsing.

Nearoh already includes:

* Lexer
* Parser
* AST system
* Runtime evaluator
* Environment / scope model
* Functions
* Classes / instances / methods
* Dynamic lists
* Builtins system
* Growing standard language behavior

That means Nearoh is moving beyond syntax into real language engineering.

---

# Long-Term Direction

Planned future growth includes:

* Modules / imports
* Dictionaries / maps
* Better error reporting
* File I/O
* Math / utility standard library
* Graphics + windowing bridge
* Improved performance paths
* Bytecode or compiled backend research
* Dedicated Nearoh editor / IDE

---

# Philosophy

Nearoh is being built carefully and intentionally.

The goal is not to copy Python line-for-line.

The goal is to preserve what makes Python productive while gaining deeper ownership of the machine underneath it.

---

# Author

Built by Reece Gilbert.

This project reflects years of programming curiosity, systems experimentation, and the desire to create something real from scratch.
