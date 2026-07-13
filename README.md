# Nearoh Coding Language

Nearoh is a Python-inspired programming language written in C, focused on real usability, clean architecture, and long-term growth into a serious everyday language.

Nearoh is not a toy parser or throwaway syntax experiment. It is being built as a practical language/runtime with full control over the internals: parsing, runtime behavior, values, memory paths, imports, diagnostics, testing, tooling, and future low-level expansion.

Current repository size:

* **11,713 total lines**
* **120 files counted**
* **2,532 lines in `runtime.c`**
* **1,304 lines in `builtins.c`**
* **1,111 lines in `parser.c`**
* **739 lines in `lexer.c`**
* **687 lines in `value.c`**
* **384 lines in the new garbage collector**
* **1,600+ lines of native IDE implementation**
* **15 custom pass tests**
* **3 active custom expected-failure tests**
* **Full regression suite passing under both GC stress mode and normal threshold-based collection**

Nearoh has moved beyond just being a custom interpreter. It is becoming a small language ecosystem: language core, runtime, builtins, imports, examples, regression tests, CLI tooling, a native IDE, heap-managed closures, shared mutable object identity, and a real mark-and-sweep garbage collector.

## Links

Website: https://nearoh-coding-language.base44.app
GitHub: https://github.com/ReeceGilbert/Nearoh-Coding-Language

---

# Vision

Nearoh aims to combine the productivity and readability of Python with the control and extensibility of a C-backed runtime.

The goal is to create a language that feels familiar and productive at the surface, while remaining understandable, inspectable, and expandable underneath.

Nearoh is being built toward a future where the language can grow from a clean interpreted core into a practical personal-use language with its own libraries, tooling, native bridge, graphics/input systems, and dedicated editor / IDE.

## Core Goals

* Familiar Python-style workflow
* Clean, readable syntax
* Functions, classes, objects, methods, and scripting productivity
* Runtime written in C for control and future performance work
* Expandable native bridge for machine-facing systems
* Ability to build higher-level libraries on top of a small native core
* Stable imports and multi-file program structure
* Source-aware diagnostics that point to the correct file, line, and column
* Regression tests that catch runtime behavior changes
* Long-term editor / IDE environment built around the language
* Strong architecture over rushed features

This project is being built to become a serious personal-use language, not just a demo.

---

# Current Status

## Runtime Foundation and Tooling Milestone

Nearoh now supports real multi-feature programs, expanded native built-ins, file I/O, multi-file execution through a tracked import system, golden-output regression tests, expected-failure tests, heap-allocated lexical environments, escaped closures, shared mutable container identity, cycle-safe printing, a non-moving mark-and-sweep garbage collector, and an early native IDE shell built from scratch in C with Win32/GDI.

The language can run larger examples from the command line or from the IDE, inspect tokens and AST output, execute classes and object systems, mutate lists and dictionaries, use built-in utility functions, read and write files, import other `.nr` files, detect circular imports, skip duplicate imports, and report runtime errors with source-aware file/line/column diagnostics.

Implemented systems currently include:

* Lexer
* Parser
* AST generation
* Runtime evaluator
* Scope/environment model
* Heap-allocated lexical environments
* Escaped closures
* Shared mutable object identity
* Non-moving mark-and-sweep garbage collector
* Root marking and gray-stack tracing
* Cycle collection
* Threshold-based automatic collection at safe points
* Optional full GC stress mode
* Cycle-safe list and dictionary printing
* Variables and reassignment
* Arithmetic
* Strings
* Numbers
* Booleans
* `if / elif / else`
* `while` loops
* `for ... in` list iteration
* Functions
* Return values
* Classes
* Automatic `__init__` constructors
* Object fields
* Member access
* Bound `self` methods
* Lists
* List indexing
* List index assignment
* Dictionaries
* Dictionary indexing
* Dictionary index assignment
* Member assignment
* Instance member container mutation
* File I/O
* Tracked imports
* Duplicate import skipping
* Circular import detection
* Import path normalization
* File-relative imports
* Compatibility fallback for older working-directory-style import paths
* Source-aware AST annotation
* Runtime file/line/column error reporting
* Expected-failure regression tests
* Golden-output pass tests
* Debug / token / AST CLI modes
* Early native Win32/GDI IDE shell
* IDE editor buffer with typing, cursor movement, scrolling, and click-to-place cursor
* IDE editor and console line numbers
* IDE console output panel with scrolling
* IDE toolbar buttons for Run, Tokens, AST, and Debug
* IDE process runner with captured stdout/stderr
* Core runtime safety checks

---

# Project Size

Current counted project size:

```text
TOTAL LINES: 11,713
FILES COUNTED: 120
```

Largest core files:

```text
runtime.c       2,532 lines
builtins.c      1,304 lines
parser.c        1,111 lines
lexer.c           739 lines
value.c           687 lines
ast.c             640 lines
gc.c              384 lines
main.c            231 lines
env.c             229 lines
```

Native IDE files:

```text
ide/main.c       654 lines
ide/editor.c     279 lines
ide/buffer.c     258 lines
ide/console.c    146 lines
ide/file_io.c     96 lines
ide/process.c     95 lines
```

The native IDE implementation is roughly **1,600 lines** of C across the main window, editor, buffer, console, file I/O, and process runner, with additional header files supporting the subsystem.

---


# Memory Management

Nearoh now has a real runtime memory system instead of relying on inconsistent manual ownership.

The current collector is a **non-moving stop-the-world mark-and-sweep garbage collector**. Managed runtime objects are linked into a central heap list and traced from runtime roots.

GC-managed object types currently include:

* Functions
* Classes
* Instances
* Bound methods
* Lists
* Dictionaries
* Heap-allocated environments

The collector includes:

* Object headers with type, mark state, size, and linked-list membership
* Explicit root marking
* A gray stack for graph traversal
* Recursive tracing through object references
* Environment-chain tracing
* Sweep logic for unreachable objects
* Byte accounting and adaptive collection thresholds
* Safe-point collection after complete top-level statements
* Optional `NEAROH_GC_STRESS` mode that collects at every safe point
* Full shutdown cleanup through the same managed-object system

This change fixed one of the most important runtime ownership problems: closures can now safely escape the function that created them because captured environments live on the heap.

Example:

```python
def outer():
    x = 42

    def inner():
        return x

    return inner

f = outer()
print(f())
```

Expected output:

```text
42
```

Nearoh also preserves shared mutable identity:

```python
a = [1]
b = a
b[0] = 9

print(a)
```

Expected output:

```text
[9]
```

Cyclic containers are supported and reclaimed when unreachable. Printing is cycle-aware, so a self-referential list prints safely instead of recursing forever:

```python
a = []
a[0] = a
print(a)
```

Expected output:

```text
[[...]]
```

The complete regression suite passed with GC stress mode enabled, meaning a full collection occurred after every top-level statement. The suite also passed again after returning to normal threshold-based collection.

---

# Builtins

Nearoh currently includes native built-ins such as:

* `print()`
* `len()`
* `type()`
* `str()`
* `num()`
* `append()`
* `range()`
* `keys()`
* `values()`
* `has()`
* `read_file()`
* `write_file()`
* `append_file()`
* String tools such as lower/upper/title/contains/substr/split/join/replace
* Numeric utilities such as abs/floor/ceil/round/min/max/clamp/randint
* Timing utility such as `clock_ms()`

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

Nearoh has a tracked import system for multi-file programs.

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

* Reads another `.nr` file
* Lexes it
* Parses it into an AST
* Annotates the imported AST with its source file path
* Executes it in the current runtime environment
* Tracks imported files in the runtime
* Skips duplicate imports
* Detects circular imports
* Normalizes path separators
* Supports leading `./` path normalization
* Resolves relative imports from the file doing the importing
* Falls back to working-directory-style paths for older examples
* Reports failed imports with the attempted path

Example missing import error:

```text
Runtime error at examples/modules/missing_main.nr line 1 col 1: Could not read imported file: examples/modules/missing_utils.nr
```

Example imported-file runtime error:

```text
Runtime error at examples/modules/bad_utils.nr line 1 col 7: Undefined variable.
```

This means Nearoh can point to the correct imported source file when runtime errors happen across multi-file programs.

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

* **Run** -> executes the program normally
* **--tokens** -> prints lexer output
* **--ast** -> prints parsed AST
* **--debug** -> prints source, tokens, AST, diagnostics, then execution

---

# Nearoh IDE

Nearoh now includes an early native IDE prototype in the `ide/` folder.

The IDE is not a web wrapper, tkinter app, or framework mockup. It is being built from scratch in C using the raw Win32 API and GDI.

Current IDE features include:

* Native Win32 window
* Custom text buffer/editor
* Typing and editing
* Cursor movement
* Mouse click-to-place cursor
* Editor scrolling
* Editor line numbers
* Console output panel
* Console line numbers
* Console scrolling
* Save/load support for the current working file
* Toolbar buttons for Run, Tokens, AST, and Debug
* Keyboard shortcuts for Run and Tokens
* Captured stdout/stderr from the Nearoh executable
* Absolute-path process launching with repo-root working directory handling

Current IDE controls:

* **Run button** -> executes the current `.nr` file
* **Tokens button** -> runs `nearoh --tokens`
* **AST button** -> runs `nearoh --ast`
* **Debug button** -> runs `nearoh --debug`
* **Ctrl+R** -> run current file
* **Ctrl+T** -> inspect tokens
* **Mouse wheel over editor** -> scroll editor
* **Mouse wheel over console** -> scroll console

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

Nearoh now has two kinds of regression tests:

1. **Pass tests**

   * Must exit successfully.
   * Some also have exact expected-output files.
   * These catch behavior changes even when the program still exits successfully.

2. **Expected-failure tests**

   * Must fail.
   * Must include specific expected error text.
   * These lock down diagnostics and runtime error behavior.

Current custom pass tests include:

* `arithmetic_precedence.nr`
* `boolean_short_circuit.nr`
* `closure_escape.nr`
* `dict_update_lookup.nr`
* `for_loop_function_scope.nr`
* `for_loop_variable_scope.nr`
* `function_return_chain.nr`
* `function_scope.nr`
* `gc_cycle_stress.nr`
* `list_index_append.nr`
* `method_return_value.nr`
* `object_list_mutation.nr`
* `self_cycle.nr`
* `shared_list_identity.nr`
* `two_instances_separate_state.nr`
* `while_counter.nr`

Current active custom expected-failure tests include:

* `bad_constructor_arg_count.nr`
* `bad_list_index_out_of_bounds.nr`
* `bad_undefined_member.nr`

`nested_containers.nr` remains in the repository as a historical failure case from before nested container assignment support was repaired.

Currently tested behavior includes:

* Arithmetic precedence
* Function scope isolation
* Function return chaining
* Method return values
* Object field mutation
* Separate state between class instances
* Objects stored inside lists
* List index assignment
* List append through index assignment at the end
* Dictionary updates and lookup
* Boolean short-circuiting
* While-loop mutation
* For-loop behavior in local and function scopes
* Basic program execution
* Functions
* Classes
* Dictionaries
* Import-once behavior
* Path normalization
* File-relative imports
* Nested imports
* Core stress behavior
* Arena showcase behavior
* Constructor argument count failures
* List index out-of-bounds failures
* Undefined member failures
* Circular import failure
* Runtime errors inside imported files
* Missing import path diagnostics
* Builtin error behavior
* Escaped closure lifetime
* Shared list identity
* Self-referential containers
* Cycle-safe printing
* Repeated cyclic garbage creation
* Live closure preservation across collections
* Live container preservation across collections
* Full mark-and-sweep operation under GC stress mode

Expected ending:

```text
ALL TESTS PASSED
```

The regression suite has already caught and prevented real scope bugs, including function-local assignment overwriting globals and for-loop variables inside functions overwriting globals.

---

# Example Programs

The repository includes runnable examples such as:

* `examples/hello.nr`
* `examples/variables.nr`
* `examples/functions.nr`
* `examples/classes.nr`
* `examples/lists.nr`
* `examples/loops.nr`
* `examples/objects_and_lists.nr`
* `examples/dictionaries.nr`
* `examples/arena_showcase.nr`
* `examples/core_stress.nr`
* `examples/builtins.nr`
* `examples/file_io.nr`
* `examples/import_once_main.nr`
* `examples/import_path_normalize_main.nr`
* `examples/import_cycle_a.nr`
* `examples/import_cycle_b.nr`
* `examples/modules/main.nr`
* `examples/modules/main_nested.nr`
* `examples/modules/bad_main.nr`
* `examples/modules/missing_main.nr`

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

* Classes
* Constructors
* Instance fields
* Methods
* Bound `self`
* Lists
* Dictionaries
* List mutation
* Dictionary mutation
* Functions
* If / else logic
* For loops
* While loops
* Builtins
* Object state changes

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

* Classes
* Lists of objects
* Constructors
* Object state mutation
* Functions
* Conditional logic
* While loops
* For loops
* Runtime score tracking
* Multi-round battle simulation logic

This moves Nearoh beyond syntax demos into real executable projects.

---

# Recent Core Improvements

Recent cleanup, stabilization, and tooling work included:

* Cleaner `main.c` CLI architecture
* Improved parser structure and error handling
* AST cleanup and memory handling fixes
* Source-path annotation for AST nodes
* Value/list/dictionary system improvements
* Environment system cleanup
* Better runtime diagnostics with source file, line, and column
* Runtime import tracking
* Duplicate import skipping
* Circular import detection
* Import path normalization
* File-relative import resolution
* Missing import errors that show the attempted path
* Safer project-wide organization
* Fixed instance member container index assignment
* Fixed dictionary mutation persistence
* Fixed function-local assignment leaking into globals
* Fixed for-loop assignment inside functions leaking into globals
* Added expanded utility built-ins
* Added string/number conversion built-ins
* Added basic file I/O
* Added regression-style example programs
* Added expected-failure tests
* Added golden-output pass tests
* Added `run_tests.ps1`
* Added early native Win32/GDI IDE shell
* Added IDE editor line numbers, console line numbers, toolbar run buttons, console rendering, and click-to-place cursor navigation
* Replaced inconsistent object ownership with a unified GC-managed heap
* Added object headers and linked heap tracking
* Added heap-allocated lexical environments
* Fixed escaped closures
* Preserved shared mutable identity for managed objects
* Added mark, gray-stack trace, and sweep phases
* Added explicit runtime root handling
* Added threshold-based collection at safe points
* Added optional `NEAROH_GC_STRESS` mode
* Added cycle collection
* Added cycle-safe printing for recursive lists and dictionaries
* Added closure, identity, self-cycle, and GC stress regression tests
* Passed the entire regression suite under both GC stress and normal collection modes

Nearoh now has a stronger foundation for future growth.

---

# Why This Project Matters

Many hobby language projects stop at parsing expressions.

Nearoh already includes real runtime behavior:

* Executable programs
* User-defined functions
* Object-oriented systems
* Dynamic lists
* Dictionaries
* Scope handling
* Escaped lexical closures
* Shared mutable object identity
* Mark-and-sweep garbage collection
* Cycle handling
* Builtins
* File I/O
* Imports
* Multi-file execution
* Source-aware diagnostics
* Structured examples
* Command-line tooling
* Debugging modes
* Golden-output regression tests
* Expected-failure regression tests
* Early native IDE tooling

That means the project is moving into genuine language engineering territory.

Nearoh is still early, but it is already capable of running meaningful programs across multiple interacting systems.

---

# Roadmap

## Near-Term

* Continue improving diagnostics and error UX
* Add more runtime safety checks
* Improve built-in error behavior
* Add more standard library utilities
* Continue auditing native allocation accounting
* Expand temporary-root coverage before allowing collection inside arbitrary expression evaluation
* Begin designing module namespaces/module objects
* Expand regression tests
* Add more example programs
* Continue improving the native IDE shell

## Mid-Term

* Namespaced modules
* Expanded builtins
* Better performance paths
* More detailed GC diagnostics and heap statistics
* More complete standard library
* File and path utilities
* Cleaner automated test workflow
* IDE project navigation
* IDE error navigation
* IDE syntax highlighting
* Native graphics/window bridge planning

## Long-Term

* Native graphics/window bridge
* Input/timing systems
* Larger standard library built in Nearoh
* Bytecode VM or compiled backend research
* Dedicated Nearoh editor / IDE with project navigation, richer editing, and diagnostics
* Possible self-hosted growth path

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

The current stage is centered on strengthening the now-GC-backed runtime, expanding diagnostics and builtins, growing regression coverage, improving import/module architecture, and turning the native IDE from a working shell into a more complete language tool.

Every milestone is focused on turning Nearoh into a real, usable language rather than a superficial prototype.
