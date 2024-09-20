#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[]) {
  char *args[MAXARG];
  int arg_index = 0;
  int n;
  char buf[512], c;
  int buf_index = 0;
  while ((n = read(0, &c, 1)) != 0) {
    if (c == ' ' && buf_index != 0) {
      args[arg_index] = (char*)malloc(buf_index);
      memcpy(args[arg_index], buf, buf_index);
      arg_index++;
      buf_index = 0;
      continue;
    }

    if (c == '\n') {
      if (buf_index != 0) {
        args[arg_index] = (char*)malloc(buf_index);
        memcpy(args[arg_index], buf, buf_index);
        arg_index++;
        buf_index = 0;
      }
      core(argc, argv, arg_index, args);
      arg_index = 0;
      continue;
    }

    buf[buf_index++] = c;
  }

  if (buf_index != 0) {
    args[arg_index] = (char*)malloc(buf_index);
    memcpy(args[arg_index], buf, buf_index);
    arg_index++;
  }

  if (arg_index != 0) {
    core(argc, argv, arg_index, args);
  }

  exit(0);
}

void
core(int argc, char *argv[], int additional_argc, char *additional_argv[]) {
  argc -= 1;
  int new_argc = argc + additional_argc;
  char *new_argv[new_argc];
  for (int i = 0; i < argc; i++) {
    new_argv[i] = (char*)malloc(sizeof(char) * strlen(argv[i + 1]));
    strcpy(new_argv[i], argv[i + 1]);
  }
  for (int i = 0; i < additional_argc; i++) {
    new_argv[argc + i] = (char*)malloc(sizeof(char) * strlen(additional_argv[i]));
    strcpy(new_argv[argc + i], additional_argv[i]);
  }
  
  int pid;
  pid = fork();
  if (pid == 0) {
    exec(new_argv[0], new_argv);
    exit(0);
  }
  wait(&pid);
}
