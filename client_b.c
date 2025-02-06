#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static int g_signal_received = 0;

static void wait_signal(void)
{
    while (!g_signal_received)
        usleep(100);
    g_signal_received = 0;
}

static void handle_signal(int sig)
{
    (void)sig;
    g_signal_received = 1;
}

static void send_char(int pid, char c)
{
    int bit;

    bit = 0;
    while (bit < 8)
    {
        if ((c & (1 << bit)) != 0)
            kill(pid, SIGUSR1);
        else
            kill(pid, SIGUSR2);
        wait_signal();
        bit++;
    }
}

static void send_string(int pid, char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        send_char(pid, str[i]);
        i++;
    }
    send_char(pid, '\0');
    write(1, "Message sent successfully!\n", 28);
}

int main(int argc, char **argv)
{
    struct sigaction sa;

    if (argc != 3)
    {
        write(2, " <server_pid> <string>\n", 22);
        return (1);
    }

    sa.sa_handler = handle_signal;
    sa.sa_flags = SA_INFO;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);

    send_string(atoi(argv[1]), argv[2]);
    return (0);
}