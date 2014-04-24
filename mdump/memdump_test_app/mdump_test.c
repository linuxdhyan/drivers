#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXBUFLEN 4095

/* This macro will enable debug mode ,and will print the address of 
   each objects user declare in stack,heap,data,bss in console  */
#define DEBUG
/* Define Global Variable which will go into data segment */
int global_var = 0xdeadbeaf;

/* Define Global Variable which will go into bss segment */
int global_var2 ;

int main()
{
	
	/* Define Variable which will go into stack segment of elf*/
	char lc_var =0x07;
	int li_var =0x0708;
	char readme[MAXBUFLEN + 1] ;

	/* Define Pointers which point to heap segment using malloc*/
	char *ptr1,*ptr2,*ptr3;
	
	FILE *fp = fopen("/usr/bin/README", "r");

	/* Intialize all pointers through malloc */
	ptr1 = malloc(100);
	ptr2 = malloc(100);
	ptr3 = malloc(300);
	
	/* read a file into the local buffer ,so that it will show in stack dump */
	if (fp != NULL) {
		size_t newLen = fread(readme, sizeof(char),MAXBUFLEN , fp);
		if (newLen == 0) {
			fputs("Error reading file", stderr);
		} else {
			readme[++newLen] = '\0'; /* Just to be safe. */
		}
		fclose(fp);
	}
	else
	{
		fputs("Error in Opening file", stderr);

	}
	
	/* Dump Some data to Heap so that it will show in heap dump */
	sprintf(ptr1,"SARD RnD TEAM AMARJA PROTO TYPE \n");		
	sprintf(ptr2,"                                \n");		
	sprintf(ptr3,"\nTHIS IS A HEAP SEGMENT WRITTEN FROM  FILE %s FUNCTION \
		%s() LINE NUMBER %d\n", __FILE__ , __FUNCTION__ , __LINE__ );		

	/* Keep Printing the values so that if someone want to Know 
	   the address of each defined objects */
	while(1)
	{
		#ifdef DEBUG	
		printf("address of global_var  (data) 	 	 :%08x\n",&global_var);
		printf("address of global_var2 (bss) 	 	 :%08x\n",&global_var2);
		printf("address of char var    (stack)  	 :%08lx\n",&lc_var);
		printf("address of int var     (stack) 	 	 :%08lx\n",&li_var);
		printf("address of pointer1    (heap)	 	 :%08lx\n",ptr1);
		printf("address of pointer2    (heap)	 	 :%08lx\n",ptr2);
		printf("address of pointer3    (heap) 	 	 :%08lx\n",ptr3);
		#endif
		sleep(100);
	}

	return 0;
	
}
