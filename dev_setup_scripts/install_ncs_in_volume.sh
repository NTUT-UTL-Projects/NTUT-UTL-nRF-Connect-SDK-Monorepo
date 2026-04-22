#!/usr/bin/env bash
# ============================================================
# Script Name: install_ncs_in_volume.sh
# ------------------------------------------------------------
# Purpose:
#   Install the nRF Connect SDK into the mounted volume within
#   this container environment.
#
# Description:
#   This script initializes and updates the nRF Connect SDK
#   inside the specified working directory.
#
# Notes:
#   - This script must be run **inside the container**.
#   - Ensure that the target directory is correctly mounted.
#   - Requires network access to fetch SDK components via west.
#   - This script installs the full SDK and is therefore **not
#     suitable** for CI environments.
# ============================================================

# ------------------------------
# Configuration
# ------------------------------
SCRIPT_DIR=$(cd $(dirname $0); pwd)
. "$SCRIPT_DIR/../.devcontainer/.env"

# ------------------------------
# Check the path is existed
# ------------------------------
cd "$NCS_VOLUME_MOUNT_DIR" || {
    echo "Error: Unable to access $NCS_VOLUME_MOUNT_DIR"
    exit 1
}

# ------------------------------
# Set revision
# ------------------------------
URL=""
REVISION=""

usage() {
    echo "Usage: $0 [-m URL] [--mr REVISION]"
    echo "  -h, --help  get help for $0 or a command."
    echo "  -m          clones the repository at that URL and uses it as the manifest repository."
    echo "  --mr        manifest repository branch or tag name to check out."
    exit 1
}

# Parse arguments
while [[ $# -gt 0 ]]; do
    case "$1" in
        --mr)
            if [[ -n "$2" && "$2" != -* ]]; then
                REVISION="$2"
                shift 2
            else
                echo "Error: --mr requires an argument."
                usage
            fi
            ;;
        -h|--help)
            usage
            ;;
        -m)
            if [[ -n "$2" && "$2" != -* ]]; then
                URL="$2"
                shift 2
            else
                echo "Error: -m requires an argument."
                usage
            fi
            ;;
        *)
            echo "Unknown option: $1"
            usage
            ;;
    esac
done

# ------------------------------
# West install the nRF Connect SDK
# ------------------------------
echo "$REVISION"
echo "Initializing nRF Connect SDK in $NCS_VOLUME_MOUNT_DIR..."
west init ${URL:+-m "$URL"} ${REVISION:+--mr "$REVISION"}

echo "Updating SDK components..."
west update

echo "nRF Connect SDK installation completed successfully."
