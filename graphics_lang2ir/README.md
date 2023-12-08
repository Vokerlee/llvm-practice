## Language with support of graphics standard library

Language `grlang` is a language with built-in support of simple 2D-graphics API.

### Short description
  * Grammar of the language is available in the directory `grammar/grammar.y`
  * Language supports built-in calls of library functions (`stdlib` of the language):
    * `print expr;` as `print` call;
    * `?` -- as `scan`, integer (i32) value;
    * `void sgl_init(int width, int height)`, `sgl_close()`, `sgl_srand()` as statements;
    * `sgl_rand()` as integer (i32) value;

### How to build the project

```shell
mkdir build && cd build
cmake ..
make
./bin/grlang < [filename_path]
```

### How to generate output executable file

To compile the `grlang` program and link it with `stdlib` your program just use `bash scripts/ld.sh [filename_path] [build_dir] [output_path]`
  * Example: you are in `build` directory, use `bash ../scripts/ld.sh ../examples/gameoflife.g . .`, output file is `gameoflife.out`
    in `build` directory.
