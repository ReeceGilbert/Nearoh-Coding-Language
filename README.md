# Nearoh Coding Language

Nearoh is a Python-inspired programming language written in C, focused on real usability, clean architecture, and long-term growth into a serious everyday language.

Nearoh is not a toy parser or throwaway syntax project. It is being built as a practical language/runtime I would genuinely want to use, while keeping full control over internals, runtime behavior, memory paths, imports, diagnostics, and future low-level expansion.

## Links

Website: https://nearoh-coding-language.base44.app  
GitHub: https://github.com/ReeceGilbert/Nearoh-Coding-Language

---

# Vision

Nearoh aims to combine the productivity and readability of Python with the control and extensibility of a C-backed runtime.

The goal is to create a language that feels familiar and productive at the surface, but remains understandable, inspectable, and expandable underneath.

Nearoh is being built toward a future where the language can grow from a clean interpreted core into a practical personal-use language with its own libraries, tooling, native bridge, and eventually a dedicated editor / IDE.

## Core Goals

- Familiar Python-style workflow
- Clean, readable syntax
- Functions, classes, objects, methods, and scripting productivity
- Runtime written in C for control and future performance work
- Expandable native bridge for machine-facing systems
- Ability to build higher-level libraries on top of a small native core
- Stable imports and multi-file program structure
- Source-aware diagnostics that point to the correct file, line, and column
- Long-term editor / IDE environment built around the language
- Strong architecture over rushed features

This project is being built to become a serious personal-use language, not just a demo.

---

# Current Status

## Major Runtime Foundation Milestone Reached

Nearoh now supports real multi-feature programs, expanded native built-ins, file I/O, multi-file execution through a tracked import system, and an early native IDE shell built from scratch in C with Win32/GDI.

The language can run larger examples from the command line or from the IDE, inspect tokens and AST output, execute classes and object systems, mutate lists and dictionaries, use built-in utility functions, read and write files, import other `.nr` files, detect circular imports, skip duplicate imports, and report runtime errors with source-aware file/line/column diagnostics.

Implemented systems currently include:

- Lexer
- Parser
- AST generation
- Runtime evaluator
- Scope/environment model
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
- File I/O
- Tracked imports
- Duplicate import skipping
- Circular import detection
- Import path normalization
- File-relative imports
- Compatibility fallback for older working-directory-style import paths
- Source-aware AST annotation
- Runtime file/line/column error reporting
- Expected-failure regression tests
- Debug / token / AST CLI modes
- Early native Win32/GDI IDE shell
- IDE editor buffer with typing, cursor movement, scrolling, and click-to-place cursor
- IDE editor and console line numbers
- IDE console output panel with scrolling
- IDE toolbar buttons for Run, Tokens, AST, and Debug
- IDE process runner with captured stdout/stderr
- Core runtime safety checks

---

# Builtins

Nearoh currently includes the following native built-ins:

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

Nearoh supports basic file reading, writing, and appending through native built-ins.

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

Nearoh now has a tracked import system for multi-file programs.

Example:

```python
import "utils.nr"

say_hello("Reece")
print(favorite)
```

Example imported file:

```python
def say_hello(name):
    print("Hello", name)

favorite = "Nearoh"
```

Current import behavior:

- Reads another `.nr` file
- Lexes it
- Parses it into an AST
- Annotates the imported AST with its source file path
- Executes it in the current runtime environment
- Tracks imported files in the runtime
- Skips duplicate imports
- Detects circular imports
- Normalizes path separators
- Supports leading `./` path normalization
- Resolves relative imports from the file doing the importing
- Falls back to working-directory-style paths for older examples
- Reports failed imports with the attempted path

Example missing import error:

```text
Runtime error at examples/modules/missing_main.nr line 1 col 1: Could not read imported file: examples/modules/missing_utils.nr
```

Example imported-file runtime error:

```text
Runtime error at examples/modules/bad_utils.nr line 1 col 7: Undefined variable.
```

This means Nearoh can now point to the correct imported source file when runtime errors happen across multi-file programs.

Namespaces and true module objects are still future work, but the current import system is now a real foundation instead of a one-off include-style loader.

---

# CLI Usage

```bash
nearoh examples/hello.nr
nearoh --tokens examples/hello.nr
nearoh --ast examples/hello.nr
nearoh --debug examples/hello.nr
```

## Modes

- **Run** -> executes the program normally
- **--tokens** -> prints lexer output
- **--ast** -> prints parsed AST
- **--debug** -> prints source, tokens, AST, diagnostics, then execution

---

# Nearoh IDE

Nearoh now includes an early native IDE prototype in the `ide/` folder.

The IDE is not a web wrapper, tkinter app, or framework mockup. It is being built from scratch in C using the raw Win32 API and GDI.

Current IDE features include:

- Native Win32 window
- Custom text buffer/editor
- Typing and editing
- Cursor movement
- Mouse click-to-place cursor
- Editor scrolling
- Editor line numbers
- Console output panel
- Console line numbers
- Console scrolling
- Save/load support for the current working file
- Toolbar buttons for Run, Tokens, AST, and Debug
- Keyboard shortcuts for Run and Tokens
- Captured stdout/stderr from the Nearoh executable
- Absolute-path process launching with repo-root working directory handling

Current IDE controls:

- **Run button** -> executes the current `.nr` file
- **Tokens button** -> runs `nearoh --tokens`
- **AST button** -> runs `nearoh --ast`
- **Debug button** -> runs `nearoh --debug`
- **Ctrl+R** -> run current file
- **Ctrl+T** -> inspect tokens
- **Mouse wheel over editor** -> scroll editor
- **Mouse wheel over console** -> scroll console

Example build command:

```bash
gcc ide/main.c ide/buffer.c ide/editor.c ide/file_io.c ide/console.c ide/process.c -o ide/nearoh_ide.exe -lgdi32
```

This IDE is still early, but it already proves that Nearoh can be edited, run, inspected, and debugged from its own dedicated native tool instead of only from the command line.

---

# Testing

Nearoh includes a PowerShell regression test runner:

```powershell
.\run_tests.ps1
```

The runner checks both successful programs and expected-failure programs.

Pass tests must exit successfully.

Expected-failure tests must fail and include specific expected error text.

Currently tested behavior includes:

- Basic program execution
- Functions
- Classes
- Dictionaries
- Import-once behavior
- Path normalization
- File-relative imports
- Nested imports
- Core stress behavior
- Arena showcase behavior
- Circular import failure
- Runtime errors inside imported files
- Missing import path diagnostics

Expected ending:

```text
ALL TESTS PASSED
```

This gives Nearoh a safety net before adding larger systems like namespaces, more builtins, standard library utilities, or the future IDE.

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
- `examples/import_once_main.nr`
- `examples/import_path_normalize_main.nr`
- `examples/import_cycle_a.nr`
- `examples/import_cycle_b.nr`
- `examples/modules/main.nr`
- `examples/modules/main_nested.nr`
- `examples/modules/bad_main.nr`
- `examples/modules/missing_main.nr`

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

This test helps verify that multiple runtime systems continue working together as the language grows.

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
- Source-path annotation for AST nodes
- Value/list / dictionary system improvements
- Environment system cleanup
- Better runtime diagnostics with source file, line, and column
- Runtime import tracking
- Duplicate import skipping
- Circular import detection
- Import path normalization
- File-relative import resolution
- Missing import errors that show the attempted path
- Safer project-wide organization
- Fixed instance member container index assignment
- Fixed dictionary mutation persistence
- Added expanded utility built-ins
- Added string/number conversion built-ins
- Added basic file I/O
- Added regression-style example programs
- Added expected-failure tests
- Added `run_tests.ps1`
- Added early native Win32/GDI IDE shell
- Added IDE editor line numbers, console line numbers, toolbar run buttons, console rendering, and click-to-place cursor navigation

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
- Multi-file execution
- Source-aware diagnostics
- Structured examples
- Command-line tooling
- Debugging modes
- Regression tests
- Early native IDE tooling

That means the project is moving into genuine language engineering territory.

Nearoh is still early, but it is already capable of running meaningful programs across multiple interacting systems.

---

# Roadmap

## Near-Term

- Continue improving diagnostics and error UX
- Add more runtime safety checks
- Improve built-in error behavior
- Add more standard library utilities
- Clean up import memory ownership further
- Begin designing module namespaces/module objects
- Expand regression tests
- Add more example programs

## Mid-Term

- Namespaced modules
- Expanded builtins
- Better performance paths
- Improved memory systems
- More complete standard library
- File and path utilities
- Tooling improvements
- Cleaner automated test workflow
- Continue expanding the native IDE shell

## Long-Term

- Native graphics/window bridge
- Input/timing systems
- Larger standard library built in Nearoh
- Bytecode VM or compiled backend research
- Dedicated Nearoh editor / IDE with project navigation, richer editing, and diagnostics
- Possible self-hosted growth path

---

# Philosophy

Nearoh is being built carefully and intentionally.

The goal is not to copy Python line-for-line. The goal is to preserve what makes Python productive while gaining deeper ownership of the machine underneath it.

Readable high-level development on top. Low-level power underneath.

Nearoh code because it lets you be near the code.

---

# Author

Built by Reece Gilbert.

ChatGPT was used as a coding assistant during planning, debugging, documentation, and implementation work. The project direction, repository, testing, integration, and final design decisions are owned by Reece Gilbert.

This project reflects years of programming curiosity, systems experimentation, graphics work, simulation building, and the drive to create something real from scratch.

---

# Current Stage

Nearoh is early, active, and growing quickly.

The current stage is centered on strengthening the runtime, improving imports, expanding diagnostics, growing the builtin layer, adding regression tests, and growing the new native IDE from a working shell into a more complete language tool.

Every milestone is focused on turning Nearoh into a real, usable language rather than a superficial prototype.
