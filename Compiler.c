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

char* parseOP(int op);
void printCode(int *codes);

typedef struct instruction
{
    int op;
    int l;
    int m;
}instruction;


void fetchCode()
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

void printCode(int *codes)
{
    int i;
    for(i=0; i<codeSize; i++)
    {
        if(i%3==0)
        {
            if(i!=0)
                printf("\n");

            printf("%d ", i/3);

            printf("%s ", parseOP(codes[i]));

        }
        else
        {
            printf("%d ", codes[i]);
        }
    }
}

void fetchInstruction()
{

}

char *parseOP(int op)
{
    char *opword = NULL;

    switch(op)
    {
    case 1:
        opword = "lit";
        break;
    case 2:
        opword = "opr";
        break;
    case 3:
        opword = "lod";
        break;
    case 4:
        opword = "sto";
        break;
    case 5:
        opword = "cal";
        break;
    case 6:
        opword = "inc";
        break;
    case 7:
        opword = "jmp";
        break;
    case 8:
        opword = "jpc";
        break;
    case 9:
        opword = "sio";
    default:
        break;

    }

    return opword;
}

int main(int argc, char *argv[])
{
    fetchCode();
    printCode(code);


    return 0;
}
