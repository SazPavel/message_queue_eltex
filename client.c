#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct msg_buf
{
    long type;
    char buf[8];
};

int main()
{
    key_t key;
    char tmp[4] = "Hi\n";
    int id, i;
    struct msg_buf msg;
    msg.type = 1L;
    key = ftok("server", 'a');
    if(key == -1)
    {
        perror("ftok");
        exit(-1);
    }
    id = msgget(key, 0);
    if(id == -1)
    {
        perror("msgget");
        exit(-1);
    }
    if(msgrcv(id, &msg, sizeof(msg.buf), msg.type, 0) == -1)
    {
        if(msgctl(id, IPC_RMID, 0) == -1)
        {
            perror("msgctl");
            exit(-1);
        }
        perror("msgrcv");
        exit(-1);
    }
    printf("client: %s", msg.buf);
    memset(&msg.buf[0], 0, sizeof(msg.buf));
    for(i = 0; i < sizeof(tmp); i++)
         msg.buf[i] = tmp[i];
    msg.type = 2L;
    if(msgsnd(id, &msg, sizeof(msg.buf), 0) == -1)
    {
        if(msgctl(id, IPC_RMID, 0) == -1)
        {
            perror("msgctl");
            exit(-1);
        }
        perror("msgsnd");
        exit(-1);
    }
    exit(0);
}
