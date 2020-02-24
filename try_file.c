#include<stdio.h>

int main()

{

FILE *fp;

int i=0,n;

fp=fopen("Try_input","r");

while(i<10){
fscanf(fp,"%d\n",&n);
printf("%d\n",n);
i++;
}
fclose(fp);
}

