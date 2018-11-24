#include "../src/scanner.c"
int main()
{
    char *buffer= NULL;
    int i,c=0;
    FILE *f;
    f=fopen("pom","r");
    while(i!=TYPE_EOF){
		myQueueInit();
		i=getNextToken(&buffer);
		// if(i == TYPE_EOL)
		//   c++;

		printf("TYPE : %d\n",i);
		if(buffer)
			printf("BUFFER %s\n",buffer );
	}

/*
     // ungetToken(i);
     // i= getNextToken(&buffer);

    printf("TYPE : %d\n",i);
    if(buffer)
        printf("BUFFER %s\n",buffer );

    i= getNextToken(&buffer);

      printf("TYPE : %d\n",i);
      if(buffer)
        printf("BUFFER %s\n",buffer );

      i= getNextToken(&buffer);

      printf("TYPE : %d\n",i);
      if(buffer)
          printf("BUFFER %s\n",buffer );
*/
	myQueueFree();
    fclose(f);
    printf("%d\n",c );

}
