#!/usr/bin/env bash
# ============================================================
# Script Name: fix_jlink_error.sh
# ------------------------------------------------------------
# Purpose:
#   Fix this error.
#   ```
#   Setting up jlink (9.16.1) ...
#   Updating udev rules via udevadm...
#   Failed to reload udevadm rules, retrying...
#   Failed to send reload request: Permission denied
#   Error: Failed to update udevadm rules.
#   dpkg: error processing package jlink (--configure):
#    installed jlink package post-installation script subprocess returned error exit status 1
#   Setting up libjsoncpp25:amd64 (1.9.5-6build1) ...
#   Setting up librhash0:amd64 (1.4.3-3build1) ...
#   Setting up cmake-data (3.28.3-1build7) ...
#   Setting up libarchive13t64:amd64 (3.7.2-2ubuntu0.6) ...
#   Setting up cmake (3.28.3-1build7) ...
#   Processing triggers for libc-bin (2.39-0ubuntu8.7) ...
#   Errors were encountered while processing:
#    jlink
#   E: Sub-process /usr/bin/dpkg returned an error code (1)
#   ```
# ============================================================

JLINK_POSTINST="/var/lib/dpkg/info/jlink.postinst"

# Clear the script that is trying to run udevadm
cat << EOF > "$JLINK_POSTINST"
#!/bin/sh
exit 0
EOF

# Make it executable
chmod +x "$JLINK_POSTINST"
