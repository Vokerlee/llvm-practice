#!/bin/bash

grlang_stdlib_rel_path=lib/libgrlang_stdlib.a

src_file=$1
build_path=$2
output_path=$3

src_file_noext="${src_file##*/}"
src_file_noext=$(echo ${src_file_noext} | cut -f -1 -d '.')

output_ll_file=${src_file_noext}.ll
./${build_path}/bin/grlang < ${src_file} > ${output_path}/${output_ll_file}

output_asm_file=${src_file_noext}.s
llc ${output_path}/${output_ll_file} -o ${output_path}/${output_asm_file}

output_obj_file=${src_file_noext}.o
clang++ -c ${output_path}/${output_asm_file} -o ${output_path}/${output_obj_file}

output_file=${src_file_noext}.out
clang++ ${output_path}/${output_obj_file} ${output_path}/${grlang_stdlib_rel_path} -lLLVM -lSDL2 -o ${output_path}/${output_file}
