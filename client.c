#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void send_char(int pid, char c)
{
    int bit;

    for (bit = 0; bit < 8; bit++)
    {
        if (c & (1 << bit))
            kill(pid, SIGUSR1);
        else
            kill(pid, SIGUSR2);
        usleep(100);
    }
}

int main(int argc, char *argv[])
{
    int server_pid;

    if (argc != 3)
    {
        printf("Usage: %s <server_pid> <string>\n", argv[0]);
        return 1;
    }

    server_pid = atoi(argv[1]);
    if (server_pid <= 0)
    {
        printf("Invalid PID\n");
        return 1;
    }

    for (int i = 0; argv[2][i]; i++)
        send_char(server_pid, argv[2][i]);
    
    send_char(server_pid, '\0');

    return 0;
} 