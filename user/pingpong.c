#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
	int p2c[2];
	int c2p[2];
	pipe(p2c);
	pipe(c2p);

	int pid = fork();
	if (pid < 0) {
		printf("fork failed\n");
		exit(1);
	}

	if (pid == 0) {
		// child process
		char buf[1];

		read(p2c[0], buf, 1);
		printf("child received: %s\n", buf);
		printf("%d: received ping\n", getpid());
		close(p2c[0]);

		write(c2p[1], buf, 1);
		close(c2p[1]);

		exit(0);
	} else {
		// parent process
		char *msg = "a";
		char buf[1];

		write(p2c[1], msg, 1);
		close(p2c[1]);

		read(c2p[0], buf, 1);
		printf("parent received: %s\n", buf);
		printf("%d: received pong\n", getpid());
		close(c2p[0]);

		wait(0);
		exit(0);
	}
}
