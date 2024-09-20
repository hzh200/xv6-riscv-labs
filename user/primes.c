#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void core(int source);

int
main() {
  int fds[2];
  pipe(fds);
  int pid;
  pid = fork();
  if (pid == 0) {
    close(fds[1]);
    core(fds[0]);
    close(fds[0]);
  } else {
    close(fds[0]);
    for (int i = 2; i <= 35; i++) {
      write(fds[1], &i, 4);
    }
    close(fds[1]);
    wait(&pid);
  }
  exit(0);
}

void
core(int in) {
  int num, n, first_flag = 0, target, child_flag = 0, child_id = -1, out = -1;
  while ((n = read(in, &num, 4)) != 0) {
    if (first_flag == 0) {
      target = num;
      printf("prime %d\n", num);
      first_flag = 1;
    } else {
      if (num % target != 0) {
        if (child_flag == 0) {
          int fds[2];
          pipe(fds);
          child_id = fork();
          if (child_id == 0) {
            close(fds[1]);
            core(fds[0]);
          } else {
            close(fds[0]);
            out = fds[1];
          }
          child_flag = 1;
        }
        write(out, &num, 4);
      }
    }
  }

  close(in);
  if (out != -1) {
    close(out);
  }
  
  if (child_flag != -1) {
    wait(&child_id);
  }

  exit(0);
}
