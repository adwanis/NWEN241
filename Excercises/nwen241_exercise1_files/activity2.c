#include <stdio.h>

int main(void)
{
    int a, b;
    FILE *in; // use for handling input file
    FILE *out; // use for handling output file

    // Open raw.txt for reading
       
       in = fopen("raw.txt", "r");
       if(in == NULL){
        printf("Input Error");
        return 1;
       }

    // Open processed.txt for writing

        out = fopen("processed.txt","w");
        if(out == NULL){
        printf("Output Error");
        return 1;
       }
    // Go thru raw.txt file and generate processed.txt file accordingly
     while(fscanf(in,"%d %d", &a, &b) != EOF ){
        
        int c = a + b;

       fprintf(out, "%d %d %d\n", a, b, c);
     }          
       

       fclose(in);
       fclose(out);

    return 0;
}
