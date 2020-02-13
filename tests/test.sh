#!/bin/bash

#this script needs GNU csplit and gnu "$SED" to work on macOS, run
#brew install coreutils
#brew install gnu-sed
#to install them

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

if [[ "$OSTYPE" == "darwin"* ]]; then
	CSPLIT=gcsplit
	SED=gsed
else
	CSPLIT=csplit
	SED=sed
fi

export DIR_42SH=$DIR
export ls=ls
export arg=-a
export arg2=/
export "loop=bonjour\$loop"

mkdir testing_dotdots
mkdir -p tricky_dots/1/2/3/4/5/6/7
mkdir -p test_cdpath1/1 test_cdpath1/2 test_cdpath2/1 test_cdpath2/3
mkdir -p test_cdpath1/var
export CDPATH=$PWD/test_cdpath1:$PWD/test_cdpath2

for file in "$DIR/bash_tests/"*
do
	test_name=$(basename "$file")
	./42sh "$file" > "$DIR/$test_name.42sh" 2>&1
	bash "$file" > "$DIR/$test_name.bash" 2>&1
done

rm -rf tricky_dots
rm -rf testing_dotdots
rm -rf test_cdpath1 test_cdpath2

for file in "$DIR/fixed_tests/"*
do
	test_name=$(basename "$file")
	./42sh "$file" > "$DIR/$test_name.42sh" 2>&1
done

cat << EOF > "$DIR/parse_error.bash"
42sh: parse error near '&&'
~~~
42sh: parse error near '&&'
~~~
42sh: parse error near '>'
~~~
42sh: parse error near '>'
~~~
42sh: parse error near ';'
~~~
42sh: parse error near ';'
~~~
42sh: parse error near '<<'
~~~
42sh: parse error near '<<'
~~~
42sh: parse error near '>'
~~~
42sh: parse error near '>'
~~~
42sh: parse error near '&'
~~~
42sh: parse error near '|'
~~~
42sh: parse error near '|'
~~~
42sh: parse error near '|'
~~~
42sh: parse error near ';'
~~~
lol
~~~
42sh: parse error near '&&'
~~~
lol
~~~
42sh: parse error near '<'
EOF

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
42sh: cd: HOME not set
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

./42sh "$DIR/setenv" > "$DIR/setenv.42sh" 2>&1
tcsh "$DIR/setenv"> "$DIR/setenv.bash" 3>&1

"$SED" -i -E  's/.*:.*: (.*:)/42sh: \1/g' "$DIR/"*.bash
"$SED" -i -E "s/(.+)\/$/\1/g" "$DIR/cd.42sh" #remove / at the end of line for $PWD
"$SED" -i -E "s/command not found/cannot execute command/g" "$DIR/expansion.bash"
"$SED" -i -E "s/‘//g" "$DIR/env.bash"
"$SED" -i -E "s/’//g" "$DIR/env.bash"
"$SED" -i -E "s/norights: Permission denied/Could not open file/g" "$DIR/pipe.bash"
"$SED" -i -E "s/42sh: .*: Bad file descriptor/42sh: Bad file descriptor/g" "$DIR/redir.bash"
"$SED" -i -E "s/42sh: .*: No such file or directory/42sh: Could not open file/g" "$DIR/redir.bash"


mkdir -p "$DIR/bash" "$DIR/42sh"

for file in "$DIR/"*.42sh
do
	name="$(basename "$(echo "$file" | cut -d '.' -f 1)")"
	$CSPLIT --silent --suppress-matched "$file" '/~~~/' '{*}'\
		--prefix "$DIR/42sh/$name "
done
for file in "$DIR/"*.bash
do
	name="$(basename "$(echo "$file" | cut -d '.' -f 1)")"
	$CSPLIT --silent --suppress-matched "$file" '/~~~/' '{*}'\
		--prefix "$DIR/bash/$name "
done

"$SED" -n -E -i "/bonjour|kikoo|foo|jj/p" "$DIR/42sh/env 00" "$DIR/bash/env 00"
"$SED" -n -E -i "/kkk=lkjlkj/p" "$DIR/42sh/env 03" "$DIR/bash/env 03"
"$SED" -n -E -i "/kkkk|assa|lkjlkj|kkk/p" "$DIR/42sh/env 04" "$DIR/bash/env 04"
"$SED" -n -E -i "/kkkk|assa|lkjlkj|kkk/p" "$DIR/42sh/env 04" "$DIR/bash/env 04"

for file in "$DIR/bash/setenv"*
do
	file="$(basename "$file")"
	"$SED" -n -E -i "/test.*=/p" "$DIR/42sh/$file" "$DIR/bash/$file"
done

EXIT_ST=0
n=0
ok=0

for file in "$DIR/bash/"*
do
	file="$(basename "$file")"
	if diff -u -U 10 "$DIR/42sh/$file" "$DIR/bash/$file"; then
		ok=$(($ok + 1))
		printf "%-15s %s ✅\n" $file 
	else
		echo "$file" ❌
		EXIT_ST=1
	fi
	n=$(($n + 1))
done

if (($EXIT_ST == 0)); then 
	emoji=✅
else
	emoji=❌
fi

printf "\n$ok/$n Succesful Tests %s\n" "$emoji" 1>&2

rm -rf "$DIR/"*42sh "$DIR/"*bash
exit $EXIT_ST
