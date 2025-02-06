#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static void handle_signal(int sig)
{
    static char c = 0;
    static int bit_position = 0;

    if (sig == SIGUSR1)
        c |= (1 << bit_position);
    bit_position++;

    if (bit_position == 8)
    {
        if (c == '\0')
        {
            write(1, "\n", 1);
            bit_position = 0;
            c = 0;
            return;
        }
        write(1, &c, 1);
        bit_position = 0;
        c = 0;
    }
}

int main(void)
{
    struct sigaction sa;

    sa.sa_handler = handle_signal;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    printf("Server PID: %d\n", getpid());

    while (1)
        pause();

    return 0;
}