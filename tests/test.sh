#!/bin/bash

#this script needs GNU csplit and gnu "$SED" to work on macOS, run
#brew install coreutils
#brew install gnu-sed
#to install them

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

cd "$DIR/.."

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

export LC_ALL=C

mkdir -p glob
for a in {a..f}; do
	for b in {a..f}; do
		for c in {a..f}; do
			touch "glob/$a$b$c"
		done
	done
done

for file in "$DIR/bash_tests/"*.test
do
	test_name=$(basename "$file" | cut -d '.' -f 1)
	if [ $test_name = "stdin1" ]
	then
		./42sh < "$file" > "$DIR/$test_name.42sh" 2>&1;
		bash < "$file" > "$DIR/$test_name.bash" 2>&1;
	elif [ $test_name = "stdin2" ]
	then
		./42sh "$file" < "$file" > "$DIR/$test_name.42sh" 2>&1;
		bash "$file" < "$file" > "$DIR/$test_name.bash" 2>&1;
	else
		./42sh "$file" > "$DIR/$test_name.42sh" 2>&1;
		bash "$file" > "$DIR/$test_name.bash" 2>&1;
	fi
done

rm -rf tricky_dots
rm -rf testing_dotdots
rm -rf test_cdpath1 test_cdpath2
rm -rf glob

for file in "$DIR/fixed_tests/"*.test
do
	test_name=$(basename "$file" | cut -d '.' -f 1)
	./42sh "$file" > "$DIR/$test_name.42sh" 2>&1;
	cp "$DIR/fixed_tests/$test_name.right" "$DIR/$test_name.bash"
done

./42sh "$DIR/setenv.test" > "$DIR/setenv.42sh" 2>&1
tcsh "$DIR/setenv.test"> "$DIR/setenv.bash" 3>&1

"$SED" -i -E  's/.*:.*: (.*:)/42sh: \1/g' "$DIR/"*.bash
"$SED" -i -E "s/(.+)\/$/\1/g" "$DIR/cd.42sh" #remove / at the end of line for $PWD
"$SED" -i -E "s/norights: Permission denied/Could not open file/g" "$DIR/pipe.bash"
"$SED" -i -E "s/42sh: .*: Bad file descriptor/42sh: Bad file descriptor/g" "$DIR/redir.bash"
"$SED" -i -E "s/42sh: .*: No such file or directory/42sh: Could not open file/g" "$DIR/redir.bash"
"$SED" -i -E  's;\$PWD;'"${PWD}"';g' "$DIR/builtin_error.bash"


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
	sort -o "$DIR/42sh/$file" "$DIR/42sh/$file"
	sort -o "$DIR/bash/$file" "$DIR/bash/$file"
done

EXIT_ST=0
n=0
ok=0

for file in "$DIR/bash/"*
do
	file="$(basename "$file")"
	if diff -u -U 10 "$DIR/42sh/$file" "$DIR/bash/$file" 1>&2; then
		ok=$((ok + 1))
		printf "%-15s %s ✅\n" $file 
	else
		echo "$file" ❌ 1>&2
		EXIT_ST=1
	fi
	n=$((n + 1))
done

if ((EXIT_ST == 0)); then 
	emoji=✅
else
	emoji=❌
fi

printf "\n$ok/$n Succesful Tests %s\n" "$emoji" 1>&2

rm -rf "$DIR/"*42sh "$DIR/"*bash
exit $EXIT_ST
