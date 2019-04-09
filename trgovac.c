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

int msgIdRead;
int msgIdWrite1, msgIdWrite2, msgIdWrite3;

void retreat(int failure) 
{
    if (msgctl(msgIdRead, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }

    exit(0);
}

int main(void)
{
    struct my_msgbuf bufRead, bufWrite;
    key_t keyRead;
    key_t keyWrite1, keyWrite2, keyWrite3;
    char text[200];
    char step[10];

    srand(time(NULL));
    
    keyRead = 2000;

    keyWrite1 = 1001;
    keyWrite2 = 1002;
    keyWrite3 = 1003;
 
    // Creating queeue for reciving messages.
    if ((msgIdRead = msgget(keyRead, 0600 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }

    // Creating queeue for sending messages to first process.
    if ((msgIdWrite1 = msgget(keyWrite1, 0600 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }

    // Creating queeue for sending messages to second process.
    if ((msgIdWrite2 = msgget(keyWrite2, 0600 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }
    
    // Creating queeue for sending messages to third process.
    if ((msgIdWrite3 = msgget(keyWrite3, 0600 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }

    sigset(SIGINT, retreat);

    for(;;) {
        scanf("%s", step);
        int flag = 0;
        int r = rand() % 3 + 1; 
        text[0] = r + '0';

        memcpy(bufWrite.mtext, text, strlen(text)+1);
        bufWrite.mtype = 1;

        printf("Sending %c.\n", bufWrite.mtext[0]);
        
        // Message sent to first process.
        if (msgsnd(msgIdWrite1, (struct msgbuf *)&bufWrite, strlen(text)+1, 0) == -1) {
            perror("msgsnd");
        }

        // Message sent to first process.
        if (msgsnd(msgIdWrite2, (struct msgbuf *)&bufWrite, strlen(text)+1, 0) == -1) {
            perror("msgsnd");
        }

        // Message sent to first process.
        if (msgsnd(msgIdWrite3, (struct msgbuf *)&bufWrite, strlen(text)+1, 0) == -1) {
            perror("msgsnd");
        }
        
        // Message recived.
        if (msgrcv(msgIdRead, (struct msgbuf *)&bufRead, sizeof(bufRead)-sizeof(long), 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
        
        printf("Got answer.\n");       
    }

    return 0;
}



