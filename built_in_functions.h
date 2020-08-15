//
// Created by Hanyu Zhang on 2020-08-15.
//

#ifndef C_SHELL_BUILT_IN_FUNCTIONS_H
#define C_SHELL_BUILT_IN_FUNCTIONS_H
#endif

#include <unistd.h>
#include <stdio.h>

int hzsh_help (char **args);
int hzsh_exit (char **args);
int hzsh_cd (char **args);

// List of builtin commands
char* builtin_command_names[] = {
        "cd",
        "exit",
        "help"
};

int (*builtin_func[]) (char**) = {
        &hzsh_cd,
        &hzsh_exit,
        &hzsh_help
};

int builtin_command_count = sizeof(builtin_command_names) / sizeof(char*);

// Implementation of builtin functions
int hzsh_help (char **args) {
    printf("Helen Zhang's hzsh\n");
    printf("The following are available builtin commands:\n");

    for (int i = 0; i < builtin_command_count; i++) {
        printf("%s\n", builtin_command_names[i]);
    }

    printf("Use the man command for information on other commands\n");
    return 1;
}

int hzsh_exit (char **args) {
    return 0;
}

int hzsh_cd (char** args) {
    if (args[1] == NULL) {
        fprintf(stderr, "hzsh: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("hzsh");
        }
    }
    return 1;
}