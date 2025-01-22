// https://github.com/whileskies/xv6-labs-2020/blob/main/doc/Lab1-Xv6%20and%20Unix%20utilities.md
#include "kernel/types.h"
#include "user/user.h"

int main() {
	int p[2];
	pipe(p);

	char signal;
	int is_child = (fork() == 0);
	if (is_child) {
		read(p[0], &signal, 1);
		printf("%d: received ping\n", getpid());
		write(p[1], " ", 1);
		exit(0);
	}
	else {
		write(p[1], " ", 1);
		wait(0);
		read(p[0], &signal, 1);
		printf("%d: received pong\n", getpid());
	}

	close(p[0]);
	close(p[1]);
	exit(0);
}
