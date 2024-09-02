#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

int total = 0;

for(int i = 1;i < argc; i++){

	int num;
	if (sscanf(argv[i], "%d", &num) == 1) {
            total += num;
        }

}


printf("%d\n", total);
return 0;


}