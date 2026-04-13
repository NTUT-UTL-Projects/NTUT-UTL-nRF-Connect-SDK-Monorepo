#!/usr/bin/env bash
# ============================================================
# nRF Connect SDK DevContainer (Non-Root) - Bash Version
# ------------------------------------------------------------
# Purpose:
#   This script replicates the VS Code Dev Container environment
#   for Nordic nRF Connect SDK development directly in a terminal.
#
# Usage:
#   chmod +x run_nrf_devcontainer.sh
#   ./run_nrf_devcontainer.sh
# ============================================================

# ------------------------------
# Configuration
# ------------------------------
SCRIPT_DIR=$(cd $(dirname $0); pwd)
. "$SCRIPT_DIR/config.sh"

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

# ------------------------------
# Run container
# ------------------------------
echo "Starting nRF Connect SDK DevContainer..."

podman run -it --rm \
  --name "$CONTAINER_NAME" \
  -v /dev:/dev \
  --privileged \
  -e PATH="$PATH:/opt/SEGGER/JLink:/home/$USER/.local/bin:/opt/ncs/toolchain/bin:/usr/local/sbin:/usr/sbin:/sbin" \
  -e ACCEPT_JLINK_LICENSE=1 \
  --mount type=volume,src="$CCACHE_VOLUME",dst=$CCACHE_CONTAINER \
  --mount type=volume,src="$NCS_VOLUME",dst=$NCS_CONTAINER \
  --mount type=volume,src="$SEGGER_VOLUME",dst=$SEGGER_CONTAINER \
  --mount type=bind,src="$WORKSPACE_HOST",dst="$WORKSPACE_CONTAINER" \
  -w "$WORKSPACE_CONTAINER" \
  "$IMAGE_NAME" \
  bash

# ------------------------------
# End
# ------------------------------
echo "Container exited. All done."
