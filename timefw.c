#include "timefw.h"

void make_clean(void)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        execlp("make", "make", "clean", NULL);
        exit(-1);
    }
    wait(&status);
}

void make_it(int htsize)
{
    pid_t pid;
    int status;
    char sizebuf[64];

    make_clean();
    pid = fork();
    if (pid == 0)
    {
        sprintf(sizebuf, "ops=-DHT_SIZE=%d", htsize);
        execlp("make", "make", sizebuf, NULL);
        exit(-1);
    }
    wait(&status);
}

void set_timer(void)
{
    struct itimerval v;
    memset(&v, 0, sizeof(struct itimerval));

    v.it_value.tv_usec = 1;
    setitimer(ITIMER_REAL, &v, NULL);
}

void gimme_timer(void)
{
    struct itimerval v;

    memset(&v, 0, sizeof(struct itimerval));
    getitimer( ITIMER_REAL, &v);
    printf("GOON:%ld %ld\n", v.it_value.tv_sec, v.it_value.tv_usec);
}

void execute_fw()
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        execlp("./fw", "./fw", "tests/the", NULL);
        exit(-1);
    }
    wait(&status);
}


int main(int argc, char** argv)
{
    chdir("fw");
    make_it(3);
    set_timer();
    execute_fw();
    gimme_timer();
    return 0;
}

