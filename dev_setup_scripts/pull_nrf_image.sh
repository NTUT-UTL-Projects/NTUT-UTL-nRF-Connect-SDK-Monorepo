#!/usr/bin/env bash
# ============================================================
# nRF Connect SDK DevContainer (Non-Root) - Bash Version
# ------------------------------------------------------------
# Purpose:
#   This script replicates the VS Code Dev Container environment
#   for Nordic nRF Connect SDK development directly in a terminal.
# ============================================================

# ------------------------------
# Configuration
# ------------------------------
SCRIPT_DIR=$(cd $(dirname $0); pwd)
. "$SCRIPT_DIR/../.devcontainer/.env"

# ------------------------------
# Pull image from remote registry
# ------------------------------
echo "Pulling image '$IMAGE_NAME'..."
if podman pull "$IMAGE_NAME" --policy=newer; then
    echo "Image '$IMAGE_NAME' pulled successfully."
else
    echo "Failed to pull image '$IMAGE_NAME'."
    exit 1
fi
