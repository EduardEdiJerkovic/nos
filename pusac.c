#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

struct my_msgbuf {
    long mtype;
    char mtext[200];
};

int msgReadId, msgWriteId;

void retreat(int failure) 
{
    if (msgctl(msgReadId, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }
    exit(0);
}

int main(int argc, char **argv)
{
    struct my_msgbuf bufRead, bufWrite;
    key_t keyRead, keyWrite;
    char text[200];

    int i = argv[1][0] - '0';
    keyRead = 1000 + i;
    keyWrite = 2000;
    
    if ((msgReadId = msgget(keyRead, 0600 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }

    if ((msgWriteId = msgget(keyWrite, 0600 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }
    
    sigset(SIGINT, retreat);
    
    for(;;) {
        if (msgrcv(msgReadId, (struct msgbuf *)&bufRead, sizeof(bufRead), 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

        printf("%d. pusac got material type %c.\n", i, bufRead.mtext[0]);

        if (bufRead.mtext[0] == '0' + i) {
            printf("%d. pusac took the material.\n", i);
            text[0] = bufRead.mtext[0];
            memcpy(bufWrite.mtext, text, strlen(text) + 1);
            bufWrite.mtype = 1;
            if (msgsnd(msgWriteId, (struct msgbuf *)&bufWrite, sizeof(text), 0) == -1) {
                perror("msgsnd");
            }
        }
    }

    return 0;
}



