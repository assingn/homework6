/** 
 * postfix.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */

#include<stdio.h> //표준입출력을 담당하는 <stdio.h> 헤더파일 선언
#include<stdlib.h> //표준 라이브러리 함수를 담당하는 <stdlib.h> 헤더파일 선언
#include<string.h> //문자열 처리를 담당하는 <string.h> 헤더파일 선언

#define MAX_STACK_SIZE 10 //스택의 최대 크기를 10으로 정의
#define MAX_EXPRESSION_SIZE 20 //수식의 최대 크기를 20으로 정의

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum{ //enum형 precedence 선언
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];	/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */

int evalResult = 0;	   /* 계산 결과를 저장 */

void postfixpush(char x); //postfixpush함수 원형 선언
char postfixPop(); //postfixPop함수 원형 선언
void evalPush(int x); //evalPush함수 원형 선언
int evalPop(); //evalPop함수 원형 선언
void getInfix(); //getInfix함수 원형 선언
precedence getToken(char symbol); //getToken함수 원형 선언
precedence getPriority(char x); //getPriority함수 원형 선언
void charCat(char* c); //charCat함수 원형 선언
void toPostfix(); //toPostfix함수 원형 선언
void debug(); //debug함수 원형 선언
void reset(); //reset함수 원형 선언
void evaluation(); //evaluation함수 원형 선언

int main() //main함수 시작
{
    printf("[----- [배정민] [2023041088] -----]\n");

	char command; //char형 command변수 선언

	do{ //do-while문 시작
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); //command값 입력받기

		switch(command) { //command값에 따라 case 조건문 실행
		case 'i': case 'I':
			getInfix();
			break;
		case 'p': case 'P':
			toPostfix();
			break;
		case 'e': case 'E':
			evaluation();
			break;
		case 'd': case 'D':
			debug();
			break;
		case 'r': case 'R':
			reset();
			break;
		case 'q': case 'Q':
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q'); //command값이 q나 Q가 아닐때까지 반복

	return 1;
}

void postfixPush(char x) //void형 postfixPush함수 정의
{
    postfixStack[++postfixStackTop] = x; //postfixStackTop을 1증가시키고 x값을 postfixStack에 대입
}

char postfixPop() //char형 postfixPop함수 정의
{
    char x; //char형 x변수 선언
    if(postfixStackTop == -1) //postfixStackTop이 -1일때
        return '\0'; //NULL반환
    else { 	//postfixStackTop이 -1이 아닐때
    	x = postfixStack[postfixStackTop--]; //x에 postfixStack[postfixStackTop]대입 후 postfixStackTop을 1감소
    }
    return x; //x반환
}

void evalPush(int x) //void형 evalPush함수 정의
{
    evalStack[++evalStackTop] = x; //evalStackTop을 1증가시키고 x값을 evalStack에 대입
}

int evalPop() //int형 evalPop함수 정의
{
    if(evalStackTop == -1) //evalStackTop이 -1일때
        return -1; // -1반환
    else //evalStackTop이 -1이 아닐때
        return evalStack[evalStackTop--]; //evalStack[evalStackTop]반환 후 evalStackTop을 1감소
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix() //void형 getInfix함수 정의
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp); //infixExp값 입력받기
}

precedence getToken(char symbol) //precedence형 getToken함수 정의
{
	switch(symbol) { //symbol값에 따라 case 조건문 실행
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand;
	}
}

precedence getPriority(char x) //precedence형 getPriority함수 정의
{
	return getToken(x); //getToken(x)값 반환
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c) //void형 charCat함수 정의
{
	if (postfixExp == '\0') //postfixExp가 NULL이면
		strncpy(postfixExp, c, 1); //postfixExp에 c값을 1만큼 복사
	else
		strncat(postfixExp, c, 1); //postfixExp에 c값을 1만큼 이어붙이기
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix() //void형 toPostfix함수 정의
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
    char *exp = infixExp; 
    char x; /* 문자하나를 임시로 저장하기 위한 변수 */

    while (*exp != '\0') // NULL 문자를 만날 때까지 반복
    {
        if (getToken(*exp) == operand) // 피연산자인 경우
        {
            charCat(exp); // postfixExp에 피연산자 추가
        }
        else if (getToken(*exp) == rparen) // 오른쪽 괄호인 경우
        {
            while (postfixStack[postfixStackTop] != '(') // 왼쪽 괄호를 만날 때까지
            {
                charCat(&postfixStack[postfixStackTop]); // 연산자를 postfixExp에 추가
                postfixStackTop--; // 스택 top을 감소
            }
            postfixStackTop--; // 스택 top을 감소
        }
        else if (getToken(*exp) == lparen) // 왼쪽 괄호인 경우
        {
            postfixPush(*exp); // 스택에 추가
        }
        else // 연산자인 경우
        {
            while (postfixStackTop != -1 && getPriority(*exp) <= getPriority(postfixStack[postfixStackTop])) // 스택이 비어있지 않고, 현재 연산자의 우선순위가 스택의 top 연산자보다 낮거나 같은 경우
            {
                charCat(&postfixStack[postfixStackTop]); // 스택의 top 연산자를 postfixExp에 추가
                postfixStack[postfixStackTop] = '\0'; // 스택의 top을 초기화
                postfixStackTop--; // 스택 top을 감소
            }
            postfixPush(*exp); // 현재 연산자를 스택에 추가
        }
        exp++; // 다음 문자로 이동
    }

    while (postfixStackTop != -1) // 스택에 남은 모든 연산자를 postfixExp에 추가
    {
        x = postfixPop(); // 스택에서 연산자를 꺼내옴
        charCat(&x); // postfixExp에 추가
    }
}
void debug() //void형 debug함수 정의
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++) //i가 0부터 MAX_STACK_SIZE보다 작을때까지 반복문 실행
		printf("%c  ", postfixStack[i]); //postfixStack[i]값 출력

	printf("\n");

}

void reset() //void형 reset함수 정의
{
	infixExp[0] = '\0'; //infixExp[0]에 NULL대입
	postfixExp[0] = '\0'; //postfixExp[0]에 NULL대입

	for(int i = 0; i < MAX_STACK_SIZE; i++) //i가 0부터 MAX_STACK_SIZE보다 작을때까지 반복문 실행
		postfixStack[i] = '\0'; //postfixStack[i]에 NULL대입
         
	postfixStackTop = -1; //postfixStackTop에 -1대입
	evalStackTop = -1; //evalStackTop에 -1대입
	evalResult = 0; //evalResult에 0대입
}

void evaluation() //void형 evaluation함수 정의
{
	/* postfixExp, evalStack을 이용한 계산 */
	int OPR1, OPR2, i; //int형 OPR1, OPR2, i변수 선언
	int length = strlen(postfixExp); //postfixExp의 길이를 length에 대입 
    char symbol; //char형 symbol변수 선언
	evalStackTop = -1; // evalStackTop을 -1로 초기화
    
	for (i = 0; i < length; i++) // i가 0부터 length보다 작을때까지 반복문 실행
    {
        symbol = postfixExp[i]; // symbol에 postfixExp[i]값 대입
        if (getToken(symbol) == operand) // symbol이 피연산자인 경우
        {
            evalPush(symbol - '0'); // 스택에 push
        }
        else // 연산자인 경우
        {
            OPR2 = evalPop(); // 스택에서 pop
            OPR1 = evalPop(); // 스택에서 pop
            switch (getToken(symbol))
            {
            case plus:                 // 더하기 연산
                evalPush(OPR1 + OPR2); // evalpush함수 호출
                break;
            case minus:                // 빼기 연산
                evalPush(OPR1 - OPR2); // evalpush함수 호출
                break;
            case times:                // 곱하기 연산
                evalPush(OPR1 * OPR2); // evalpush함수 호출
                break;
            case divide:               // 나누기 연산
                evalPush(OPR1 / OPR2); // evalpush함수 호출
                break;
            default:
                break;
            }
        }
    }
    evalResult = evalPop(); // 계산 결과를 스택에서 pop
}

