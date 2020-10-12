#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

cd "$DIR/.."

if [ -z "$1" ]; then
	echo "Usage: tests/make_job_control_live_test.sh [TEST_NAME]"
	exit 1
fi


echo "Make sure your terminal has at least 80 columns!"
echo "Recording test \"$1\"..."

OLD_TERMWIDTH=$(tput cols)
stty cols 80 #force all tests to have 80 columns for consistency
script -T "$DIR/live_tests/job_control/$1.timing" --log-in "$DIR/live_tests/job_control/$1.stdin" -c "./42sh > $DIR/live_tests/job_control/$1.right 2>&1"

read -p "Keep test? [y/n] " -n 1 -r
echo
if [[ ! "$REPLY" =~ ^[Yy]$ ]]; then
	rm "$DIR/live_tests/job_control/$1.timing" "$DIR/live_tests/job_control/$1.stdin" "$DIR/live_tests/job_control/$1.right"
	stty cols "$OLD_TERMWIDTH"
	exit 1
fi

echo "Checking reproducibility with --maxdelay 0.02..."

scriptlive -T "$DIR/live_tests/job_control/$1.timing" --log-in "$DIR/live_tests/job_control/$1.stdin" --maxdelay 0.02 -c "./42sh > $DIR/live_tests/job_control/$1.log 2>&1"

diff "$DIR/live_tests/job_control/$1.log" "$DIR/live_tests/job_control/$1.right" > "$DIR/live_tests/job_control/$1.diff_log"
if [[ $? != "0" ]]; then
	echo "Not reproducible"
	cat "$DIR/live_tests/$1.diff_log"
	read -p "Keep diff? [y/n] " -n 1 -r
	if [[ ! "$REPLY" =~ ^[Yy]$ ]]; then
		rm "$DIR/live_tests/job_control/$1.diff_log"
	fi
else
	echo "Diff OK!" && rm "$DIR/live_tests/job_control/$1.diff_log"
fi
echo

read -p "Keep test? [y/n] " -n 1 -r
if [[ ! "$REPLY" =~ ^[Yy]$ ]]; then
	rm "$DIR/live_tests/job_control/$1.timing" "$DIR/live_tests/job_control/$1.stdin" "$DIR/live_tests/job_control/$1.right"
fi
echo

rm "$DIR/live_tests/job_control/$1.log"

stty cols "$OLD_TERMWIDTH"

