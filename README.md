## Description of subprojects
All projects are related to different LLVM aspects:
  * `graphics_app` — application (`GameOfLife`) written via GTK4 & SDL libraries
      * Idea: implement LLVM pass for dynamic instruction statistics collection,
        create own ISA most preferable for collected trace via pass.
      * Detailed description see in the folder.
  *  `graphics_asm2ir` — create & implement own ISA, assembler & LLVM IR generator.
      * Idea: possibility to interpret assembler-file via LLVM IR generation.
      * `GameOfLife` was written in such assembler and interpreted (see `examples`).
  * `graphics_lang2ir` — implement language front-end of imperative language via `flex` & `bison`
        generating LLVM IR for the following LLVM IR compilation into binary file
        (+ linkage with own standard library).
      * Idea: possibility to compile file of own programming language into binary file with linkage with own standard library (primitive i/o, etc).
      * Detailed description see in the folder.
  * `graphics_llvm_backend` — implement own LLVM back-end of `RISC-V`-like ISA (64 bits)
        with graphics extension & clang intrinsics.
      * Build commands:
        * LLVM build:
            ```shell
            cmake -S llvm -B build -G Ninja -DLLVM_ENABLE_PROJECTS="clang" -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_ASSERTIONS=On -DLLVM_TARGETS_TO_BUILD=GRISCV -DLLVM_INCLUDE_TESTS=OFF
            cmake --build build
            ```
        * Compile example:
            ```shell
            ./build/bin/clang -c -O0 src.c -o src.o --target=griscv
            ./build/bin/clang -S -O0 src.c -o src.s --target=griscv
            ./build/bin/clang -S -emit-llvm -O0 src.c -o src.ll --target=griscv
            ```
  * `graphics_riscv_simulator` — RISC-V simulator with support of added graphics extensions.
    * Idea: possibility to execute built binary file via LLVM back-end with support of graphics instructions.
    * Usage example (`GameOfLife` example):
        * ```shell
          mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release && make
          ./rvsim ~/llvm-project-grisc-v/griscv_examples/gameoflife.o
          ```
