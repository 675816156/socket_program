#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ERR() do { printf("%s:%d errno=%d [%s]\n", __func__, __LINE__, errno, strerror(errno)); exit(EXIT_FAILURE); } while(0)

#define BUF_SZ 128

typedef struct msgbuf {
	long mtype;       /* message type, must be > 0 */
	char mtext[BUF_SZ];    /* message data */
} msgbuf;

void usage(char *s)
{
	printf("%s : \n", s);
	printf("\t [process number]\n");
}
// create n process
int create_n_process(int n, pid_t **pids)
{
	int i = 0;

	*pids = (pid_t *)malloc(n * sizeof(pid_t));
	if (NULL == (*pids))
		ERR();

	for (i = 0; i < n; ++i) {
		(*pids)[i] = fork();
		if ((*pids)[i] < 0) {
			ERR();
		} else if ((*pids)[i] > 0) {
			/* parent */
			continue;
		} else {
			/* child */
			return 1;
		}
	}

	return 0;
}
// wait all child process exit
void wait_for_children(int n)
{
	int i = 0;
	for (i = 0; i < n; ++i) {
		waitpid(-1, NULL, 0);
	}
}
// ./a.out <how many process to create>
int main(int argc, char *argv[])
{
	int i = 0;
	int ret = 0;
	int msgq_id = 0;
	key_t ftok_key = 0;
	int process_num = 0;
	pid_t *pids = NULL;
	msgbuf msg_buf;

	if (argc < 2) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	process_num = atoi(argv[1]);
	if (process_num <= 0) {
		ERR();
	}

	memset(&msg_buf, 0, sizeof(msgbuf));
	ftok_key = ftok(".", 'j');
	if (ftok_key < 0) {
		ERR();
	}

	msgq_id = msgget(ftok_key, IPC_EXCL);
	if (msgq_id < 0) {
		msgq_id = msgget(ftok_key, IPC_CREAT | 0660);
		if (msgq_id < 0)
			ERR();
	}

	ret = create_n_process(process_num, &pids);
	if (ret) {
		/* child */
		msg_buf.mtype = 1;
		snprintf(msg_buf.mtext, BUF_SZ, "Hello, I am %d child!\n", getpid());
		ret = msgsnd(msgq_id, &msg_buf, BUF_SZ, 0);
		if (ret < 0) {
			ERR();
		}
	} else {
		/* parent */
		memset(&msg_buf, 0, sizeof(msgbuf));
		wait_for_children(process_num);
		for (i = 0; i < process_num; ++ i) {
			if (BUF_SZ != msgrcv(msgq_id, &msg_buf, BUF_SZ, 1, 0))
				ERR();
			printf("%s", msg_buf.mtext);
		}
	}

	/*msgctl(msgq_id, IPC_RMID, NULL);*/
	free(pids);
	return 0;
}
