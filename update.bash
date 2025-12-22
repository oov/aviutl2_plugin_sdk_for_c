#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SUBMODULE_PATH="aviutl2_plugin_sdk"

# 1. Check for updates without cloning
CURRENT_SHA=$(git ls-tree HEAD "$SUBMODULE_PATH" | awk '{print $3}')
URL=$(git config -f .gitmodules --get "submodule.$SUBMODULE_PATH.url")
REMOTE_SHA=$(git ls-remote "$URL" main | awk '{print $1}')

if [ "$CURRENT_SHA" = "$REMOTE_SHA" ]; then
    echo "No updates available in submodule ($CURRENT_SHA)."
    exit 0
fi

echo "Updates detected: $CURRENT_SHA -> $REMOTE_SHA"

# 2. Fetch submodule only when needed
echo "Fetching submodule with depth 1..."
git submodule update --init --recursive --depth=1

echo "Launching AI to process updates..."

pushd "$SCRIPT_DIR" > /dev/null
npx -y @github/copilot --allow-all-paths --allow-all-tools --model claude-opus-4.5 -p "$(cat UPDATE-PROMPT.md)"
popd > /dev/null
