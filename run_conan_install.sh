#!/usr/bin/env bash

conan install . -s build_type=Debug -s compiler.libcxx=libstdc++11 --install-folder=cmake-build-debug -o curl=True
conan install . -s build_type=Release -s compiler.libcxx=libstdc++11 --install-folder=cmake-build-release -o curl=True