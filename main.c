// Create by Helen Z in August 2020.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "built_in_functions.h"

void main_loop (void);
char *read_line (void);
char **split_line (char *line);
int launch_process(char **args);
int execute (char **args);


int main (int argc, char **argv) {
    // Load any config files.

    // Run the main command loop.
    // - interactive or from file
    main_loop();

    // Perform any necessary kill/cleanup jobs.
    // - execute shutdown command
    // - free memory
    // - terminate program

    return EXIT_SUCCESS;
}

void main_loop (void) {
    char *line;
    char **args;
    int status;

    do {
        printf("> ");
        line = read_line();
        args = split_line(line);
        status = execute(args);

        free(line);
        free(args);
    } while (status);
}

char *read_line(void) {
    char* line = NULL;
    ssize_t buf_size = 0;

    if (getline(&line, &buf_size, stdin) == -1){
        if (feof(stdin)) {
            exit(EXIT_SUCCESS);
        } else {
            perror("read_line");
            exit(EXIT_FAILURE);
        }
    }
    return line;
}

#define HZSH_TOKEN_BUFSIZE 128
#define HZSH_TOKEN_DELIMITERS " \n\t\f\r"
char **split_line (char *line) {
    int buf_size = HZSH_TOKEN_BUFSIZE;
    int pos = 0;
    char** tokens = malloc(sizeof(char*) * buf_size);
    char* token;

    if (!tokens) {
        fprintf(stderr, "hzsh: failed to allocate memory for tokens\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, HZSH_TOKEN_DELIMITERS);
    while (token != NULL) {
        tokens[pos] = token;
        pos++;

        if (pos >= buf_size) {
            buf_size += HZSH_TOKEN_BUFSIZE;
            tokens = realloc(tokens, sizeof(char*) * buf_size);
            if (!tokens) {
                fprintf(stderr, "hzsh: failed to reallocate memory for tokens\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, HZSH_TOKEN_DELIMITERS);  // continue to tokenize line
    }

    tokens[pos] = NULL;
    return tokens;
}

int launch_process(char** args) {
    pid_t pid;
    pid_t wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process here
        if (execvp(args[0], args) == -1) {
            perror("hzsh: failed to execvp command");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // error with fork()
        perror("hzsh: failed to fork process");
    } else {
        // Parent process here
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status)); // exited or killed by sig
    }

    return 1;
}

int execute (char**args) {
    if (args[0] == NULL) {
        // an empty command was entered
        return 1;
    }

    for (int i = 0; i < builtin_command_count; i++) {
        if (strcmp(args[0], builtin_command_names[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }
    return launch_process(args);
}



