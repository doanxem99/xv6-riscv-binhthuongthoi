#include "kernel/types.h"
#include "user/user.h"

int main() {
	int parent_fd[2], child_fd[2];
	pipe(parent_fd);
	pipe(child_fd);

	char signal;
	int is_child = (fork() == 0);
	if (is_child) {
		read(parent_fd[0], &signal, 1);
		printf("%d: received ping\n", getpid());
		write(child_fd[1], " ", 1);
	}
	else {
		write(parent_fd[1], " ", 1);
		read(child_fd[0], &signal, 1);
		printf("%d: received pong\n", getpid());
	}

	close(parent_fd[0]);
	close(parent_fd[1]);
	close(child_fd[0]);
	close(child_fd[1]);
	exit(0);
}
