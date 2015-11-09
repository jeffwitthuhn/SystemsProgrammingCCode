#include<stdio.h>
#include <math.h>
#define BPB 8
#ifdef BIN
#define BitsPerDigit 1
#define DigitMask 1
#endif

#ifdef HEX
#define BitsPerDigit 4
#define DigitMask 15
#endif

#define SRC(x) x=((x >> BitsPerDigit) | (x << (sizeof(x)*BPB - BitsPerDigit)))
#define SLC(x) x=((x << BitsPerDigit) | (x >> (sizeof(x)*BPB - BitsPerDigit)))

void flushStream(){
	int c; 
	scanf("%c", &c);
	return;
}
void menu(){
	printf("Select option\n");
	printf("  1 - display the number\n");
	printf("  2 - shift left circular one digit\n");
	printf("  3 - shift right circular one digit\n");
	printf("  4 - set digit n to zero\n");
	printf("  5 - exit\n");
	return;

}
void display(unsigned number){
	//printf("1 and 2: %d, 1 and 1:%d, 1 and 0: %d", 1&2, 1&1, 1&0);
#ifdef BIN
	unsigned mask=DigitMask;
	mask=mask<<(BPB*sizeof(number)-BitsPerDigit);
	while(mask!=0){;
		unsigned digit=mask&number;
		if(digit!=0)
			printf("%d",((digit)/(digit)));
		else
			printf("%d",digit);
		mask=mask>>1;
		}
	printf("\n");
#endif
#ifdef HEX
	printf("0x%08X\n", number);
#endif 
	return;
}
void zero(unsigned *number, unsigned n){	
#ifdef BIN
	unsigned mask=BitsPerDigit<<n;
	*number &= (~mask);	
#endif
#ifdef HEX
	unsigned mask=DigitMask<<BitsPerDigit*n;
	*number &= (~mask);	
#endif

}
