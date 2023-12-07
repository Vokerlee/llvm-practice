#!/bin/bash

grlang_stdlib_rel_path=lib/libgrlang_stdlib.a

src_file=$1
build_path=$2
output_path=$3

output_ll_file="${src_file##*/}".ll
output_asm_file="${src_file##*/}".s
output_obj_file="${src_file##*/}".o

./${build_path}/bin/grlang < ${src_file} > ${output_path}/${output_ll_file}

src_file="${src_file##*/}"
src_file=$(echo ${src_file} | cut -f -1 -d '.')
output_file="${src_file##*/}".out

llc ${output_path}/${output_ll_file} -o ${output_path}/${output_asm_file}
clang++ -c ${output_path}/${output_asm_file} -o ${output_path}/${output_obj_file}
clang++ ${output_path}/${output_obj_file} ${output_path}/${grlang_stdlib_rel_path} -lLLVM -o ${output_path}/${output_file}
