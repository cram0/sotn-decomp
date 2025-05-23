#!/usr/bin/env python3

# tool to list functions by difficulty and decomp.me WIP links
import argparse
from pathlib import Path
import sys
from tabulate import tabulate
import os
import re
import concurrent.futures

from helpers import find_scratches

parser = argparse.ArgumentParser(
    description="Create list of undecompiled functions and sort them by difficulty"
)
parser.add_argument(
    "--use-call-trees",
    required=False,
    action="store_true",
    help="Use to add links to the 'function_calls' folder generated by analyze_calls.py",
)
parser.add_argument(
    "--no-fetch",
    required=False,
    action="store_true",
    help="Disable fetching scratch links from decomp.me",
)

parser.add_argument(
    "--keywords",
    metavar="keyword",
    type=str,
    nargs="+",
    help="Only match paths with these keywords",
)


# look in asm files, read in the text and check for branches and jump tables
def get_asm_files(asm_path):
    files = []
    for path in Path(asm_path).rglob("*.s"):
        # ignore data
        if ".data" in str(path) or ".bss" in str(path):
            continue
        f = open(f"{path}", "r")
        text = f.read()

        # check for different mips branch types and count
        branches = 0
        branch_types = [
            "b          ",
            "bczt       ",
            "bczf       ",
            "beq        ",
            "beqz       ",
            "bge        ",
            "bgeu       ",
            "bgez       ",
            "bgezal     ",
            "bgt        ",
            "bgtu       ",
            "bgtz       ",
            "ble        ",
            "bleu       ",
            "blez       ",
            "bgezal     ",
            "bltzal     ",
            "blt        ",
            "bltu       ",
            "bltz       ",
            "bne        ",
            "bnez       ",
            "j          ",
            "jal        ",
            "jalr       ",
            "jr         ",
        ]
        for branch in branch_types:
            branches = branches + text.count(branch)

        jump_table = "   "
        if "jpt_" in text or "jtbl_" in text:
            jump_table = "Yes"

        f = {"name": path, "text": text, "branches": branches, "jump_table": jump_table}

        files.append(f)

    return files


def find_wip(o):
    result = find_scratches(o[1], "psp")

    if result:
        return {"link": result[0], "percent": result[1]}

    return None


if __name__ == "__main__":
    args = parser.parse_args()
    asm_files = get_asm_files("asm/pspeu")

    # sort by name, then number of branches, then length
    asm_files = sorted(asm_files, key=lambda x: (x["name"]))
    asm_files = sorted(asm_files, key=lambda x: (x["branches"]))
    asm_files = sorted(asm_files, key=lambda x: len(x["text"].split("\n")))

    if args.keywords and len(args.keywords):
        # filter based on keywords
        asm_files = [
            file
            for file in asm_files
            if any(keyword in str(file["name"]) for keyword in args.keywords)
        ]

    output = []

    for f in asm_files:
        name = str(f["name"])
        length = len(f["text"].split("\n"))
        branches = f["branches"]
        jump_table = f["jump_table"]

        matches = re.search(r"\/(\w+)\/nonmatchings\/\w+\/(\w+)\.s", name)
        if matches:
            ovl_name = matches.group(1)
            func_name = matches.group(2)
        else:
            ovl_name = ""

            ovl_pattern = r"/([^/]+)_psp/"
            ovl_match = re.search(ovl_pattern, name)
            if ovl_match:
                ovl_name = ovl_match.group(1)

            name_pattern = r"/([^/]+)\.s"
            name_match = re.search(name_pattern, name)
            if name_match:
                func_name = name_match.group(1)

        wip = ""
        wip_percentage = ""
        output.append(
            [
                ovl_name,
                func_name,
                length,
                branches,
                jump_table,
                wip,
                wip_percentage,
            ]
        )

    if not args.no_fetch:
        # we are mostly waiting on IO so run in parallel
        with concurrent.futures.ThreadPoolExecutor() as executor:
            futures = [executor.submit(find_wip, o) for o in output]
            results = [f.result() for f in futures]

        # Update output with the results
        for i, o in enumerate(output):
            # keep the in-source results as definitive
            if results[i] != None:
                o[5] = results[i]["link"]
                o[6] = results[i]["percent"]

    if args.use_call_trees:
        base_url = (
            "https://raw.githubusercontent.com/Xeeynamo/sotn-decomp/gh-duplicates"
        )
        for i, o in enumerate(output):
            unique_name = ".".join([o[0], o[1]])

            svg_path = os.path.join("function_calls", f"{unique_name}.svg")
            if os.path.exists(svg_path):
                o[1] = f"[{o[1]}]({base_url}/{svg_path})"

    headers = ["Ovl", "Function", "Length", "Branches", "Jtbl", "WIP", "%"]
    print(tabulate(output, headers=headers, tablefmt="github"))
