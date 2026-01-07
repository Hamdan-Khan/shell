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

void cmd_cd(char **args)
{
    char *dir;
    int argsLen = getArrLength(args);
    if (argsLen != 2){
        p(RED"Invalid arguments\n"RST);
        return;
    };
    dir = args[1];
    chdir(dir);
}

Builtin builtinCmds[] = {
    {.cmdName = "echo", .cmd = cmd_echo},
    {.cmdName = "exit", .cmd = cmd_exit},
    {.cmdName = "cd", .cmd = cmd_cd},
    {.cmdName = NULL},
};
