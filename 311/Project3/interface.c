
#include "headers.h"
int main(){ \
    int err; //error number
    int toChild[2];
    int fromChild[2];
    pid_t childpid;
    err = pipe(toChild);
    char param1[10]; 
    char param2[10];
    if(err==-1)
        printf("error on pipe1 errno: %d\n",errno); 
    childpid=pipe(fromChild);
    char param3[10];
    char param4[10];
    if(err==-1)
        printf("error on pipe2 errno: %d\n",errno);       
    pid_t parentpid=getpid(); 
    sprintf(param1, "%d", toChild[0]);
    sprintf(param2, "%d", toChild[1]); 
    sprintf(param3, "%d", fromChild[0]);
    sprintf(param4, "%d", fromChild[1]); 
    err=fork();
    if (err==-1)
       printf("error on fork errno: %d\n",errno);
    pid_t processpid = getpid();
    if(processpid-parentpid>=1){

        char *param[]={"a", "b", "c"};
        //initialize db
        err= execl("./db","db",param1, param2, param3, param4, NULL);
        if (err==-1)
          printf("error on execve ./db: %d\n",errno);
        return 0; 
     } 
    //print out data in file
    //initialize variables
   
    char input[BUFFLEN+1]; //initial input
    char buffer [BUFFLEN+1];
    char dud [1]; 
    sprintf(dud, " ");
    char *split,*dup; //splitting the input
    int id,m,d,y;
    //inputs that are reconized. 
    char *exit="exit";
    char *account="account";
    char *list = "list";
    char *date = "date";
    char *comma=",";
    char *slash="/";
    close(toChild[0]);
    close(fromChild[1]);
    int running=1; 
    int status;
    
    while (running){
    	 err=read(fromChild[0], buffer, BUFFLEN);
   		 if(err==-1){
    	 	printf("from child pipe error, number: %d\n", errno);
    	 	}
    	
   			 buffer[BUFFLEN]= '\0';
   		 if(strcmp(buffer, "done")==0){
   		 	running=0;
   		 }
   		 else  printf("%s",buffer);
   		
   		             
   		 err=write(toChild[1], buffer, strlen(buffer)+1);
         if(err==-1){
            printf("trouble writing to child, errno: %d\n", errno);
            }
    }
    
    running=1; 
    while(running){
    	
    	 printf("\nInput command (account,id | list | date,mm/dd/yy | exit):\n");
   		 scanf("%s",input);
     
    	 err=write(toChild[1], input, strlen(input)+1);
         if(err==-1){
            printf("trouble writing to child, errno: %d\n", errno);
            }
         if(strcmp(input, "list")==0){
          int printing=1; 

         while(printing){
             err=read(fromChild[0], buffer, BUFFLEN);
             if(err==-1){
              printf("from child pipe error, number: %d\n", errno);
              }
            
             buffer[BUFFLEN]= '\0';
             if(strcmp(buffer, "done")==0){
              printing=0;
             }
             else  printf("%s",buffer);
            
                         
             err=write(toChild[1], buffer, strlen(buffer)+1);
               if(err==-1){
                  printf("trouble writing to child, errno: %d\n", errno);
                  }
			     }
	   	}
			
		 else {
			err=read(fromChild[0], buffer, BUFFLEN);
   		 	if(err==-1)
    	    printf("pipe0 error, number: %d\n", errno);
   		 	buffer[BUFFLEN]= '\0';
   		 	printf("%s\n",buffer);

		    }
     if(strcmp(input, "exit")==0){
          err=waitpid(childpid,&status,0);
          if(err==-1)
            {
               printf("waitpid error, errno: %d\n", errno);
            }
            running=0;
          }
    }
    printf("\ninterface: child process (%d) completed.\n",childpid );
    printf("interface: child process exit status = %d.\n", WEXITSTATUS(status));
    printf("interface: Complete.\n");
    return 0;
}
