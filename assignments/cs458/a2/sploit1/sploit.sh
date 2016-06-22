#!/bin/bash -eu
BASE=$1

echo "Upvoting 'How to use submit' as user 'j3tracey'..."
curl "http://${BASE}/vote.php?userid=6&id=2&vote=1"
echo -e "\033[32mDone!\033[0m"
