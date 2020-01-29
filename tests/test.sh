#!/bin/bash

#this script needs GNU csplit and gnu "$SED" to work on macOS, run
#brew install coreutils
#brew install gnu-sed
#to install it

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

if [[ "$OSTYPE" == "darwin"* ]]; then
	CSPLIT=gcsplit
	SED=gsed
else
	CSPLIT=csplit
	SED=sed
fi

export DIR_21SH=$PWD
export ls=ls
export arg=-a
export arg2=/
export "loop=bonjour\$loop"
./21sh "$DIR/expansion" > "$DIR/exp.21sh" 2>&1
bash "$DIR/expansion" > "$DIR/exp.bash" 2>&1


mkdir testing_dotdots
mkdir -p tricky_dots/1/2/3/4/5/6/7
mkdir -p test_cdpath1/1 test_cdpath1/2 test_cdpath2/1 test_cdpath2/3
mkdir -p test_cdpath1/var
export CDPATH=$PWD/test_cdpath1:$PWD/test_cdpath2

./21sh "$DIR/cd" > "$DIR/cd.21sh" 2>&1
bash "$DIR/cd" > "$DIR/cd.bash" 2>&1

rm -rf tricky_dots
rm -rf testing_dotdots
rm -rf test_cdpath1 test_cdpath2


./21sh "$DIR/env" > "$DIR/env.21sh" 2>&1
bash "$DIR/env" > "$DIR/env.bash" 2>&1


./21sh "$DIR/setenv" > "$DIR/setenv.21sh" 2>&1
tcsh "$DIR/setenv"> "$DIR/setenv.bash" 3>&1


./21sh "$DIR/pipe" > "$DIR/pipe.21sh" 2>&1
bash "$DIR/pipe" > "$DIR/pipe.bash" 2>&1

./21sh "$DIR/redir" > "$DIR/redir.21sh" 2>&1
bash "$DIR/redir" > "$DIR/redir.bash" 2>&1

./21sh "$DIR/quotes" > "$DIR/quotes.21sh" 2>&1
bash "$DIR/quotes" > "$DIR/quotes.bash" 2>&1

./21sh "$DIR/heredoc" > "$DIR/heredoc.21sh" 2>&1
bash "$DIR/heredoc" > "$DIR/heredoc.bash" 2>&1

./21sh "$DIR/and_or" > "$DIR/and_or.21sh" 2>&1
bash "$DIR/and_or" > "$DIR/and_or.bash" 2>&1

./21sh "$DIR/parse_error" > "$DIR/parse_error.21sh" 2>&1
cat << EOF > "$DIR/parse_error.bash"
21sh: parse error near '&&'
21sh: parse error near '&&'
21sh: parse error near '>'
21sh: parse error near '>'
21sh: parse error near ';'
21sh: parse error near ';'
21sh: parse error near '<<'
21sh: parse error near '<<'
21sh: parse error near '>'
21sh: parse error near '>'
21sh: parse error near '&'
21sh: parse error near '|'
21sh: parse error near '|'
21sh: parse error near '|'
21sh: parse error near ';'
lol
21sh: parse error near '&&'
lol
EOF

./21sh "$DIR/builtin_error" > "$DIR/builtin_error.21sh" 2>&1
cat << EOF > "$DIR/builtin_error.bash"
env: ./lol: No such file or directory
~~~
env: illegal option -- q
~~~
~~~
~~~
setenv: Too many arguments.
~~~
setenv: Invalid variable name
~~~
setenv: Invalid variable name
~~~
~~~
$PWD
~~~
cd: error
~~~
~~~
21sh: cd: HOME not set
~~~
cd: error
cd: error
cd: PWD not set
getcwd: could not get current dir
cd: PWD not set
getcwd: could not get current dir
~~~
cd: OLDPWD not set
EOF


"$SED" -i -E  's/.*:.*: (.*:)/21sh: \1/g' "$DIR/"*.bash
"$SED" -i -E "s/(.+)\/$/\1/g" "$DIR/cd.21sh" #remove / at the end of line for $PWD
"$SED" -i -E "s/21sh:(.*): command not found/env:\1: No such file or directory/g" "$DIR/env.21sh"
"$SED" -i -E "s/‘//g" "$DIR/env.bash"
"$SED" -i -E "s/’//g" "$DIR/env.bash"
"$SED" -i -E "s/norights: Permission denied/Could not open file/g" "$DIR/pipe.bash"
"$SED" -i -E "s/21sh: .*: Bad file descriptor/21sh: Bad file descriptor/g" "$DIR/redir.bash"
"$SED" -i -E "s/21sh: .*: No such file or directory/21sh: Could not open file/g" "$DIR/redir.bash"


mkdir -p "$DIR/bash" "$DIR/21sh"

for file in "$DIR/"*.21sh
do
	name="$(basename "$(echo "$file" | cut -d '.' -f 1)")"
	$CSPLIT --silent --suppress-matched "$file" '/~~~/' '{*}'\
		--prefix "$DIR/21sh/$name "
done
for file in "$DIR/"*.bash
do
	name="$(basename "$(echo "$file" | cut -d '.' -f 1)")"
	$CSPLIT --silent --suppress-matched "$file" '/~~~/' '{*}'\
		--prefix "$DIR/bash/$name "
done

"$SED" -n -E -i "/bonjour|kikoo|foo|jj/p" "$DIR/21sh/env 00" "$DIR/bash/env 00"
"$SED" -n -E -i "/kkk=lkjlkj/p" "$DIR/21sh/env 03" "$DIR/bash/env 03"
"$SED" -n -E -i "/kkkk|assa|lkjlkj|kkk/p" "$DIR/21sh/env 04" "$DIR/bash/env 04"
"$SED" -n -E -i "/kkkk|assa|lkjlkj|kkk/p" "$DIR/21sh/env 04" "$DIR/bash/env 04"
for file in "$DIR/bash/setenv"*
do
	file="$(basename "$file")"
	"$SED" -n -E -i "/test.*=/p" "$DIR/21sh/$file" "$DIR/bash/$file"
done

EXIT_ST=0
for file in "$DIR/bash/"*
do
	file="$(basename "$file")"
	if diff -u -U 10 "$DIR/21sh/$file" "$DIR/bash/$file"; then
		echo "$file" ✅
	else
		echo "$file" ❌
		EXIT_ST=1
	fi
done

rm -rf "$DIR/"*21sh "$DIR/"*bash
exit $EXIT_ST
