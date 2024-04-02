# /// script
# requires-python = ">=3.8"
# dependencies = []
#   "jinja2",
# ]
# ///
import sys
from typing import Any, Sequence
import jinja2
import argparse


def format_each(iterable: Any, s: str) -> Any:
    return (s.format(x) for x in iterable)


_GENERATED_FILE_WARNING = """// d8888b.  .d88b.       d8b   db  .d88b.  d888888b      d88888b d8888b. d888888b d888888b
// 88  `8D .8P  Y8.      888o  88 .8P  Y8. `~~88~~'      88'     88  `8D   `88'   `~~88~~'
// 88   88 88    88      88V8o 88 88    88    88         88ooooo 88   88    88       88
// 88   88 88    88      88 V8o88 88    88    88         88~~~~~ 88   88    88       88
// 88  .8D `8b  d8'      88  V888 `8b  d8'    88         88.     88  .8D   .88.      88
// Y8888D'  `Y88P'       VP   V8P  `Y88P'     YP         Y88888P Y8888D' Y888888P    YP"""


def gen_warning(filename: str) -> str:
    return f"{_GENERATED_FILE_WARNING}\n\n// This file was GENERATED from {filename}"


def main(argv: Sequence[str]) -> int:
    parser = argparse.ArgumentParser(
        description="Renders a Jinja template from stdin to stdout"
    )
    parser.parse_args(argv)  # No arguments, but throws if arguments are provided
    template_str = sys.stdin.read()
    env = jinja2.Environment(
        line_statement_prefix="#%",
        line_comment_prefix="##",
    )
    env.filters["format_each"] = format_each  # type: ignore
    tmpl = env.from_string(template_str)
    s = tmpl.render(generated_file_notice=gen_warning)
    sys.stdout.write(s)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
