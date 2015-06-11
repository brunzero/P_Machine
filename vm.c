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
int tempPC = 0;
int codeSize = 0;
int code[MAX_CODE_LENGTH];
int stack[MAX_STACK_HEIGHT];
int buffer;
instruction ir;
FILE *codeFile;
FILE *output;





int main(int argc, char *argv[])
{
    load();
    output = fopen("stacktrace.txt", "w");

    fprintf(output, "Line  OP    L  M\n");

    printCode(code);

    fprintf(output, "                    pc  bp  sp   stack\n");
    fprintf(output, "Initial values      %d   %d   %d\n", pc, bp, sp);

    while(halt == 0)
    {
        fetchInstruction();
        executeInstruction();
        printInfo();
    }

    fclose(output);
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
        fprintf(output, "%2d   ", tempPC/3);
        tempPC = pc;
        fprintf(output, "%3s ", parseOP(ir.op));
        fprintf(output, "%3d ",ir.l);
        fprintf(output, "%3d ", ir.m);
    }
        fprintf(output, " %3d %3d %3d     ", pc/3, bp, sp);
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
                fprintf(output, "\n");

            fprintf(output, "%2d  ", i/3);

            fprintf(output, "  %3s ", parseOP(codes[i]));

        }
        else
        {
            fprintf(output, "%3d ", codes[i]);
        }
    }
    fprintf(output, "\n\n");
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
        if(i == bp && i>1)
        {
            fprintf(output, "| ");
        }
        fprintf(output, "%d ", stack[i]);
    }
    fprintf(output, "\n");
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
    int input;
    switch(ir.m)
    {
    case 0:
        fprintf(output, "%d\n", stack[sp]);
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

void ret ()
{
    sp = bp-1;
    pc = stack[sp+4]*NEXT_INSTRUCTION;
    bp = stack[sp+3];
}

void neg ()
{
    stack[sp] = -stack[sp];
}

void add ()
{
    sp --;
    stack[sp] = stack[sp] + stack[sp + 1];
}

void sub ()
{
    sp --;
    stack[sp] = stack[sp] - stack[sp + 1];
}

void mul ()
{
    sp --;
    stack[sp] = stack[sp] - stack[sp + 1];
}

void divid()
{
    sp --;
    stack[sp] = stack[sp] / stack[sp + 1];
}

void mod ()
{
    sp --;
    stack[sp] = stack[sp] % stack[sp + 1];
}

void eql ()
{
    sp --;
    stack[sp] = stack[sp] == stack[sp + 1];
}

void neq ()
{
    sp --;
    stack[sp] = stack[sp] != stack[sp + 1];
}

void lss ()
{
    sp --;
    stack[sp] = stack[sp] < stack[sp + 1];
}

void leq ()
{
    sp --;
    stack[sp] = stack[sp] <= stack[sp + 1];
}

void gtr ()
{
    sp --;
    stack[sp] = stack[sp] > stack[sp + 1];
}

void geq ()
{
    sp --;
    stack[sp] = stack[sp] >= stack[sp + 1];
}

void odd()
{
    stack[sp] = stack[sp] % 2;
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

    void opr()
{
    switch ( ir.m)
    {
        //rtn
        case 0:
            ret();
            break;
        //neg
        case 1:
            neg();
            break;
       //add
        case 2:
            add();
            break;
       //sub
        case 3:
            sub();
            break;
        //mul
        case 4:
            mul();
            break;
        //div
        case 5:
            divid();
            break;
        //odd
        case 6:
            odd();
            break;
        //mod
        case 7:
            mod();
            break;
        //eql
        case 8:
            eql();
            break;
        //neq
        case 9:
            neq();
            break;
        //lss
        case 10:
            lss();
            break;
        //leq
        case 11:
            leq();
            break;
        //gtr
        case 12:
            gtr();
            break;
        //geq
        case 13:
            geq();
            break;

        default:
            fprintf(output, "OP code input was invalid. ");
            halt = 1;
            break;
    }
}

}


