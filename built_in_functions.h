//
// Created by Hanyu Zhang on 2020-08-15.
//

#ifndef C_SHELL_BUILT_IN_FUNCTIONS_H
#define C_SHELL_BUILT_IN_FUNCTIONS_H
#endif

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int hzsh_help (char **args);
int hzsh_exit (char **args);
int hzsh_cd (char **args);
int hzsh_cat (char **args);

// List of builtin commands
char* builtin_command_names[] = {
        "cd",
        "exit",
        "help",
        "cat"
};

int (*builtin_func[]) (char**) = {
        &hzsh_cd,
        &hzsh_exit,
        &hzsh_help,
        &hzsh_cat
};

int builtin_command_count = sizeof(builtin_command_names) / sizeof(char*);

// Implementation of builtin functions
int hzsh_help (char **args) {
    printf("HEREEEEE\n");
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

int hzsh_cat (char** args) {
    int src; // source file
    int n; // tracking variable
    char buf[1];
    if (args[1] == NULL) {  // keep echoing
        char *buf;
        size_t buf_size = 64;
        buf = (char *)malloc(buf_size * sizeof(char));
        if (!buf) {
            fprintf(stderr, "hzsh: failed to allocate memory for cat buffer\n");
            exit(EXIT_FAILURE);
        }
        do {
            getline(&buf, &buf_size, stdin);
            printf("%s", buf);
        } while (1);
    } else {
        char buf[1];
        int src_file;

        src_file = open(args[1], O_RDONLY);
        if (src_file == -1) {
            fprintf(stderr, "hzsh: failed to open the file %s", args[1]);
            perror("");
        }

        while((n = read(src_file, buf, 1)) > 0) {
            write(stdout, buf, sizeof(buf));
        }
        close(src_file);
    }
    return 1;
}