#include <stdio.h>

void op_logger(char *op_name)
{
    printf("%s\n", op_name);
}

void call_logger(char *called_func_name)
{
    printf("call %s\n", called_func_name);
}
