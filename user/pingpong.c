#include "kernel/types.h"
#include "user/user.h"

// Write a user-level program that uses xv6 system calls to ''ping-pong'' a byte between two processes 
// over a pair of pipes, one for each direction. The parent should send a byte to the child; the child 
// should print "<pid>: received ping", where <pid> is its process ID, write the byte on the pipe to the 
// parent, and exit; the parent should read the byte from the child, print "<pid>: received pong", and 
// exit. Your solution should be in the file user/pingpong.c.

int
main()
{
  int p1[2];
  int p2[2];
  pipe(p1); // parent write to child
  pipe(p2); // child write to parent

  int pid = fork();
  if (pid == 0) { // child
    close(p1[1]);
    char buf[1];

    read(p1[0], buf, 1);
    printf("child received: %s\n", buf);
    printf("%d: received ping\n", getpid());
    close(p1[0]);

    close(p2[0]);
    write(p2[1], buf, 1);
    close(p2[1]);
    
    exit(0);
  } else { // parent
    close(p1[0]);
    char *msg = "a";
    char buf[1];

    write(p1[1], msg, 1);
    close(p1[1]);
    

    close(p2[1]);
    read(p2[0], buf, 1);
    printf("parent received: %s\n", buf);
    printf("%d: received pong\n", getpid());
    close(p2[0]);

    wait(0);
    exit(0);
  }

  exit(1);
}
