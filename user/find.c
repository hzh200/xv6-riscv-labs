#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void core(const char* path, const char* filename);

int
main(int argc, char *argv[]) {
  if (argc != 3) {
      fprintf(2, "Usage: find path name.\n");
      exit(1);
  }

  core(argv[1], argv[2]);
  exit(0);
}

void
core(const char* path, const char* filename) {
  char buf[512], *p;

  if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
    printf("find: path too long\n");
    return;
  }

  int dir_fd;
  struct dirent de;
  struct stat st;

  if ((dir_fd = open(path, O_RDONLY)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';

  while((read(dir_fd, &de, sizeof(de))) == sizeof(de)) {
    if (de.inum == 0)
      continue;

    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if (strcmp(de.name, filename) == 0) {
      printf("%s\n", buf);
    }

    if (strcmp(".", de.name) == 0 || strcmp("..", de.name) == 0) {
      continue;
    }

    int fd;
    if ((fd = open(buf, O_RDONLY)) < 0) {
      fprintf(2, "%d %s\n", fd, buf);
      fprintf(2, "find: cannot open %s\n", buf);
      continue;;
    }

    if(fstat(fd, &st) < 0) {
      fprintf(2, "find: cannot stat %s\n", buf);
      close(fd);
      continue;
    }
    close(fd);

    if (st.type == T_DIR) {
      core(buf, filename);
    }
  }

  close(dir_fd);
  return;
}
