# 42sh

[![Build Status](https://travis-ci.com/42shell/42sh.svg?branch=master)](https://travis-ci.com/42shell/42sh)
[![codecov](https://codecov.io/gh/42shell/42sh/branch/master/graph/badge.svg)](https://codecov.io/gh/42shell/42sh)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/a3f04c20ba3845da9733c5b79a84e2f5)](https://www.codacy.com/gh/42shell/42sh?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=42shell/42sh&amp;utm_campaign=Badge_Grade)
[![Coverity Scan analysis](https://img.shields.io/coverity/scan/20272.svg)](https://scan.coverity.com/projects/20272)

A basic unix shell coded entirely from scratch with the following features:

* Multiple commands in one line with `;`
* All POSIX redirections: `>, <, >>, <<, |`
* Line editing using the termcap library
* Readline shortcuts `CTRL-U, CTRL-Y, CTRL-K, CTRL-W` to cut or paste parts of the line
* Bash-like tab completion
* Command history
* Full quote support: `", ', \`
