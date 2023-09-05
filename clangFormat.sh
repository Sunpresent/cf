#!/bin/bash
# Finds .hpp .h .c .cpp files and format them with clang-format
# Usage: [-p <dir>]
Dir="."

while getopts ":p:" opt; do
    case "${opt}" in
        p)
            Dir=${OPTARG}
            ;;
    esac
done

for i in $(find $Dir -regex '.*/.*\.\(c\|cpp\|h\|hpp\|tcc\)$')
do
    echo "Perfoming format on $i"
    clang-format -i --style="file" $i
done
