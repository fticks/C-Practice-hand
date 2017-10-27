#! /bin/bash

#
# This file has several tests to check that the API
# works to an acceptable standard.
#

# 文件包含，注意点号(.)和文件名中间有一空格
# . funcs.sh 两种方式
source funcs.sh


# ---------------------------- Tests -----------------------------------


function test_api {
	echo "Testing API calls.."
	compile src/api.c
	#output=`./test` # ` 反引号
	output=$(./test) #两种表达式方式，`表达式`，或者$(表达式)
	# if [[ $? != 0 ]]; then #函数返回值在调用该函数后通过 $? 来获得
	if (( $? != 0 )); then # 两种方式 (( 或 [[
		 err "$output" "$output"
		 exit 1
	fi
}



# Run tests
test_api





echo "No API errors"
