#!/bin/bash

git clone https://github.com/42Paris/norminette /tmp/norminette
(cd /tmp/norminette; bundle)

if [ -z "$TRAVIS_BRANCH" ]; then
	TRAVIS_BRANCH=$(git rev-parse --abbrev-ref HEAD)
fi

if [ "$TRAVIS_BRANCH" == master ]; then
	FILES=$(git diff --name-only HEAD HEAD~ src include libft/src libft/inc)
else
	git config remote.origin.fetch "+refs/heads/*:refs/remotes/origin/*"
	git fetch origin master
	FILES=$(git diff --name-only origin/master -- src include libft/src libft/inc)
fi

if [ -z "$FILES" ]; then
	echo "No .c or .h changed ✔️"
	exit 0
fi

! /tmp/norminette/norminette.rb $FILES | grep -B 1 "Error" && echo "No norm error ✔️"
