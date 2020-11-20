# 42sh

[![Build Status](https://travis-ci.com/42shell/42sh.svg?branch=master)](https://travis-ci.com/42shell/42sh)
[![codecov](https://codecov.io/gh/42shell/42sh/branch/master/graph/badge.svg)](https://codecov.io/gh/42shell/42sh)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/a3f04c20ba3845da9733c5b79a84e2f5)](https://www.codacy.com/gh/42shell/42sh?utm_source=github.com\&utm_medium=referral\&utm_content=42shell/42sh\&utm_campaign=Badge_Grade)
[![Coverity Scan analysis](https://img.shields.io/coverity/scan/20272.svg)](https://scan.coverity.com/projects/20272)

An advanced unix shell coded entirely from scratch with the following features:

*   **Line editing:**
    *   Made from scratch, we implemented our own readline using the termcap library
    *   Readline shortcuts CTRL-U, CTRL-Y, CTRL-K, CTRL-W
    *   Line history

*   **Simple commands execution:**
    *   ; and & control operators
    *   All POSIX basic input/output redirections: >, <, >>, <<, |
    *   Logical operators && and ||
    *   Useful builtins (cd, echo, exit, hash, type)

*   **Environment management and parameter expansion:**
    *   Special parameters
    *   Internal variables
    *   Full quote support: ", ', \\
    *   Field splitting
    *   Associated builtins (env, set, unset, export)

*   **Advanced expansion features:**
    *   Pattern matching (\* \[a!bc-e])
    *   Arithmetic expansion with $((...))

*   **Contextual autocompletion:**
    *   Command autocompletion (binaries and builtins)
    *   Command options autocompletion
    *   Variables, files and directories autocompletion

*   **More advanced command syntax and features:**
    *   Subshells, command grouping with (...) and {...}
    *   Command substitution with $(...)
    *   Process substitution with <(...) and >(...)

*   **Full job control support:**
    *   Signal handling
    *   Full tracking of all processes launched by the shell (IDs, statuses, exit codes)
    *   Basic builtins to interact with the processes (jobs, fg, bg)

*   **And more:**
    *   Non interactive mode, allowing you to write scripts in a file and give it as an argument to the program
    *   More than 400 automatic tests, using script and scriptlive to test interactive mode features like autocompletion and job control
    *   Thoroughly bug-tested using [AFL](https://github.com/google/AFL)
