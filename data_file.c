#include <stdio.h>
#include <stdlib.h>
 int main() {
        int i, temp=0,n;
	//double resp_history[700];
	FILE *fp;
	if(fp==NULL){
	  printf("Error in File creation");
	  exit(0);
	}
	fp=fopen("kss_input_file.txt","w");	       
	for(i=0;i<700;i++)
	{
	  if((n=rand()%10)==0)
	    n=1;
	  temp+=n;
	  fprintf(fp,"%d\t",temp);	       
	}
	fclose(fp);
	fp=fopen("kss_input_file.txt","r");
	fseek(fp,0L,0);
	for(i=0;i<700;i++)
	{
		fscanf(fp,"%d\t",&n);
		printf("%d\t",n);
	}
        return 0;
  }
