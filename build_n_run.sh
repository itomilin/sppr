#!/usr/bin/env bash

set -e

MODE=$1
if [[ -z $MODE ]]; then
    echo "[ ERROR ] First arg must be a mode (IAC|ORCH)."
    exit 1
fi

rm -Rf ./build
mkdir build && cd build

if [[ $MODE == "IAC" ]]; then
    cmake -DUSE_IAC=ON .. && make
elif [[ $MODE == "ORCH" ]]; then
    cmake -DUSE_ORCH=ON .. && make
elif [[ $MODE == "TEST" ]]; then
    cmake -DUSE_TEST=ON .. && make
else
    echo "[ ERROR ] Not handled mode."
    exit 1
fi

cp sppr ../
cd ..
./sppr

