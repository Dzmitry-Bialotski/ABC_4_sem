#include <stdio.h>
#include <stdbool.h>

bool Summator(bool a, bool b, bool* C)
{
    bool sum = (*C)^(a^b);
    *C = (a & b) | ((*C) & (a^b)); 
    return sum;
}

int Addition(int A, int B)
{
    bool C = 0;
    bool bit1, bit2;
    int result = 0, i = 0;
    for( i = 0; i <= 31; i++)
    {
        bit1 = A & (1<<i);
        bit2 = B & (1<<i);
        result |= (Summator(bit1,bit2,&C)<<i);
    }
    return result;
}

int change_sign(int a)
{
    if(a & (1<<31)){
        a=~a;
        a+=1;
        return a;
    }
    else {
        a-=1;
        a=~a;
        return a;
    }
}

int Substraction(int a,int b){
    b=change_sign(b);
    int s=Addition(a,b);
    return s;
}
void print_binary(int a){
        int i=30;
    if((a&(1<<31))<0)
        printf("1");
    else{
        printf("0");
    }
    for(; i >= 0; i--)
    {
        printf("%i",(a & (1<<i)) > 0);
    }
}

void print_step(int A,int Q,int Q_0,int M){
    A=Addition(A,M);
    printf("\n A: ");
    print_binary(A);
    printf("  Q: ");
    print_binary(Q);
    printf(" Q_0: %i",Q_0);
    printf("  M:=");
    print_binary(M);
}

long  multiplication(int a,int b)
{
    long ANS = 0;
    int A = 0;
    int Q_0 = 0;
    int Q = b;
    int Steps = 32;
    int neg_a=change_sign(a);
    while(Steps > 0){
        switch ((Q & 1)*2+(Q_0)) {
            case 2:
                A = Addition(A,neg_a);
                print_step(A,Q,Q_0,neg_a);
                printf("\t (10)");
                break;
            case 1:
                A = Addition(A,a);
                print_step(A,Q,Q_0,a);
                printf("\t (01)");
                break;
        }
        Q_0=Q & 1;
        Q = Q >> 1;
        if (Q < 0)
            Q = (Q^(1<<31)) | (( A&1 )<<31);
        else
            Q=Q | (( A&1 )<<31);
        A=(A>>1)|(A &(1<<31));
        Steps--;
        print_step(A,Q,Q_0,0);
        printf("\n---------------------------------------------------------------------------------------");
    }
    long long BigA = A;
    long long BigQ = Q;
    ANS = ( BigA << 32) | BigQ;
    return ANS;
}
int main()
{
    int a, b;
    printf("Input 2 integer numbers");
    printf("\n a = ");
    scanf("%i", &a);
    printf(" b = ");
    scanf("%i", &b);

    printf("\n Sum = %d\n",Addition(a,b));
    print_binary(Addition(a,b));
    printf("\n Sub = %d\n",Substraction(a,b));
    print_binary(Substraction(a,b));
    printf("\n Mult=%ld",multiplication(a,b));
    scanf("%d",&b);
    return 0;

}