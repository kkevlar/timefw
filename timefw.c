#include "timefw.h"

void make_clean(int logfile)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        dup2(logfile, STDOUT_FILENO);
        close(logfile);
        execlp("make", "make", "clean", NULL);
        exit(-1);
    }
    wait(&status);
}

void make_it(int logfile, int htsize)
{
    pid_t pid;
    int status;
    char sizebuf[64];

    make_clean(logfile);
    pid = fork();
    if (pid == 0)
    {
        dup2(logfile, STDOUT_FILENO);
        close(logfile);
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

void gimme_timer(struct timeval* start_time, int num)
{
    struct timeval end_time;
    struct timeval difference;
    long long wow;

    gettimeofday(&end_time,0x0);
    wow = timeval_diff(&difference, &end_time, start_time);

    printf("%07d,%09lld\n", num, wow);
}

void execute_fw(int logfile)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        dup2(logfile, STDOUT_FILENO);
        close(logfile);
        execlp("./fw", "./fw", "tests/the", NULL);
        exit(-1);
    }
    wait(&status);
}

void do_it_all(int logfile, int num)
{
    struct timeval start_time;

    make_it(logfile, num);
    set_timer(&start_time);
    execute_fw(logfile);
    gimme_timer(&start_time, num);
}


int main(int argc, char** argv)
{
    int logfile;
    int i;

    logfile = open("logfile.txt", 
        O_CREAT | O_TRUNC | O_WRONLY,
        0666);

    chdir("fw");

    for (i = 0; i < 300; i++)
    {
        fprintf(stderr, "ROUND %d\n", i);
        
        
        do_it_all(logfile, 1021);
        do_it_all(logfile, 1022);
        do_it_all(logfile, 1024);
        
        do_it_all(logfile, 83160);
        do_it_all(logfile, 83177);
        

    }   

    return 0;
}

