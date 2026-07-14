#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

// Some hints:
// Look at user/ls.c to see how to read directories.
// Use recursion to allow find to descend into sub-directories.
// Don't recurse into "." and "..".
// Changes to the file system persist across runs of qemu; to get a clean file system run make clean and then make qemu.
// You'll need to use C strings. Have a look at K&R (the C book), for example Section 5.5.
// Note that == does not compare strings like in Python. Use strcmp() instead.
// Add the program to UPROGS in Makefile.

char*
filename(char *path)
{
  char *p;

  for(p = path + strlen(path); p >= path && *p != '/'; p--) // 從最後往前找到第一個 '/'，即為檔名
    ;
  return p + 1;
}

void
find(char *path, char *target)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
    case T_DEVICE:
    case T_FILE:
      if (strcmp(filename(path), target) == 0){
        printf("%s\n", path);
      }
      break;

    case T_DIR:
      if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("find: path too long\n");
        break;
      }
      while(read(fd, &de, sizeof(de)) == sizeof(de)){
        strcpy(buf, path);
        p = buf+strlen(buf); // 目前路徑位址+大小
        *p++ = '/';

        if(de.inum == 0)
          continue;
        if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
          continue;

        memmove(p, de.name, DIRSIZ); // 把檔名接到後面
        p[DIRSIZ] = 0;
        
        find(buf, target);
      }
      break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc != 3){
    printf("Find arguments required");
    exit(1);
  }

  char* path = argv[1];
  char* target = argv[2];
  find(path, target);

  exit(0);
}
