#!/usr/bin/env bash
# ============================================================
# Script Name: install_tools_for_native_sim_building.sh
# ------------------------------------------------------------
# Purpose:
#   Install nRF Connect SDK dependencies for `native_sim` builds.
#   Used for running and testing Zephyr-based applications on the host.
# ============================================================

apt update && apt install \
    make \
    && rm -rf /var/lib/apt/lists/*
