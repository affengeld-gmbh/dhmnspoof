#include<stdio.h>
int main() {
	long int decimalNumber,remainder,quotient;
	int i=1,j,temp;
	char hexadecimalNumber[100];
	char iasc[2];


	printf("Enter any decimal number: ");
	scanf("%ld",&decimalNumber);
	quotient = decimalNumber;
	while(quotient!=0) {
		temp = quotient % 16;
		//To convert integer into character
		if( temp < 10)
		           temp =temp + 48; else
		         temp = temp + 55;
		hexadecimalNumber[i++]= temp;
		quotient = quotient / 16;
	}

	//convert integer to char
	sprintf(iasc,"%d",i);
	hexadecimalNumber[0]=iasc[0];

	printf("Equivalent hexadecimal value of decimal number %ld: ",decimalNumber);
	for (j = i -1 ;j> 0;j--)
	      printf("%c",hexadecimalNumber[j]);

              printf("\n");

	printf("Hexadecimalnumber arr 0 : %c\n", hexadecimalNumber[0]);

	const char* returnArrayPointer();
	const char *baboon;	


	baboon=returnArrayPointer();


	printf("%c\n", baboon[0]);

	return 0;
}



const char* returnArrayPointer() 
{
static char array[10];

   array[0]='A';
   array[1]='B';

return array; 
}
