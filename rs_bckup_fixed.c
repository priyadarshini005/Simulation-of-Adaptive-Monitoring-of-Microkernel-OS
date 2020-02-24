#include<sys/stat.h>

#include<pthread.h>

#include<fcntl.h>

#include<unistd.h>

#include<stdio.h>

#include<sys/time.h>


int brk_flag;

struct timeval stop;

struct args_struct
{
double exe_time,timeout;

struct timeval start;

};

void* time_calc(struct args_struct *args)
{
args->timeout=0.001507;

while(1)

{

gettimeofday(&stop,NULL);

args->exe_time=(double)(stop.tv_usec-args->start.tv_usec)/1000000000.0;


if((args->exe_time)>(args->timeout))

{

printf("\nKSS failed\nRestarting KSS\n");

system("var=`pgrep serv`;kill $var;");

system("./serv");

printf("\nKSS Restarted\n");

printf("Execution time=%lf\tTimeout=%lf\n",args->exe_time,args->timeout);

}

else if(brk_flag)

{

printf("\nKSS Responded\n");

break;

}

}

}

void* receive_response(int *fd2)

{

char response[20];

read(*fd2,&response,20);

printf("\n%s\n",response);

brk_flag=1;

}

int main()

{

FILE *fp;

int fd1,fd2,fd_in,fd_out,i;

double timeout;

pthread_t thread1,thread2;

struct args_struct args;

struct timeval start;

mknod("fifo1",S_IFIFO|0666,0);

mknod("fifo2",S_IFIFO|0666,0);

//mknod("fifo_1",S_IFIFO|0666,0);

fd1=open("fifo1",O_WRONLY);

fd2=open("fifo2",O_RDONLY);

i=0;

while(1)

{

brk_flag=0;

//fd_in=open("fifo_1",O_RDONLY);

gettimeofday(&start,NULL);

printf("\nKSS Pinged\n");

write(fd1,"\nRS:Hello\n",11);

args.start=start;

//read(fd_in,&timeout,sizeof(double));

args.timeout=timeout;

pthread_create(&thread1,NULL,time_calc,&args);

pthread_create(&thread2,NULL,receive_response,&fd2);

pthread_join(thread1,NULL);

pthread_join(thread2,NULL);

/*mknod("fifo_2",S_IFIFO|0666,0);

if(args.exe_time<timeout)

{

fd_out=open("fifo_2",O_WRONLY);

write(fd_out,&args.exe_time,sizeof(double));

}

*/
i++;

printf("Execution time=%lf\n",args.exe_time);

}

/*unlink("fifo_1");

unlink("fifo_2");*/

unlink("fifo1");

unlink("fifo2");

}
