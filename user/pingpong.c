#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main() {
  int fds[2];
  pipe(fds);
  char buf[5];
  int pid;
  pid = fork();
  if (pid == 0) {
    read(fds[0], buf, 4);
    printf("%d: received %s\n", getpid(), buf);
    write(fds[1], "pong", 4);
    close(fds[0]);
    close(fds[1]);
  } else {
    write(fds[1], "ping", 4);
    wait(&pid);
    read(fds[0], buf, 4);
    printf("%d: received %s\n", getpid(), buf);
    close(fds[0]);
    close(fds[1]);
  }
  exit(0);
}
