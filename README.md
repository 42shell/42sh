# 21sh

[![Build Status](https://travis-ci.com/fratajczak/21sh.svg?branch=master)](https://travis-ci.com/fratajczak/21sh)
[![codecov](https://codecov.io/gh/fratajczak/21sh/branch/master/graph/badge.svg)](https://codecov.io/gh/fratajczak/21sh)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/95c3d9f8e10f40b39c91767c800b9b2a)](https://www.codacy.com/manual/fratajczak/21sh?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=fratajczak/21sh&amp;utm_campaign=Badge_Grade)
[![Coverity Scan analysis](https://img.shields.io/coverity/scan/20272.svg)](https://scan.coverity.com/projects/20272)

A basic unix shell coded entirely from scratch with the following features:

* Multiple commands in one line with `;`
* All POSIX redirections: `>, <, >>, <<, |`
* Line editing using the termcap library
* Readline shortcuts `CTRL-U, CTRL-Y, CTRL-K, CTRL-W` to cut or paste parts of the line
* Bash-like tab completion
* Command history
* Full quote support: `", ', \`
