#!/usr/bin/env python
import argparse
import os
from Parser import CMakeCacheParser

def main():
    
    parser = argparse.ArgumentParser(description="Resolve dependencies in cmake project")
    parser.add_argument("file", help="Location of CMakeCache.txt file")
    args = parser.parse_args()

    # Resolve absolute path of input file
    filePath = os.path.abspath(args.file)

    # Parse the CMakeCache file
    parser = CMakeCacheParser.CMakeCacheParser(filePath)
    parser.parse()


if __name__ == "__main__":
    main()