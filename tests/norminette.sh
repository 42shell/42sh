#!/bin/bash

git clone https://github.com/42Paris/norminette /tmp/norminette
(cd /tmp/norminette; bundle)

if [ $(git rev-parse --abbrev-ref HEAD) == master ]; then
	FILES=$(git diff --name-only HEAD HEAD~ src include libft/src libft/inc)
else
	FILES=$(git diff --name-only master -- src include libft/src libft/inc)
fi
if [ $FILES -z ]; then
	exit 0
fi

! /tmp/norminette/norminette.rb "$FILES" | grep -B 1 "Error"
