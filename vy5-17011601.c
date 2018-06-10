//
//  main.c
//  VyProje
//
//  Created by İpek Uyanık on 9.06.2018.
//  Copyright © 2018 İpek Uyanık. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#define SIZE 100

// begining of stack operations
typedef struct STACK{
    int top;
    int st[SIZE];
}Stack;

void initialize(Stack *stack){
    stack->top=-1;
}

int isEmpty(Stack *stack){
    if(stack->top == -1)
        return 1;
    else
        return 0;
}

int isFull(Stack *stack){
    if(stack->top==SIZE)
        return 1;
    else
        return 0;
}

void push(Stack *stack, int value){
    if(isFull(stack))
        printf("Stack is full!\n");
    else{
        stack->st[++stack->top]=value;
    }
}

int pop(Stack *stack){
    if(isEmpty(stack)){
        printf("Stack is empty!\n");
        return 0;
    }
    else{
        return stack->st[stack->top--];
    }
}
char top(Stack *stack)
{
    return stack->st[stack->top];
}

//end of stack operations

bool isOperator(char op){
    return (op=='+' || op=='-' || op=='*' || op=='^'|| op=='/' || op=='(' || op==')' );
}

//function to print given stack
void print_stack(Stack *stack){
    int i;
    printf("Stack:");
    if(!isEmpty(stack))
        for(i=0; i<=stack->top;i++)
            printf("%c", stack->st[i]);
    else
        printf("Empty");
    printf("\n");
}


int precedence(char op)
{
    if(op=='(')
        return(0);
    if(op=='+'||op=='-')
        return(1);
    if(op=='*'||op=='/'||op=='%')
        return(2);
    if(op=='^')
        return 3;
    
    return(-1);
}
//function to evaluate given arithmetic operation
int operations(int a,int b ,char c){
    
    switch (c)
    {
        case '+':return a+b;
        case '-':return a-b;
        case '*':return a*b;
        case '/':return a/b;
        default:return 0;
    }
 
}

void infix_to_postfix(Stack *stack, char infix[],char postfix[])
{
    char x,token;
    int i,j;    //i-index of infix,j-index of postfix
    initialize(stack);
    j=0;
    
    for(i=0;infix[i]!='\0';i++){
        token=infix[i];
        // is current text is opeator
        if(!isOperator(token))
            postfix[j++]=token;
        // open parenthesis case
        else if(token=='(')
            push(stack,'(');
        // close parenthesis case
        else if(token==')')
            
            while((x=pop(stack))!='(')
                postfix[j++]=x;
        //evaluate the expression until encounter to a parenthesis
        else{
            
            while(!isEmpty(stack) && precedence(token)<=precedence(top(stack))){
                
                x=pop(stack);
                postfix[j++]=x;
                
            }
            push(stack,token);
        }
    }
    // pop until there is no higher priority operator than this
    while(!isEmpty(stack))
        postfix[j++]=pop(stack);
    
    postfix[j]='\0';
}

int solv_operations(Stack *stack, char *postfix,int count,int *sayilar){
    int i=0;
    int x,y;
    while(i<count){
        if(!isOperator(postfix[i])){
            if(postfix[i]>='a' && postfix[i]<='z')
                push(stack,sayilar[postfix[i]-'a']);
            else
                push(stack,postfix[i]-'0');
            
        }else{
            y=pop(stack);
            x=pop(stack);
            x=operations(x,y,postfix[i]);
            push(stack,x);
        }
        i++;
        
    }
    return pop(stack);
}




int main(){
    
    int i,j;
    Stack *stack;
    
    stack= (Stack*)malloc(sizeof(Stack));
    
    char infix[30],postfix[30];
    
    
    // input file
    FILE *fp = fopen("input.txt","r");
    
    int *variables =(int*)malloc(20*sizeof(int));
    char** data=(char**)calloc(50,sizeof(char*));
    for(int i=0;i<55;i++)
        data[i]=(char*)calloc(50,sizeof(char));
    char* line=(char*)calloc(50,sizeof(char));
    char* ptr=(char*)malloc(50*sizeof(char));
    int size=0;
    
    while(!feof(fp)){
        fgets(line,50,fp);
        ptr=strtok(line,", ");
        data[size][0]=ptr[0];
        j=1;
        
        ptr=strtok(NULL,", ");
        while(ptr[0]!=';'){
            data[size][j]=ptr[0];
            j++;
            ptr=strtok(NULL,", ");
        }
        if(j==3){
            variables[data[size][0]-'a']=data[size][2]-'0';
            data[size][55]='f';
        }
        size++;
    }
    fclose(fp);
    
    infix_to_postfix(stack,infix,postfix);
    printf("\nPostfix expression: %s \n", postfix);
    
    solv_operations(stack, postfix, size , variables);
    
    print_stack(stack);
    return 0;
}

