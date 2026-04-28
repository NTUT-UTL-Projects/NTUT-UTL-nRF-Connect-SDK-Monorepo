#!/usr/bin/env bash

apt update && apt install \
    clang-format \
    && rm -rf /var/lib/apt/lists/*
