#!/usr/bin/env bash

unameOut="$(uname -s)"
case "${unameOut}" in

    MINGW*)     conan install . -s build_type=Debug --install-folder=cmake-build-debug
                conan install . -s build_type=Release --install-folder=cmake-build-release;;
    Linux*)     conan install . -s build_type=Debug -s compiler.libcxx=libstdc++11 --install-folder=cmake-build-debug
                conan install . -s build_type=Release -s compiler.libcxx=libstdc++11 --install-folder=cmake-build-release;;
esac
