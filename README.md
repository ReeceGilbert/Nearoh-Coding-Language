# Nearoh Coding Language

Nearoh is a Python-inspired programming language written in C, focused on real usability, clean architecture, and long-term growth into a serious everyday language.

Nearoh is not a toy parser or throwaway syntax project. It is being built as a practical language/runtime I would genuinely want to use while maintaining full control over internals, runtime behavior, memory paths, and future low-level expansion.

## Links

Website:  
https://nearoh-coding-language.base44.app

GitHub:  
https://github.com/ReeceGilbert/Nearoh-Coding-Language

---

# Vision

Nearoh aims to combine the productivity and readability of Python with the control and extensibility of a C-backed runtime.

The goal is to create a language that feels familiar and productive at the surface, but remains fully understandable and expandable underneath.

## Core Goals

- Familiar Python-style workflow
- Clean, readable syntax
- Functions, classes, objects, methods, and scripting productivity
- Runtime written in C for control and performance
- Expandable native bridge for machine-facing systems
- Ability to build higher-level libraries on top of a small native core
- Long-term editor / IDE environment built around the language
- Strong architecture over rushed features

This project is being built to become a serious personal-use language, not just a demo.

---

# Current Status

## Major Milestone Reached

Nearoh now supports real multi-feature programs, expanded native builtins, file I/O, and basic multi-file execution through imports.

The language can now run larger examples from the command line, inspect tokens and AST output, execute classes and object systems, mutate lists and dictionaries, use built-in utility functions, read and write files, and import another `.nr` file into the current runtime environment.

Implemented systems currently include:

- Lexer
- Parser
- AST generation
- Runtime evaluator
- Scope / environment model
- Variables and reassignment
- Arithmetic
- Strings
- Numbers
- Booleans
- `if / elif / else`
- `while` loops
- `for ... in` list iteration
- Functions
- Return values
- Classes
- Automatic `__init__` constructors
- Object fields
- Member access
- Bound `self` methods
- Lists
- List indexing
- List index assignment
- Dictionaries
- Dictionary indexing
- Dictionary index assignment
- Member assignment
- Instance member container mutation
- Basic imports
- Multi-file program execution
- File I/O
- Runtime line/column error reporting
- Debug / token / AST CLI modes
- Core runtime safety checks

---

# Builtins

Nearoh currently includes the following native builtins:

- `print()`
- `len()`
- `type()`
- `str()`
- `num()`
- `append()`
- `range()`
- `keys()`
- `has()`
- `read_file()`
- `write_file()`
- `append_file()`

Example:

```python
items = []
items = append(items, "compiler")
items = append(items, "debugger")

print(len(items))
print(items[0])
print(items[1])

stats = {
    "runs": 1,
    "language": "Nearoh"
}

print(has(stats, "runs"))
print(keys(stats))
```

---

# File I/O

Nearoh now supports basic file reading, writing, and appending through native builtins.

```python
write_file("nearoh_file_test.txt", "Hello")
append_file("nearoh_file_test.txt", " Nearoh")

text = read_file("nearoh_file_test.txt")
print(text)
```

Expected output:

```text
Hello Nearoh
```

This is an important step toward Nearoh becoming a practical scripting language instead of a single-file interpreter demo.

---

# Imports

Nearoh now supports a first version of imports:

```python
import "examples/import_utils.nr"

say_hello("Reece")
print(favorite)
```

Example imported file:

```python
def say_hello(name):
    print("Hello", name)

favorite = "Nearoh"
```

The import system currently reads another `.nr` file, lexes it, parses it into an AST, and executes it in the current runtime environment.

This allows Nearoh programs to begin moving beyond one-file examples and into multi-file structure.

Current import support is intentionally simple. Future versions may add module ownership, namespaces, import caching, and cleaner memory ownership for imported AST/source data.

---

# CLI Usage

```bash
nearoh examples/hello.nr
nearoh --tokens examples/hello.nr
nearoh --ast examples/hello.nr
nearoh --debug examples/hello.nr
```

## Modes

- **Run** â executes the program normally
- **--tokens** â prints lexer output
- **--ast** â prints parsed AST
- **--debug** â prints source, tokens, AST, diagnostics, then execution

---

# Example Programs

The repository includes runnable examples such as:

- `examples/hello.nr`
- `examples/variables.nr`
- `examples/functions.nr`
- `examples/classes.nr`
- `examples/lists.nr`
- `examples/loops.nr`
- `examples/objects_and_lists.nr`
- `examples/dictionaries.nr`
- `examples/arena_showcase.nr`
- `examples/core_stress.nr`
- `examples/builtins.nr`
- `examples/file_io.nr`
- `examples/import_token_test.nr`
- `examples/import_utils.nr`
- `examples/import_main.nr`

---

# Showcase Example

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

Expected output:

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

# Core Stress Test

`core_stress.nr` is a larger regression-style example that proves several language systems work together.

It tests:

- Classes
- Constructors
- Instance fields
- Methods
- Bound `self`
- Lists
- Dictionaries
- List mutation
- Dictionary mutation
- Functions
- If / else logic
- For loops
- While loops
- Builtins
- Object state changes

Example pattern:

```python
class Player():
    def __init__(self, name, hp):
        self.name = name
        self.hp = hp
        self.inventory = []

    def add_item(self, item):
        self.inventory = append(self.inventory, item)

stats = {
    "runs": 1,
    "language": "Nearoh"
}

stats["runs"] = stats["runs"] + 1
```

This test helped verify that list and dictionary mutation now persists correctly through real runtime paths.

---

# Arena Showcase

`arena_showcase.nr` is a larger demonstration proving Nearoh can coordinate multiple systems together.

It uses:

- Classes
- Lists of objects
- Constructors
- Object state mutation
- Functions
- Conditional logic
- While loops
- For loops
- Runtime score tracking
- Multi-round battle simulation logic

This moves Nearoh beyond syntax demos into real executable projects.

---

# Recent Core Improvements

Recent cleanup and stabilization work included:

- Cleaner `main.c` CLI architecture
- Improved parser structure and error handling
- AST cleanup and memory handling fixes
- Value / list / dictionary system improvements
- Environment system cleanup
- Better runtime diagnostics with source locations
- Safer project-wide organization
- Fixed instance member container index assignment
- Fixed dictionary mutation persistence
- Added expanded utility builtins
- Added string/number conversion builtins
- Added basic file I/O
- Added basic import statement support
- Added regression-style example programs

Nearoh now has a stronger foundation for future growth.

---

# Why This Project Matters

Many hobby language projects stop at parsing expressions.

Nearoh already includes real runtime behavior:

- Executable programs
- User-defined functions
- Object-oriented systems
- Dynamic lists
- Dictionaries
- Scope handling
- Builtins
- File I/O
- Imports
- Structured examples
- Command-line tooling
- Debugging modes
- Runtime diagnostics

That means the project is moving into genuine language engineering territory.

Nearoh is still early, but it is already capable of running meaningful programs across multiple interacting systems.

---

# Roadmap

## Near-Term

- Cleaner diagnostics and error UX
- More runtime safety checks
- Better builtin error behavior
- More standard library utilities
- Import cleanup and module ownership
- Basic module caching
- More example programs and regression tests

## Mid-Term

- Namespaced modules
- Expanded builtins
- Better performance paths
- Improved memory systems
- More complete standard library
- Tooling improvements
- Cleaner test runner

## Long-Term

- Native graphics / window bridge
- Input / timing systems
- File and path utilities
- Bytecode VM or compiled backend research
- Dedicated Nearoh editor / IDE
- Potential self-hosted growth path

---

# Philosophy

Nearoh is being built carefully and intentionally.

The goal is not to copy Python line-for-line.

The goal is to preserve what makes Python productive while gaining deeper ownership of the machine underneath it.

Readable high-level development on top.  
Low-level power underneath.

---

# Author

Built by Reece Gilbert.

This project reflects years of programming curiosity, systems experimentation, graphics work, simulation building, and the drive to create something real from scratch.

---

# Current Stage

Nearoh is early, active, and growing quickly.

Every milestone is focused on turning it into a real usable language rather than a superficial prototype.

The current stage is centered on strengthening the runtime, expanding the builtin layer, improving imports, growing the standard library, and preparing the project for more serious tooling and editor work.