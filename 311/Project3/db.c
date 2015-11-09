#include "headers.h"
typedef struct Record{
	long id; 
	int checkNumber;
	float amount; 
	char date[9];
} transaction; 

int main(int argc, int **argv){
  	FILE *finput;
  	transaction* transactionList;
  	long id; 
  	long inputId;
  	int checkNumber;
  	float amount; 
  	int dateM;
  	int dateD;
  	int dateY;
  	char date[9];
  	int status; 
    int err; 
    float amountSum;
    char buffer[BUFFLEN+1];
    char input[BUFFLEN+1];
    char *exitstring ="db complete.";
    int fromParent[2];
    int toParent[2];
    char *p; 
  	char *split;
  	char outputSum[BUFFLEN+1];
  	char outputString[BUFFLEN+1];
  	int sorted=0; 
    sscanf (argv[1],"%d",&fromParent[0]);
    sscanf (argv[2],"%d",&fromParent[1]);
    sscanf (argv[3],"%d",&toParent[0]);
    sscanf (argv[4],"%d",&toParent[1]);
    
    finput=fopen("accountData", "r");

    //get record count to know how much space to allocate for list
    int recordCount =0;
    int scanning=1; 
    while (scanning){
         if(!feof(finput)){
        	 err=fscanf(finput, "%ld %d %s %f\n" , &id, &checkNumber,date, &amount);
       	 	recordCount++;
       	 	}
       	 else scanning=0;
       	 }
   	fclose(finput); 	
 	//allocate space for the list of transactions when requested; 
  	transactionList=malloc(sizeof(transaction)*recordCount);
    close(fromParent[1]); 
    close(toParent[0]);  
    finput=fopen("accountData", "r");
    int x;
	for (x = 0; x < recordCount; x++){
	    err=fscanf(finput, "%ld %d %s %f\n" , &id, &checkNumber,date, &amount);
   	 	transactionList[x].id = id;
    	transactionList[x].checkNumber = checkNumber;
   		strncpy(transactionList[x].date,date,9); 
    	transactionList[x].amount = amount;
	} 
	fclose(finput);
	for (x = 0; x < recordCount; x++){
		  char outputString[BUFFLEN+1];
		  outputString[0]='\0';
		  err=sprintf(outputString,"element[%d]: id = %ld, check Number: %d, date: %s, amount:  %.2f\n",
		  				x,transactionList[x].id
		  				,transactionList[x].checkNumber,transactionList[x].date
		  				,transactionList[x].amount);
		  err=write(toParent[1], outputString, strlen(outputString)+1);
    		if(err==-1){
    		   printf("child to parent pipe error, errno: %d\n", errno);
    		   exit(2);
    		   }
    	  err=read(fromParent[0], buffer, BUFFLEN);
    		if(err==-1){
        	printf("parent to child pipe error. errno: %d\n", errno);
       		 exit(1);
       	 }
    	  if(x==recordCount-1){
    		  	outputString[BUFFLEN+1];
    			outputString[0]='\0';
				err=sprintf(outputString,"done");
				err=write(toParent[1], outputString, strlen(outputString)+1);
    			if(err==-1){
    		  	 printf("child to parent pipe error, errno: %d\n", errno);
    		  	 exit(2);
    		  	   }   		  	 
            }  
      		}

	
	
    do{
    	char outputString[BUFFLEN+1];
    	outputString[0]='\0';
    	strcat(outputString, "response: ");
   	    err=read(fromParent[0], buffer, BUFFLEN);
    	if(err==-1){
        	printf("parent to child pipe error. errno: %d\n", errno);
       		 exit(1);
       	 }
    
    
   	 	buffer[BUFFLEN]= '\0';
     	split = strdup(buffer); 
     	split = strtok(split, ",");
     	
   	 	if ((strcmp(split,"exit")==0)){
    		err=write(toParent[1], exitstring, strlen(exitstring)+1);
    		if(err==-1){
    		   printf("child to parent pipe error, errno: %d\n", errno);
    		   exit(2);
    		   }


    	}
    	
   		else if((strcmp(split,"account")==0)){
   		      split = strtok(NULL, ",");
   		      inputId=atoi(split);
   		      finput=fopen("accountData", "r");
   		      scanning=1; 
   		      amountSum=0;
    		  while (scanning){
     				if(!feof(finput)){
    					 err=fscanf(finput, "%ld %d %s %f\n" , &id, &checkNumber,date, &amount);
   					
   						if(inputId==id)
   						 amountSum+=amount;
   	 				}
   					 else scanning=0;
   				 }
   			fclose(finput);
   			err=sprintf(outputSum, "Total for account %s: %.2f", split, amountSum);
   			strcat(outputString, outputSum);

   		}
   		
   		else if((strcmp(split,"list")==0)){
   		
   		 if(sorted==0){
   		 	int dateval1, dateval2;
        char date1[9],date2[9];
   		 	int y;
        transaction temp; 

   			for (x = 1; x < recordCount; x++){
          strncpy(date1,transactionList[x].date,9); 

   				dateval1=30*atoi(strtok(date1, "/"))
   							+atoi(strtok(NULL,"/"))
   							+365*atoi(strtok(NULL,"/"));
   				printf("dateval1 is: %d\n",dateval1);

   				y=x;
          strncpy(date2,transactionList[y-1].date,9); 
          dateval2=30*atoi(strtok(date2, "/"))
                +atoi(strtok(NULL,"/"))
                +365*atoi(strtok(NULL,"/"));

   				while((y>0) && (dateval2>dateval1))
   				{
   			  	temp.amount=transactionList[y-1].amount;
            temp.checkNumber=transactionList[y-1].id;
            temp.id=transactionList[y-1].id;
            strncpy(temp.date,transactionList[y-1].date,9);
            transactionList[y-1].amount=transactionList[y].amount;
            transactionList[y-1].checkNumber=transactionList[y].id;
            transactionList[y-1].id=transactionList[y].id;
            strncpy(transactionList[y-1].date,transactionList[y].date,9);
            transactionList[y].amount=temp.amount;
            transactionList[y].checkNumber=temp.id;
            transactionList[y].id=temp.id;
            strncpy(transactionList[y].date,temp.date,9);
            y--;
          if(y>0){
              strncpy(date2,transactionList[y-1].date,9); 

       				 dateval2=30*atoi(strtok(date2, "/"))
                    +atoi(strtok(NULL,"/"))
                    +365*atoi(strtok(NULL,"/"));
            }
   				}
   					
			} 
			int idval;
			for (x = 1; x < recordCount; x++){
        y=x;
				idval=transactionList[y].id;
	
   				while(y>0&&transactionList[y-1].id>idval){
       					temp.amount=transactionList[y-1].amount;
                temp.checkNumber=transactionList[y-1].id;
                temp.id=transactionList[y-1].id;
                strncpy(temp.date,transactionList[y-1].date,9);
                transactionList[y-1].amount=transactionList[y].amount;
                transactionList[y-1].checkNumber=transactionList[y].id;
                transactionList[y-1].id=transactionList[y].id;
                strncpy(transactionList[y-1].date,transactionList[y].date,9);
                transactionList[y].amount=temp.amount;
                transactionList[y].checkNumber=temp.id;
                transactionList[y].id=temp.id;
                strncpy(transactionList[y].date,temp.date,9);
       					y--;
   				
         				}
      			   }
      		 sorted=1;
      		 }
for (x = 0; x < recordCount; x++){
      char outputString[BUFFLEN+1];
      outputString[0]='\0';
      err=sprintf(outputString,"element[%d]: id = %ld, check Number: %d, date: %s, amount:  %.2f\n",
              x,transactionList[x].id
              ,transactionList[x].checkNumber,transactionList[x].date
              ,transactionList[x].amount);

      err=write(toParent[1], outputString, strlen(outputString)+1);
        if(err==-1){
           printf("child to parent pipe error, errno: %d\n", errno);
           exit(2);
           }
        err=read(fromParent[0], buffer, BUFFLEN);
        if(err==-1){
          printf("parent to child pipe error. errno: %d\n", errno);
           exit(1);
         }
        if(x==recordCount-1){
            outputString[BUFFLEN+1];
          outputString[0]='\0';
        err=sprintf(outputString,"done");
        err=write(toParent[1], outputString, strlen(outputString)+1);
          if(err==-1){
             printf("child to parent pipe error, errno: %d\n", errno);
             exit(2);
             }
          char outputString[BUFFLEN+1];
          outputString[0]='\0';
          strcat(outputString, "response: ");
            err=read(fromParent[0], buffer, BUFFLEN);
          if(err==-1){
              printf("parent to child pipe error. errno: %d\n", errno);
               exit(1);
         }
      buffer[BUFFLEN]= '\0';
             
            }  
          }
        }
   		
   		else if((strcmp(split,"date")==0)){
   		   	  finput=fopen("accountData", "r");
   		      split = strtok(NULL, ",");
   		      scanning=1; 
   		      amountSum=0;
    		  while (scanning){
     				if(!feof(finput)){
    					 err=fscanf(finput, "%ld %d %s %f\n" , &id, &checkNumber,date, &amount);
   					
   						if(strcmp(date, split)==0)
   						 amountSum+=amount;
   	 				}
   					 else scanning=0;
   				 }
   			fclose(finput);
   			err=sprintf(outputSum, "Total for account %s: %.2f", split, amountSum);
   			strcat(outputString, outputSum);
   		}
   		if(strcmp(buffer,"done")!=0){
   			err=write(toParent[1], outputString, strlen(outputString)+1);
    			if(err==-1){
    			   printf("child to parent pipe error, errno: %d\n", errno);
    		 	  exit(2);
    			  }
    	}
    } while ((strcmp(buffer,"exit")!=0));
    exit(0);

    return 0; 
}
