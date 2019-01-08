#include <stdio.h>
#include <string.h>

int main(int argc,char *argv[]){

	char hexMacInput[13]={'8','0','0','0','0','0','0','0','0','0','0','0','0'};	
	const char *spoofMacIn = NULL;
	int j=0, i, xTrack=0, yTrack, spoofMacInLen=0;
        char octet1h[6][5] = { {'0','x','0','0','\0'},
                               {'0','x','0','0','\0'},
                               {'0','x','0','0','\0'},
                               {'0','x','0','0','\0'},
			       {'0','x','0','0','\0'},
       			       {'0','x','0','0','\0'}
                                                       };
	static long decodedLongMac[6] = {'0','0','0','0','0','0'};														   

	if(argc!=2){	
	   scanf("%s", &hexMacInput[0]);
	}else{
	   spoofMacIn=argv[1];
	   spoofMacInLen=(int)strlen(argv[1]);
	   for(i=0;i<12;i++)
		hexMacInput[i]=spoofMacIn[i];
	}	

 printf("strlen of argv[1] is: %i\n", spoofMacInLen);	

	//Take hex input string and put it into 2D Array for conversion to long

	for (j=0 ;j<11;j++){
	  if(j % 2 == 0){
		yTrack=2;
	  }else{
		yTrack=3;
	  }
	  octet1h[xTrack][yTrack]=hexMacInput[j];
	 
	  if(yTrack==3 && j<12)
		xTrack++;
	}
	
	for(i=0;i<6;i++){
	    decodedLongMac[i]=strtol(octet1h[i], NULL, 16);
	    printf("%ld", decodedLongMac[i]);
	    
	    if(i<5)
		printf(";");
	}   
	printf("\n");

	return 0;

}
