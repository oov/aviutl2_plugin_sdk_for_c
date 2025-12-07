#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SUBMODULE_PATH="aviutl2_plugin_sdk"

pushd "$SCRIPT_DIR/$SUBMODULE_PATH" > /dev/null
git fetch origin
LOCAL_HEAD=$(git rev-parse HEAD)
REMOTE_HEAD=$(git rev-parse origin/main)
popd > /dev/null

if [ "$LOCAL_HEAD" = "$REMOTE_HEAD" ]; then
    echo "No updates available in submodule."
    exit 0
fi

echo "Updates detected in submodule."
echo "  Local:  $LOCAL_HEAD"
echo "  Remote: $REMOTE_HEAD"
echo ""
echo "Launching AI to process updates..."

pushd "$SCRIPT_DIR" > /dev/null
copilot --allow-all-tools --model claude-opus-4.5 -p "$(cat UPDATE-PROMPT.md)"
popd > /dev/null
