# Graphics application (Game of Life) and its trace

## What this folder contains

  * Game of Life is implemented in `GTK4` and `SDL` libraries.
  * Version with `SDL` implementation is used for trace analyses.
  * LLVM Logger Pass is implemented (valid for LLVM 16+ versions) for log of each LLVM IR operation for `Game of Life`` logic.
  * Scripts & Makefiles for trace collection from patched `Game of Life` with LLVM Logger Pass.
  * All analyses pictures are in `trace_analyses` folder.

## How to build & launch

  * Build `Game of Life` & launch:
    * ```shell
      mkdir -p ./gameoflife_sdl/build && cd ./gameoflife_sdl/build
      cmake ..
      make
      ./gameoflife
      ```
  * Build LLVM Logger Pass:
    * ```shell
      cd ./llvm_pass_logger
      make
      ```
  * Build patched `Game of Life` with LLVM Logger Pass & launch:
    * ```shell
      make
      ./build/gameoflife > filename.trace
      ```
  * Get LLVM IR operations frequency analyses:
    * ```shell
      python3 trace_analyses/gen_plots.py filename.trace
      ```
