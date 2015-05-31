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
void executeInstruction();
void printStack();
void printInfo();

int halt = 0;
int arCount = 1;
int presp = -1;

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
    printCode(code);

    while(halt == 0)
    {
        fetchInstruction();
        executeInstruction();
        printInfo();
    }
    return 0;
}

//initializes the arrays
//fetches the code, then closes the file
void load()
{
    initArray();
    fetchCode();
    fclose(codeFile);
}

void printInfo()
{
    if(ir.op!=NULL && ir.op!=0)
    {
        printf("%3s ", parseOP(ir.op));
        printf("%3d ",ir.l);
        printf("%3d ", ir.m);
    }
        printf(" %3d %3d %3d     ", pc/3, bp, sp);
        printStack();
}

//reads in the file and populates the code array
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

//prints the code read in for part 1 of the output
//for the assignment
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
    printf("\n\n");
}

//calls get instruction and increments pc counter
void fetchInstruction()
{
    getInstruction();
    pc += NEXT_INSTRUCTION;
}

//gets the instruction at the proper PC and places
//inside of ir struct
void getInstruction()
{
    ir.op = code[pc];
    ir.l = code[pc + 1];
    ir.m =  code[pc + 2];
}

//initializes stack and code arrays
void initArray()
{
    int i;
    for(i = 0; i < MAX_STACK_HEIGHT; i ++)
        stack[i] = 0;
    for(i = 0; i < MAX_CODE_LENGTH; i ++)
        code[i] = 0;

}

//check for halts and null data
int isDone ()
{
    if(!code[pc] || (code[pc]==9 && code[pc + 2] == 2))
        return 1;
    return 0;
}

void printStack()
{
    int i, j;

    for(i=1; i<=sp; i++)
    {
        //for(j=0; j<arCount; j++)
        //{
            if(i == presp && arCount > 1)
            {
                printf("| ");
            }
        //}
        printf("%d ", stack[i]);
    }
    printf("\n");
}

//changes the op passed in into a string
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
    switch(ir.m)
    {
    case 0:
        arCount--;
        sp = bp-1;
        pc = stack[sp+4]*NEXT_INSTRUCTION;
        bp = stack[sp+3];
        break;
    default:
        break;
    }
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
    arCount++;
    presp = sp + 1;

    stack[sp + 1] = 0;
    stack[sp + 2] = getBase(ir.l, bp);
    stack[sp + 3] = bp;
    stack[sp + 4] = pc/3;
    bp = sp + 1;
    pc = ir.m * NEXT_INSTRUCTION;
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
    case 2:
        halt = 1;
        break;
    default:
        break;
    }
}

void executeInstruction()
{
    switch(ir.op)
    {
    case 1:
        lit();
        break;
    case 2:
        opr();
        break;
    case 3:
        lod();
        break;
    case 4:
        sto();
        break;
    case 5:
        cal();
        break;
    case 6:
        inc();
        break;
    case 7:
        jmp();
        break;
    case 8:
        jpc();
        break;
    case 9:
        sio();
    default:
        break;

    }
}


