#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3
#define NEXT_INSTRUCTION 3

char* parseOP(int op);
void printCode(int *codes);
void fetchCode();
void getInstruction();
void fetchInstruction();
int getBase(int level, int base);
void lit(int pushValue);
void initArray();
void load();

typedef struct instruction
{
    int op;
    int l;
    int m;
}instruction;

int sp = 0;
int bp = 1;
int pc = 0;
int codeSize = 0;
int code[MAX_CODE_LENGTH];
int stack[MAX_STACK_HEIGHT];
int buffer;
instruction ir;
FILE *codeFile;





int main(int argc, char *argv[])
{
    load();
   // printCode(code);

    while(!isDone())
    {
        fetchInstruction();
    }

    return 0;
}

void load()
{
    initArray();
    stack[1] = 0;
    stack[2] = 0;
    stack[3] = 0;
    fetchCode();
    fclose(codeFile);
}

void fetchCode()
{
    int i = 0;
    codeFile = fopen("mcode.txt", "r");
    while(fscanf(codeFile, "%d", &buffer)!=EOF)
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
    getInstruction();
    pc += NEXT_INSTRUCTION;
}

void getInstruction()
{
    ir.op = code[pc];
    ir.l = code[pc + 1];
    ir.m =  code[pc + 2];
    printf("%d,",code[pc]);
    printf("%d,",code[pc + 1]);
    printf("%d \n",code[pc + 2]);
}

//initializes stack and code arrays
void initArray()
{
    int i;
    for(i = 0; i < MAX_STACK_HEIGHT; i ++)
        stack[i] = NULL;
    for(i = 0; i < MAX_CODE_LENGTH; i ++)
        code[i] = NULL;

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

int getBase(int level, int base)
{
    int i;
    for(i = level; i > 0; i --)
    {
        base = stack[base + 2];
    }
    return base;
}

void lit ()
{
    sp ++;
    stack[sp] = ir.m;
}

//01
void jmp ()
{
    pc = M * NEXT_INSTRUCTION;
}

//03
void lod ()
{
    sp++;
    stack[sp] = stack[getBase(ir.l, bp) + ir.m];
}

//04
void sto()
{
    stack[getBase(ir.l, bp) + ir.m] = stack[sp];
    sp = sp-1;
}

int isDone ()
{
    if(!code[pc])
        return 1;
    return 0;
}


