#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

cd "$DIR/.."

if [ -z "$1" ]; then
	echo "Usage: tests/make_live_tests.sh [TEST_NAME]"
	exit 1
fi


echo "Make sure your terminal has at least 80 columns!"
echo "Recording test \"$1\"..."

OLD_TERMWIDTH=$(tput cols)
stty cols 80 #force all tests to have 80 columns for consistency
script -T "$DIR/live_tests/$1.timing" --log-in "$DIR/live_tests/$1.stdin" -c ./42sh

read -p "Keep test? [y/n] " -n 1 -r
echo
if [[ ! "$REPLY" =~ ^[Yy]$ ]]; then
	rm "$DIR/live_tests/$1.timing" "$DIR/live_tests/$1.stdin"
	stty cols "$OLD_TERMWIDTH"
	exit 1
fi

echo "Recording reference output... (an empty line at the start is normal)"

scriptlive -T "$DIR/live_tests/$1.timing" --log-in "$DIR/live_tests/$1.stdin" -c ./42sh | tee "$DIR/live_tests/$1.right"

echo "Checking reproducibility with --maxdelay 0.02..."

scriptlive -T "$DIR/live_tests/$1.timing" --log-in "$DIR/live_tests/$1.stdin" --maxdelay 0.02 -c ./42sh | tee "$1.test_log"

diff "$1.test_log" "$DIR/live_tests/$1.right" > "$DIR/live_tests/$1.diff_log" && echo "Diff OK!" \
|| echo "Not reproducible" && cat "$DIR/live_tests/$1.diff_log" && read -p "Keep diff? [y/n] " -n 1 -r \
&& if [[ ! "$REPLY" =~ ^[Yy]$ ]]; then rm "$DIR/live_tests/$1.diff_log"; fi
echo

read -p "Keep test? [y/n] " -n 1 -r
if [[ ! "$REPLY" =~ ^[Yy]$ ]]; then
	rm "$DIR/live_tests/$1.timing" "$DIR/live_tests/$1.stdin" "$DIR/live_tests/$1.right"
fi
echo

#rm "$1.test_log"

stty cols "$OLD_TERMWIDTH"

