#include<fcntl.h>

#include<stdlib.h>

#include<unistd.h>

#include<stdio.h>

#include<sys/stat.h>

#include<pthread.h>

double history[60],weight[60];

double weightedSum(int n)

{

int j,k;

double prodSum=0.0,sum;

double timeout;

for(j=0;j<n;j++)

{

if(j==0)

{

sum=0.0;

for(k=1;k<n;k++)

sum+=weight[k];

weight[j]=1.0-sum;

}

else if(j>0&&j<n-1)

{

sum=0.0;

for(k=j+1;k<n;k++)

sum+=weight[k];

weight[j]=(1.0-sum)*weight[n-1];

}

else
weight[j]=(n+4.0)/(4.0*n);

prodSum+=(weight[j]*history[j]);

}

timeout=history[n-1]+prodSum/n;

return timeout;

}

void update(double timeout,int n)

{

int i,j,k;

k=1;

printf("\n");

for(j=0;j<n-1;j++)

{

history[j]=history[k];

k++;

}

history[n-1]=timeout;

/*printf("The timeout history is listed as:\n");

for(i=0;i<n;i++)

printf("%lf\n",history[i]);
*/

}




int main()

{

int n=50,fd_in,fd_out,i;

FILE *fp_rt,*fp_wt;

pthread_t thread1,thread2,thread3;

double timeout,resp_time,lambda;


char proc_exe1[15]="./rs";

char proc_exe2[30]="./serv > kss_output";

char proc_exe3[30]="./cli > kr_output";


system("cc -o serv kss.c");

system("cc -o cli kr.c");

system("cc -lrt -lpthread -o rs rs.c");

system("rm fifo1 fifo2 fifo_1 fifo_2 key_fifo1 key_fifo2>errors.txt");

fp_rt=fopen("Initial_Response_times_history","r");

fp_wt=fopen("weights","r");

fseek(fp_rt,0,0);

fseek(fp_wt,0,0);

printf("The response times values and weights of the history:\n",n);

for(i=0;i<n;i++)

{

printf("%d-",(i+1));

fscanf(fp_rt,"%lf\n",&history[i]);

fscanf(fp_wt,"%lf\n",&weight[i]);

printf("%lf\t%lf\n",history[i],weight[i]);

}

printf("Execution starts here\n");

mknod("fifo_1",0666|S_IFIFO,0);

mknod("fifo_2",0666|S_IFIFO,0);

while(1)

{

printf("\nEstimating timeout...\n");

timeout=weightedSum(n);

printf("The timeout estimated is:%lf\n",timeout);

pthread_create(&thread1,NULL,system,&proc_exe1);

restart:printf("Starting KSS\n");

pthread_create(&thread2,NULL,system,&proc_exe2);

pthread_create(&thread3,NULL,system,&proc_exe3);




fd_out=open("fifo_1",O_WRONLY);

printf("Sending timeout to RS:%lf\n",timeout);

write(fd_out,&timeout,sizeof(double));

printf("\nTimeout sent to RS\n");


fd_in=open("fifo_2",O_RDONLY);

read(fd_in,&resp_time,sizeof(double));

printf("\nThe response time received from RS=%lf",resp_time);


if(resp_time<=timeout)

update(resp_time,n);


else

{

mknod("fifo_1",0666|S_IFIFO,0);

mknod("fifo_2",0666|S_IFIFO,0);

system("var=`pgrep serv`;kill $var;");

system("var=`pgrep cli`;kill $var;");

lambda=1+(random()%100)/100.0;

resp_time*=lambda;

update(resp_time,n);

printf("\nResponse time history updated with new value:%lf\n",resp_time);

printf("----------------------------------------------------------------------------------\n");

timeout=weightedSum(n);

printf("The timeout estimated is:%lf\n",timeout);

goto restart;

}

printf("\nResponse time history updated with new value:%lf\n",resp_time);

printf("----------------------------------------------------------------------------------\n");

}


unlink("fifo_1");

unlink("fifo_2");

}
