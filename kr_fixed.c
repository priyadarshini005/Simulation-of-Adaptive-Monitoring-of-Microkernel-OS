#include<stdlib.h>

#include<time.h>

#include<sys/stat.h>

#include<fcntl.h>

#include<stdio.h>

int main()

{

int key,fd1,fd2,i=0,j;

char buf[35];

while(1)

{

key=rand()%1000;

mknod("key_fifo1",0666|S_IFIFO,0);

mknod("key_fifo2",0666|S_IFIFO,0);

fd1=open("key_fifo1",O_WRONLY);

printf("%d-The requested key is %d\n\n",(++i),key);

write(fd1,&key,sizeof(int));

fd2=open("key_fifo2",O_RDONLY);

read(fd2,buf,35);

buf[34]='\0';

for(j=0;j<10000;j++)

{
}

puts(buf);

printf("\n-------------------------------------------------------------------\n");

}

unlink("key_fifo1");

unlink("key_fifo2");

return 0;

}
