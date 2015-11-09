#include "hw2.h"


unsigned value;
int main(){
	int input=5;
	printf("Input number\n");
	scanf("%d%*c",&value);
	menu();
	do{
		if(input==1){
			display(value);
			menu();

		}

		else if(input==2){
			SLC(value);
			display(value);
			menu();


		}

		else if(input==3){
			SRC(value);
			display(value);
			menu();


		}

		else if(input==4){
			unsigned n;
			printf("specify digit to be zeroed: ");
			scanf("%d%*c", &n);
			zero(&value, n);
			display(value);
			menu();


		}
		else if(input==6)
			scanf("%d%*c",&value);
	scanf("%d%*c", &input);
	
	}while(input!=5);


}