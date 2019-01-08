#include<stdio.h>
#include<string.h>

int main() {
        long int obfuscatedIP;
        int j=0, i=1;
        const long* deobfuscateIPv4();
        const long *ipv4FinalOutput;
	char displayString[16]="";
	char ds2[16]="";

        printf("Enter the obfuscated IPv4 Address: ");
        scanf("%ld",&obfuscatedIP);

        ipv4FinalOutput=deobfuscateIPv4(obfuscatedIP);

        for(i=0;i<4;i++){
		sprintf(ds2,"%ld", ipv4FinalOutput[i]);

		
                if(i!=4){
                        strcat(displayString, ds2);
			if(i!=3){
				strcat(displayString,".");
			}
		}
                   
        }

	printf("IPv4 Address:%s\n", displayString);


        return 0;
}


const long* deobfuscateIPv4(long int obfuscatedIPv4)
{
static long decodedIPv4[4] = {'0','0','0','0'};	
	

   char rawHexObfuscatedIP[100];
   long int obfuscatedIP,remainder;
   int i=1,temp, j=0, xTrack=0, oTrack;
   char iasc[5];
	        
        char octet1h[4][5] = { {'0','x','0','0','\0'},
                               {'0','x','0','0','\0'},
                               {'0','x','0','0','\0'},
                               {'0','x','0','0','\0'}
                                                       };						
   while(obfuscatedIPv4!=0) {
        temp = obfuscatedIPv4 % 16;
        //To convert integer into character
        if( temp < 10)
            temp =temp + 48; else
            temp = temp + 55;
            rawHexObfuscatedIP[i++]= temp;
            obfuscatedIPv4 = obfuscatedIPv4 / 16;
    }

    sprintf(iasc,"%d",i);
    rawHexObfuscatedIP[0]=iasc[0];
	
	//rawHexObfuscatedIP[0] represents #elements in array, rawHexObfuscatedIP[1] is start of data
    //remember to convert it to int as it is type char or ascii code i.e. equiv to integer of 54!!
	//It looks right but it is not because it is very unintuitive like 9-1=8 but really it is 9-1=54 
	//because the '9' is integer 54 in ascii!!

	for (j=rawHexObfuscatedIP[0] - '0' ;j> 0;j--){
	  if(j % 2 == 0){
		oTrack=2;
	  }else{
		oTrack=3;
	  }
	  octet1h[xTrack][oTrack]=rawHexObfuscatedIP[j];
	 
	  if(oTrack==3 && j<8)
		xTrack++;
	}
	
	for(i=0;i<4;i++){
	    decodedIPv4[i]=strtol(octet1h[i], NULL, 16);
	}	
return decodedIPv4;
}
