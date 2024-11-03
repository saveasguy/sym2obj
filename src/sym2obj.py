#!/usr/bin/env python3

import argparse
import json
import os
from pathlib import Path
import subprocess
import tempfile

SYM2OBJ_DIR_ENV = "SYM2OBJ_DIR"


def sym2obj(build_cmd: list[str], output_file: str):
    env = os.environ.copy()
    sym2obj_dir = tempfile.mkdtemp()
    env[SYM2OBJ_DIR_ENV] = sym2obj_dir
    env["LD_PRELOAD"] = str(Path(__file__).parent.parent / "lib" / "libsym2obj.so")
    subprocess.run(build_cmd, env=env, check=True)
    obj2sym = dict()
    for raw in Path(sym2obj_dir).iterdir():
        object, symbols = json.loads(raw.read_text())
        obj2sym[object] = symbols
    sym2obj = {"objects": [], "symbols": dict()}
    for obj, symbols in obj2sym.items():
        sym2obj["objects"].append(obj)
        for sym in symbols:
            sym2obj["symbols"][sym] = len(sym2obj["objects"]) - 1
    with open(output_file, "w") as output:
        json.dump(sym2obj, output)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        "sym2obj",
        description="build command wrapper for collecting symbols to object files mapping.",
    )
    parser.add_argument(
        "-o",
        dest="output",
        default="sym2obj.json",
        help="destination file, where mappings will be stored.",
    )
    parser.add_argument(dest="build", nargs=argparse.REMAINDER, help="Command to run.")
    args = parser.parse_args()
    sym2obj(args.build, args.output)
