#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define ERR() do { printf("%s:%d errno=%d [%s]\n", __func__, __LINE__, errno, strerror(errno)); exit(EXIT_FAILURE); } while(0)

#define BUF_SZ 128

int main(int argc, char *argv[])
{
	pid_t pid = 0;
	key_t ftok_key = 0;
	int shm_id = 0;
	char *buf = NULL;

	ftok_key = ftok(".", 'B');
	if (ftok_key < 0)
		ERR();

	shm_id = shmget(ftok_key, BUF_SZ, IPC_EXCL);
	if (shm_id < 0) {
		shm_id = shmget(ftok_key, BUF_SZ, IPC_CREAT | 0660);
		if (shm_id < 0)
			ERR();
	}

	pid = fork();
	if (pid < 0) {
		ERR();
	} else if (pid > 0) {
		/* father */
		waitpid(-1, NULL, 0);
		buf = (char *)shmat(shm_id, (void *)NULL, 0);
		printf("%s", buf);
		if (shmdt(buf) < 0)
			ERR();
		if (shmctl(shm_id, IPC_RMID, NULL) < 0)
			ERR();
	} else {
		/* child */
		buf = (char *)shmat(shm_id, (void *)NULL, 0);
		snprintf(buf, BUF_SZ, "Hello, I am %d child!\n", getpid());
	}

	return 0;
}
