#!/usr/bin/env bash

SCRIPT_DIR=$(cd $(dirname $0); pwd)
PROJECTS_DIR="$SCRIPT_DIR/../projects"

cd "$PROJECTS_DIR" || {
    echo "Error: Unable to access $PROJECTS_DIR"
    exit 1
}

find . -type d -name "build*" -prune -o -name "*.[ch]" -print | xargs clang-format -style=file -i -verbose
