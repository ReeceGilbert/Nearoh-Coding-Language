from pathlib import Path
import argparse

# File/folder names to skip
SKIP_DIRS = {
    ".git",
    ".idea",
    ".vscode",
    ".venv",
    "venv",
    "env",
    "cmake-build-debug",
    "cmake-build-release",
    "build",
    "__pycache__",
}

# Extensions to count.
# Add/remove anything here.
COUNT_EXTENSIONS = {
    ".c",
    ".h",
    ".nr",
    ".txt",
    ".md",
    ".cmake",
}

# Exact filenames to count even if they have no extension
COUNT_FILENAMES = {
    "CMakeLists.txt",
    "Makefile",
}


def should_skip(path: Path) -> bool:
    return any(part in SKIP_DIRS for part in path.parts)


def should_count(path: Path) -> bool:
    return (
            path.suffix in COUNT_EXTENSIONS
            or path.name in COUNT_FILENAMES
    )


def count_lines(path: Path) -> int:
    try:
        with path.open("r", encoding="utf-8", errors="replace") as file:
            return sum(1 for _ in file)
    except Exception as error:
        print(f"Could not read {path}: {error}")
        return 0


def main():
    parser = argparse.ArgumentParser(
        description="Count lines of code/files in a Nearoh project directory."
    )

    parser.add_argument(
        "directory",
        nargs="?",
        default=".",
        help="Directory to scan. Defaults to current directory."
    )

    args = parser.parse_args()
    root = Path(args.directory).resolve()

    if not root.exists():
        print(f"Directory does not exist: {root}")
        return

    if not root.is_dir():
        print(f"Path is not a directory: {root}")
        return

    files = []

    for path in root.rglob("*"):
        if path.is_file() and not should_skip(path) and should_count(path):
            line_count = count_lines(path)
            files.append((path, line_count))

    files.sort(key=lambda item: str(item[0]).lower())

    total = 0

    print(f"\nLine count for: {root}\n")
    print("-" * 80)

    for path, line_count in files:
        relative_path = path.relative_to(root)
        total += line_count
        print(f"{str(relative_path):60} {line_count:>8}")

    print("-" * 80)
    print(f"{'TOTAL':60} {total:>8}")
    print(f"{'FILES COUNTED':60} {len(files):>8}")


if __name__ == "__main__":
    main()