#!/bin/bash

git clone https://github.com/42Paris/norminette /tmp/norminette
(cd /tmp/norminette; bundle)
! /tmp/norminette/norminette.rb src include libft/src libft/inc | grep -B 1 "Error"
