#!/bin/sh

#
# Usage: sh script/format-coding-style.sh
#
script_path=`dirname $0`
root=`(cd $script_path/..; pwd)`

# Change this if your clang-format executable is somewhere else
clang_format="clang-format"

command_exists () {
  type "$1" &> /dev/null ;
}

if ! command_exists $clang_format ; then
    echo "You have to install clang-format before using this script"
    echo "  Ubuntu: sudo apt-get install clang-format"
    echo "  OSX   : brew update; brew install clang-format"
    exit 0
fi

echo "\nApply coding-style to this project: " $root

read -r -p "Are you sure? [Y/n] " answer
answer="$(echo ${answer} | tr 'A-Z' 'a-z')"

if echo "$answer" | grep -iq "^y" ;then
  echo "We are applying coding-style rules..."
else
  echo "Exit the script"
  exit 0
fi

find "$root" \( -name '*.h' -or -name '*.hpp' -or -name '*.c' -or -name '*.cpp' \) -prune -print0 | xargs -0 "$clang_format" -i

