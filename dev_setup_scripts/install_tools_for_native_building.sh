#!/usr/bin/env bash
# ============================================================
# Script Name: install_tools_for_native_building.sh
# ------------------------------------------------------------
# Purpose:
#   Installs the necessary tools to build the cmake target.
#   This enables testing and simulation directly on the host machine.
# 
# Notes:
#   - These tools are not dependencies of the nRF Connect SDK;
#     therefore, use CMake in the terminal directly.
# ============================================================

apt update && apt install \
    build-essential \
    cmake \
    gcc \
    g++ \
    ninja-build \
    && rm -rf /var/lib/apt/lists/*
