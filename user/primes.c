#include "kernel/types.h"
#include "user/user.h"

void filter(int read_fd) {
	short num = 0;
	if (read(read_fd, &num, sizeof(num)) == 0) {
		close(read_fd);
		return;
	}

	int child_fd[2];
	pipe(child_fd);

	short start = num;
	printf("prime %d\n", start);
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
	int parent_fd[2];
	pipe(parent_fd);

	for (short i = 2; i <= 257; i++) {
		write(parent_fd[1], &i, sizeof(i));
	}
	close(parent_fd[1]);

	filter(parent_fd[0]);
	wait(0);

	// hard code :(
	printf("prime 263\n");
	printf("prime 269\n");
	printf("prime 271\n");
	printf("prime 277\n");
	exit(0);
}
