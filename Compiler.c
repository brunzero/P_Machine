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
void initArray();
void load();

//instruction set
void lit();
void opr();
void lod();
void sto();
void cal();
void inc();
void jmp();
void jpc();
void sio();

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

//01
void lit ()
{
    sp ++;
    stack[sp] = ir.m;
}

//02
void opr()
{

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

//05
void cal()
{
    stack[sp + 1] = 0;
    stack[sp + 2] = getBase(ir.l, bp);
    stack[sp + 3] = bp;
    stack[sp + 4] = pc;
    bp = sp + 1;
    pc = ir.m;
}

//06
void inc()
{
    sp = sp + ir.m;
}
//07
void jmp ()
{
    pc = ir.m * NEXT_INSTRUCTION;
}

//08
void jpc()
{
    if(stack[sp] == 0)
    {
        pc = ir.m;
    }
    sp = sp-1;
}

//09
void sio()
{
    switch(ir.m)
    {
    case 0:
        printf("%d\n", stack[sp]);
        sp = sp-1;
        break;
    case 1:
        sp = sp+1;
        scanf("%d", stack[sp]);
        break;
    default:
        break;
    }
}


int isDone ()
{
    if(!code[pc] || (code[pc]==9 && code[pc + 2] == 2))
        return 1;
    return 0;
}


