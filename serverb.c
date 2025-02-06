#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static pid_t g_client_pid = 0;

static void handle_signal(int sig, siginfo_t *info, void *context)
{
    static char str[10000];
    static int bit = 0;
    static int i = 0;
    
    (void)context;
    g_client_pid = info->si_pid;

    if (sig == SIGUSR1)
        str[i] |= (1 << bit);
    bit++;

    if (bit == 8)
    {
        if (str[i] == '\0')
        {
            write(1, str, i);
            write(1, "\n", 1);
            i = 0;
            bit = 0;
            str[0] = 0;
        }
        else
        {
            i++;
            bit = 0;
            str[i] = 0;
        }
    }
    kill(g_client_pid, SIGUSR1);
}

static void print_pid(int pid)
{
    char c;

    if (pid > 9)
        print_pid(pid / 10);
    c = (pid % 10) + '0';
    write(1, &c, 1);
}

int main(void)
{
    struct sigaction sa;

    sa.sa_sigaction = handle_signal;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    write(1, "Server PID: ", 11);
    print_pid(getpid());
    write(1, "\n", 1);

    while (1)
        pause();
    return (0);
}