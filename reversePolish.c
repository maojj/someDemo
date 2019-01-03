#include "reversePolish.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// method3: https://zh.wikipedia.org/wiki/%E8%B0%83%E5%BA%A6%E5%9C%BA%E7%AE%97%E6%B3%95
// method2: https://blog.csdn.net/QZC295919009/article/details/23799369
// method1: https://blog.csdn.net/bhq2010/article/details/7516369

//运算符栈的长度
#define OPSTACK_LENGTH 50
#define EXP_STACK_LENGTH 50
//操作数栈的长度
#define NUMSTACK_LENGTH 100
//输入串的最大长度
#define MAX_STRING_LENGTH 100

struct ExpNodeStrut
{
    int type; // 0 operator, 1 number
    char op;
    double num;
};

typedef struct ExpNodeStrut ExpNode;

// 后缀表达式， 即逆波兰表达式
ExpNode expQueue[EXP_STACK_LENGTH];
int expQueuekTail = -1;

//运算符栈
char opStack[OPSTACK_LENGTH];
//运算符栈顶指针
int opStackTop = -1;

//获取一个字符所代表的运算符的优先级
int getPriority(char name)
{
    switch(name) {
        case 'f': case 'F': // 分式函数符， f(x,y) = x/y , F(x,y,z) = x+y/z 如三又二分之一, F(3,1,2)
            return 4;
        case '!':
            return 3;
        case '*': case '/':
            return 2;
        case '+': case '-':
            return 1;
        default:
            return 0;
    }
}
//获取一个字符所代表的运算符的目数
int getOpNum(char name)
{
    switch(name) {
        case 'F':
            return 3;
        case 'f':
        case '*': case '/': case '+': case '-':
            return 2;
        default:
            return 0;
    }
}

// 两个数的运算
double opertate2Num(double num1, double num2, char op)
{
    switch(op) 
    {
        case  '+':
            return num1 + num2;
        case '-':
            return num1 - num2;
        case '*':
            return num1 * num2;
        case '/':
        case 'f':
            return num1 / num2;
        default:
            return 0;
    }
}

// 三个数的运算
double opertate3Num(double num1, double num2, double num3, char op)
{
    switch(op) 
    {
        case  'F':
            return num1 + num2 / num3;
        default:
            return 0;
    }
}

//运算符压栈
void pushOperator(char op)
{
    if (opStackTop < OPSTACK_LENGTH - 1)
    {
        opStack[++opStackTop] = op;
    }
    else
    {
        exit(1);
    }
}

// 运算符栈顶元素
char topOperator()
{
    if (opStackTop >= 0)
    {
        return opStack[opStackTop];
    }
    else
    {
        exit(1);
    }
}

//运算符出栈
char popOperator()
{
    char op = topOperator();
    opStack[opStackTop] = 0;
    opStackTop--;
    return op;
}



void putExpNode(ExpNode node) {
    if (expQueuekTail < EXP_STACK_LENGTH - 1)
    {
        expQueue[++expQueuekTail] = node;
    }
    else
    {
        exit(1);
    }
}

//操作数压栈
void saveNumber(double num)
{
    ExpNode node;
    node.type = 1;
    node.op = 0;
    node.num = num;
    putExpNode(node);
}

void saveOperator(char op) 
{
    ExpNode node;
    node.type = 0;
    node.op = op;
    node.num = 0;
    putExpNode(node);
}

//将输入字符串中的以0-9开头、到下一个运算符结束的一段转化为浮点型
//i加上浮点型对应的字符串的长度
double getNumFromString(char *s, int *i)
{
    int j = 0;
    static char numstr[MAX_STRING_LENGTH];
    while ((*s) >= '0' && *s <= '9')
    {
        numstr[j++] = (*s);
        s++;
    }
    if ((*s) == '.')
    {
        numstr[j++] = (*s);
        s++;
        while ((*s) >= '0' && *s <= '9')
        {
            numstr[j++] = (*s);
            s++;
        }
    }
    (*i) = (*i) + j;
    numstr[j] = '\0';
    return atof(numstr);
}

void method1(char input[MAX_STRING_LENGTH])
{
    //char input[MAX_STRING_LENGTH];//表达式的输入串
    int i, priority;
    
    char op, topOp;//op为从当前输入串中提取的一个运算符，topOp为运算符栈栈顶的运算符	
    topOp = '#';
    pushOperator(topOp);//压入#作为初始运算符

    for (i = 0; input[i] != '\0' && input[i] != '=';)
    {
        char c = input[i];

        if(c == ' ' || c == ',') {
            i++;
            continue;
        }

		//从输入串中取出一个字符作为开始，进行处理，直到表达式结束
        if (c >= '0' && c <= '9')
        {
			//如果是操作数，将整个操作数提取出来，输出到结果队列
            double number = getNumFromString(&input[i], &i);
            saveNumber(number);
        }
        else
        {
            op = c;
            priority = getPriority(op);
           
            topOp = topOperator();
            if (op == '(')
            {
				//如果是'('，将当前运算符则压栈
                pushOperator(op);
            }
            else if (op == ')')
            {
				//如果是')'，则把运算符栈顶元素依次弹出，并存入结果队列中，直至第一次遇到左括号'('， 左右括号都会丢掉。
                while (topOp != '(')
                {
                    popOperator();
                    saveOperator(topOp);
                    topOp = topOperator();
                }
                //弹出'(' 丢掉
                popOperator();
            }
            else if(priority > getPriority(topOp))
            {
                // 如果该字符是算术运算符且其优先关系高于运算符栈顶的运算符，则将该运算符入栈。
                pushOperator(op);
            }
            else 
            {
				//如果是普通运算符, 且优先级小于等于栈顶元素运算符。 则将临时栈中
                while (topOp != '#' && priority <= getPriority(topOp))
                {
					//将栈顶的运算符从栈中弹出至结果字符串末尾，直至栈顶运算符的优先级低于当前运算符，并将该字符入栈
                    popOperator();
                    saveOperator(topOp);
                    topOp = topOperator();
                }
				//将当前运算符压栈
                pushOperator(op);
            }
            i++;
        }
    }

    topOp = popOperator();
	//完成栈内剩余的运算
    while (topOp != '#')
    {
        saveOperator(topOp);
        topOp = popOperator();
    } 

    printf("\nintput: %s", input);

    printf("\r\nreverse polish:");
    for(int i = 0; i <= expQueuekTail; i++ ) {
        ExpNode node = expQueue[i];
        if(node.type == 0) {
            printf("%c ", node.op);
        } else {
            printf("%d ", (int)node.num);
        }
    }
    
    printf("\r\n");

}

double envalueReversePolish(){
    double resultQueue[EXP_STACK_LENGTH];
    int resultQueueTail = -1;

    for(int i = 0; i<= expQueuekTail; i++) {
        ExpNode node = expQueue[i];
        if(node.type == 1) {
            resultQueue[++resultQueueTail] = node.num;
        } else {
            int count = getOpNum(node.op);
            if(count == 3) {
                double number3 = resultQueue[resultQueueTail--];
                double number2 = resultQueue[resultQueueTail--];
                double number1 = resultQueue[resultQueueTail--];
                resultQueue[++resultQueueTail] = opertate3Num(number1, number2, number3, node.op);
            } else if(count == 2) {
                double number2 = resultQueue[resultQueueTail--];
                double number1 = resultQueue[resultQueueTail--];
            
                resultQueue[++resultQueueTail] = opertate2Num(number1, number2, node.op);
            }
            // 暂时不支持单目运算符
        }
    }

    printf("\n result = %f", resultQueue[0]);

    return resultQueue[0];
}


void parseReversePolish(char inputString[INPUT_MAX_LENGTH])
{
    char input[INPUT_MAX_LENGTH];
    
    sprintf(input, "1-F(1,1,2)+F(3,F(3,1,2),f(1,4))*f(1,2)");
    method1(input);
    double result = envalueReversePolish();
    getchar();
}
