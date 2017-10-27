#! /bin/bash

#
# This file should be included by other shell scripts that
# want to use any of the functions
#

function assure_installed_valgrind {
    valgrind --version &> /dev/null
    if (( $? != 0 )); then
		msg="Valgrind seems to not be installed."
		err "$msg" "$msg"
	fi
}


function needle { #needle #haystack
	python -c "import re; print(re.search(r'$1', '$2').group(0))"
	if (( $? != 0 )); then
		msg="Python script error"
		err "$msg" "$msg"
	fi
}


function extract_num { #needle with number #haystack
	string=$(needle "$1" "$2")
	needle "[0-9]*" "$string"
	if (( $? != 0 )); then
		msg="Python script error"
		err "$msg" "$msg"
	fi
}


function time_exec { #command ..
	# 将stdout和stderr合并后重定向到/dev/null
	# 如果希望执行某个命令，但又不希望在屏幕上显示输出结果，那么可以将输出重定向到 /dev/null
	realsecs=$(/usr/bin/time -f '%e' "$@" 2>&1 > /dev/null)
	echo "$realsecs"
}


function err { #string #log
	echo "------------------- ERROR ------------------------"
	# echo $1 
	echo "$1" #将$1参数转换为字符串，每次都进行换行，不会造成上下两条信息重叠
	echo "$2" >> error.log
	exit 1
}

# 在Shell中，调用函数时可以向其传递参数。
# 在函数体内部，通过 $n 的形式来获取参数的值，例如，$1表示第一个参数，$2表示第二个参数
function compile { #cfilepath
	gcc $COMPILATION_FLAGS "$1" ../thpool.c -D THPOOL_DEBUG -pthread -o test
}
