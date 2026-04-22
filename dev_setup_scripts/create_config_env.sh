#!/usr/bin/env bash
# ============================================================
# Script Name: create_config_env.sh
# ------------------------------------------------------------
# Purpose:
#   Create the `.devcontainer/.env` file so that it can be used
#   in other applications.
# ============================================================

# ------------------------------
# Configuration
# ------------------------------
SCRIPT_DIR=$(cd $(dirname $0); pwd)
DEVCONTAINER_DIR="$SCRIPT_DIR/../.devcontainer"
ENV_FILE="$DEVCONTAINER_DIR/.env"

# ------------------------------
# Check the path is existed
# ------------------------------
[ -d "$DEVCONTAINER_DIR" ] || {
    echo "Error: Unable to access $DEVCONTAINER_DIR"
    exit 1
}

# ------------------------------
# Set parameters
# ------------------------------
IMAGE_TAG=""

usage() {
    echo "Usage: $0 -t <IMAGE_TAG>"
    echo "  -h, --help  Get help for $0 or a command."
    echo "  -t          The image tag for container version."
    exit 1
}

# Parse arguments
while [[ $# -gt 0 ]]; do
    case "$1" in
        -h|--help)
            usage
            ;;
        -t)
            if [[ -n "$2" && "$2" != -* ]]; then
                IMAGE_TAG="$2"
                shift 2
            else
                echo "Error: -t requires an argument."
                usage
            fi
            ;;
        *)
            echo "Unknown option: $1"
            usage
            ;;
    esac
done

if [ -z "$IMAGE_TAG" ]; then
    echo "IMAGE_TAG is required."
    usage
    exit 1
fi

# Init const parameters
NCS_VOLUME_MOUNT_DIR="/work/ncs"
SEGGER_VOLUME_MOUNT_DIR="/opt/SEGGER"
SEGGER_JLINK_VOLUME_MOUNT_DIR="$SEGGER_VOLUME_MOUNT_DIR/JLink"

# ------------------------------
# Write `config.env`
# ------------------------------
cat << EOF > $ENV_FILE
# Shared variables for nRF Connect SDK development.

CCACHE_VOLUME="ccache"
CCACHE_VOLUME_MOUNT_DIR="/home/$USER/.cache/ccache"

CONTAINER_NAME="$(basename "$PWD")-dev-$IMAGE_TAG"

IMAGE_NAME="ghcr.io/nrfconnect/sdk-nrf-toolchain:$IMAGE_TAG"

NCS_VOLUME="sdk-nrf-$IMAGE_TAG"
NCS_VOLUME_MOUNT_DIR="$NCS_VOLUME_MOUNT_DIR"

SEGGER_VOLUME="SEGGER"
SEGGER_VOLUME_MOUNT_DIR="/opt/SEGGER"
SEGGER_JLINK_VOLUME_MOUNT_DIR="$SEGGER_JLINK_VOLUME_MOUNT_DIR"

WORKSPACE_MOUNT_DIR="$NCS_VOLUME_MOUNT_DIR/app"

EOF
