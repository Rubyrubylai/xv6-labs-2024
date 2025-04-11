#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void
find(char *path, char *filename)
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

	while(read(fd, &de, sizeof(de)) == sizeof(de)){
		strcpy(buf, path);
		p = buf+strlen(buf); // p 指到 buf 字串的結尾
		*p++ = '/'; // 在結尾加上 '/'，再把指標往後移
		
		if(de.inum == 0)
			continue;
		memmove(p, de.name, DIRSIZ); // 將 de.name 接在 buf 後面
		p[DIRSIZ] = 0; // 手動補上結尾字元（\0）
		if(stat(buf, &st) < 0){
			printf("find: cannot stat %s\n", buf);
			continue;
		}

		switch(st.type){
		case T_FILE:
			if (strcmp(de.name, filename) == 0){
				printf("%s/%s\n", path, filename);
			}
			break;

		case T_DIR:
			// 跳過同一層及上一層
			if ((strcmp(de.name, ".") != 0) && (strcmp(de.name, "..") != 0)) {
				find(buf, filename); // 傳入 buf，繼續遞迴找檔案
			}
			break;
		}
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
	if(argc != 3){
    printf("Please enter a dir and a filename!\n");
    exit(1);
  }

  char *path = argv[1];
	char *filename = argv[2];
  find(path, filename);

  exit(0);
}
