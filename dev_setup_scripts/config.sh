#!/usr/bin/env bash
# Shared variables for nRF Connect SDK DevContainer scripts

CONTAINER_NAME="nrf-connect-dev"
IMAGE_NAME="ghcr.io/nrfconnect/sdk-nrf-toolchain:latest"

CCACHE_VOLUME="ccache"
CCACHE_CONTAINER="/home/$USER/.cache/ccache"

NCS_VOLUME="sdk-nrf"
NCS_CONTAINER="/work/ncs"

SEGGER_VOLUME="SEGGER"
SEGGER_CONTAINER="/opt/SEGGER"

WORKSPACE_HOST="$(pwd)"                   # Current project folder
WORKSPACE_CONTAINER="/work/ncs/app"       # Workspace inside container
