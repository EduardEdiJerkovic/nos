#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <time.h>

struct my_msgbuf {
    long mtype;
    char mtext[200];
};

int msgIdRead1, msgIdRead2, msgIdRead3;
int msgIdWrite1, msgIdWrite2, msgIdWrite3;

void retreat(int failure) 
{
    if (msgctl(msgIdRead1, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }
    exit(0);
}

int main(void)
{
    struct my_msgbuf bufRead, bufWrite;
    key_t keyRead1, keyRead2, keyRead3;
    key_t keyWrite1, keyWrite2, keyWrite3;
    char text[200];
    
    keyRead1 = 2001;
    keyRead1 = 2002;
    keyRead1 = 2003;

    keyWrite1 = 1001;
    keyWrite2 = 1002;
    keyWrite3 = 1003;
 
    if ((msgIdRead1 = msgget(keyRead1, 0600 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }

    if ((msgIdRead2 = msgget(keyRead2, 0600 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }

    if ((msgIdRead3 = msgget(keyRead3, 0600 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }

    if ((msgIdWrite1 = msgget(keyWrite1, 0600 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }

    if ((msgIdWrite2 = msgget(keyWrite2, 0600 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }
    
    if ((msgIdWrite3 = msgget(keyWrite3, 0600 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }

    sigset(SIGINT, retreat);

    srand(time(NULL));
    char stop[10];

    for(;;) {
        scanf("%s", stop);
        int flag = 0;
        int r = rand() % 3 + 1; 
        text[0] = r + '0';

        memcpy(bufWrite.mtext, text, strlen(text)+1);
        bufWrite.mtype = 1;

        printf("%c sent.\n", bufWrite.mtext[0]);
        
        if (msgsnd(msgIdWrite1, (struct msgbuf *)&bufWrite, strlen(text)+1, 0) == -1) {
            perror("msgsnd");
        }

        if (msgsnd(msgIdWrite2, (struct msgbuf *)&bufWrite, strlen(text)+1, 0) == -1) {
            perror("msgsnd");
        }

        if (msgsnd(msgIdWrite3, (struct msgbuf *)&bufWrite, strlen(text)+1, 0) == -1) {
            perror("msgsnd");
        }
        
        // First message recived.
        if (msgrcv(msgIdRead1, (struct msgbuf *)&bufRead, sizeof(bufRead)-sizeof(long), 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

        printf("First answer.\n");

        // Second message recived.
        if (msgrcv(msgIdRead2, (struct msgbuf *)&bufRead, sizeof(bufRead)-sizeof(long), 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

        printf("Second answer.\n");

        // Third message recived.
        if (msgrcv(msgIdRead3, (struct msgbuf *)&bufRead, sizeof(bufRead)-sizeof(long), 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

        printf("Third answer.\n");       
    }

    return 0;
}



