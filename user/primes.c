#include "kernel/types.h"
#include "user/user.h"

void filter(int read_fd) {
	short num = 0;
	if (read(read_fd, &num, sizeof(num)) == 0) {
		close(read_fd);
		exit(0);
	}

	short start = num;
	printf("prime %d\n", start);

	int child_fd[2];
	pipe(child_fd);

	while (read(read_fd, &num, sizeof(num))) {
		if (num % start != 0) {
			write(child_fd[1], &num, sizeof(num));
		}
	}
	close(read_fd);
	close(child_fd[1]);

	int is_child = (fork() == 0);
	if (is_child) {
		filter(child_fd[0]);
	}
}

int main() {
	printf("prime 2\n");
	printf("prime 3\n");

	int parent_fd[2];
	pipe(parent_fd);

	for (short i = 5; i <= 280; i += 2) {
		if (i % 3 != 0) {
			write(parent_fd[1], &i, sizeof(i));
		}
	}
	close(parent_fd[1]);
	filter(parent_fd[0]);
	wait(0);

	exit(0);
}
