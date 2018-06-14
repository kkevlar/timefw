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

long long timeval_diff(
    struct timeval *difference,
    struct timeval *end_time,
    struct timeval *start_time)
{
  difference->tv_sec =end_time->tv_sec -start_time->tv_sec ;
  difference->tv_usec=end_time->tv_usec-start_time->tv_usec;

  /* Using while instead of if below makes the code slightly more robust. */

  while(difference->tv_usec<0)
  {
    difference->tv_usec+=1000000;
    difference->tv_sec -=1;
  }

  return 1000000LL*difference->tv_sec+
                   difference->tv_usec;

} 

void set_timer(struct timeval* start_time)
{
    gettimeofday(start_time,0x0);
}

void gimme_timer(struct timeval* start_time)
{
    struct timeval end_time;
    struct timeval difference;
    long long wow;

    gettimeofday(&end_time,0x0);
    wow = timeval_diff(&difference, &end_time, start_time);

    printf("TIME: (%ld secs) (%ld micros) \n", difference.tv_sec, difference.tv_usec);
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
    struct timeval start_time;

    chdir("fw");

    make_it(3);
    set_timer(&start_time);
    execute_fw();
    gimme_timer(&start_time);

    make_it(1);
    set_timer(&start_time);
    execute_fw();
    gimme_timer(&start_time);

    make_it(2);
    set_timer(&start_time);
    execute_fw();
    gimme_timer(&start_time);


    make_it(20000);
    set_timer(&start_time);
    execute_fw();
    gimme_timer(&start_time);


    make_it(3797);
    set_timer(&start_time);
    execute_fw();
    gimme_timer(&start_time);

    return 0;
}

