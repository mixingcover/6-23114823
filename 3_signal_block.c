#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static volatile sig_atomic_t count = 0;

static void handler(int sig)
{
    (void)sig;
    count++;
}

int main(void)
{
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    sigset_t block, old;
    sigemptyset(&block);
    sigaddset(&block, SIGINT);

    if (sigprocmask(SIG_BLOCK, &block, &old) == -1) {
        perror("sigprocmask");
        return 1;
    }

    printf("5초 동안 Ctrl+C를 눌러 보세요.\n");
    sleep(5);

    printf("차단 해제 전 전달 횟수: %d\n", (int)count);

    if (sigprocmask(SIG_SETMASK, &old, NULL) == -1) {
        perror("sigprocmask");
        return 1;
    }

    printf("차단 해제 후 전달 횟수: %d\n", (int)count);

    return 0;
}