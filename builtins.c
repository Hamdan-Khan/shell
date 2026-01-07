#include "shell.h"

void cmd_echo(char **args)
{
    int i = 1;
    char *token;
    while (token = args[i]){
        p("%s ",token);
        i++;
    }
    p("\n");
}

void cmd_exit(char **args)
{
    (void)args;

    p("Bye!\n");
    exit(EXIT_SUCCESS);
    return;
}

Builtin builtinCmds[] = {
    {.cmdName = "echo", .cmd = cmd_echo},
    {.cmdName = "exit", .cmd = cmd_exit},
    {.cmdName = NULL},
};
