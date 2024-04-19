#!/usr/bin/env bash

cmake -H. -Btarget/debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=YES && make -Ctarget/debug && ./target/debug/cppdiff
