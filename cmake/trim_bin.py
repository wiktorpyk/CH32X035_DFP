#!/usr/bin/env python3
"""Trim trailing 0x00 bytes from a binary file.

Usage: trim_bin.py input_file output_file
"""
import sys
from pathlib import Path

def main():
    if len(sys.argv) != 3:
        print("Usage: trim_bin.py INPUT OUTPUT")
        return 2
    inp = Path(sys.argv[1])
    out = Path(sys.argv[2])
    data = inp.read_bytes()
    trimmed = data.rstrip(b"\x00")
    out.write_bytes(trimmed)
    print(f"Wrote {out} ({len(trimmed)} bytes)")
    return 0

if __name__ == '__main__':
    raise SystemExit(main())
