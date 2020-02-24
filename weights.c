#include<stdio.h>
int main()
{
FILE *fp;
int N,j,k;
float weight[50],sum;
for(N=1;N<=50;N++)
{
for(j=N-1;j>=0;j--)
{
sum=0.0;

if(j==N-1)
weight[j]=(N+4.0)/(N*4.0);

else if(j>0&&j<N-1)
{
for(k=j+1;k<N;k++)
sum+=weight[k];

weight[j]=(1-sum)*weight[N-1];
}
else if(j==0)
{
for(k=1;k<N;k++)
sum+=weight[k];

weight[j]=1-sum;
}
}
}
fp=fopen("weights","w");
printf("The weights are:\n");
for(j=0;j<50;j++)
{
printf("%d-%lf\n",(j+1),weight[j]);
fprintf(fp,"%lf\n",weight[j]);
}
}
