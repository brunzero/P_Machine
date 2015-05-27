#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

int sp = 0;
int bp = 1;
int pc = 0;
int ir = 0;
int codeSize = 0;

int code[MAX_STACK_HEIGHT];
int buffer;

typedef struct instruction
{
    int op;
    int l;
    int m;
}instruction;


void fetchInstruction()
{
    int i = 0;
    FILE *fp = fopen("mcode.txt", "r");
    while(fscanf(fp, "%d", &buffer)!=EOF)
    {
        code[i]=buffer;
        codeSize++;
        i++;
    }
}

void printInstruction(int *codes)
{
    int i;
    for(i=0; i<codeSize; i++)
    {
        if(i%3!=0)
        {
            printf("%d ", codes[i]);
        }
        else
        {
            if(i!=0)
                printf("\n");
            printf("%d ", i/3);
            printf("%d ", codes[i]);
        }
    }
}

int main(int argc, char *argv[])
{
    fetchInstruction();
    printInstruction(code);
    return 0;
}
