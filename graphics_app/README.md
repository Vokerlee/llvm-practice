# Graphics application (Game of Life) and its trace

## What this folder contains

  * Game of Life is implemented in `GTK4` and `SDL` libraries.
  * Version with `SDL` implementation is used for trace analyses.
  * LLVM Logger Pass is implemented (valid for LLVM 16+ versions) for log of each LLVM IR operation for `Game of Life`` logic.
  * Scripts & Makefiles for trace collection from patched `Game of Life` with LLVM Logger Pass.
  * All analyses pictures are in `trace_analyses` folder.

## How to build

  * Build everything:
    * ```shell
      make
      ```
## How to launch

  * Launch `Game of Life`:
    * ```shell
      ./gameoflife_sdl/build/gameoflife
      ```
  * Launch patched `Game of Life` with LLVM Logger Pass:
    * ```shell
      ./gameoflife_trace/build/gameoflife_patched > filename.trace
      ```
  * Get LLVM IR operations frequency analyses (from patched `Game of Life`):
    * ```shell
      python3 ./gameoflife_trace/trace_analyses/gen_plots.py filename.trace
      ```
  * Launch `Game of Life` with manually generated logic in LLVM IR:
    * ```shell
      ./gameoflife_ir_gen/build/gameoflife_ir_gen
      ```

