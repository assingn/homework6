/**
 * circularQ.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */

#include <stdio.h>  //표준입출력을 담당하는 <stdio.h> 헤더파일 선언
#include <stdlib.h> //표준 라이브러리 함수를 담당하는 <stdlib.h> 헤더파일 선언

#define MAX_QUEUE_SIZE 4 //큐의 최대 크기를 4로 정의

typedef char element; //char형을 element로 선언
typedef struct { //구조체 QueueType 선언
	element queue[MAX_QUEUE_SIZE]; //char형 큐 배열 선언
	int front, rear; //int형 front와 rear 변수 선언
}QueueType;


QueueType *createQueue(); //QueueType형 포인터 createQueue함수 선언
int freeQueue(QueueType *cQ); //int형 freeQueue함수 원형 선언
int isEmpty(QueueType *cQ); //int형 isEmpty함수 원형 선언
int isFull(QueueType *cQ); //int형 isFull함수 원형 선언
void enQueue(QueueType *cQ, element item); //void형 enQueue함수 원형 선언
void deQueue(QueueType *cQ, element* item); //void형 deQueue함수 원형 선언
void printQ(QueueType *cQ); //void형 printQ함수 원형 선언
void debugQ(QueueType *cQ); //void형 debugQ함수 원형 선언
element getElement(); //char형 getElement함수 원형 선언

int main(void) //main함수 시작
{
	printf("[----- [배정민] [2023041088] -----]\n");

	QueueType *cQ = createQueue(); //QueueType형 포인터 cQ에 createQueue함수 대입
	element data; //char형 data변수 선언
	char command; //char형 command변수 선언

	do{ //do-while문 시작
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); //command값 입력받기

		switch(command) { //command값에 따라 case 조건문 실행
		case 'i': case 'I':
			data = getElement();
			enQueue(cQ, data);
			break;
		case 'd': case 'D':
			deQueue(cQ, &data);
			break;
		case 'p': case 'P':
			printQ(cQ);
			break;
		case 'b': case 'B':
			debugQ(cQ);
			break;
		case 'q': case 'Q':
   	        freeQueue(cQ);
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q'); //command값이 q나 Q가 아닐때까지 반복


	return 1; //main함수 종료
}

QueueType *createQueue() //QueueType형 포인터 createQueue함수 정의
{
	QueueType *cQ; //QueueType형 포인터 cQ선언
	cQ = (QueueType *)malloc(sizeof(QueueType)); //cQ에 QueueType크기만큼 동적할당
	cQ->front = 0; //cQ의 front에 0대입
	cQ->rear = 0; //cQ의 rear에 0대입
	return cQ; //cQ반환
}

int freeQueue(QueueType *cQ) //int형 freeQueue함수 정의
{
    if(cQ == NULL) return 1; //cQ가 NULL이면 1반환
    free(cQ); //cQ메모리 해제
    return 1;
}

element getElement() //char형 getElement함수 정의
{
	element item; //char형 item변수 선언
	printf("Input element = "); 
	scanf(" %c", &item); //item값 입력받기
	return item; //item 값 반환
}


/* complete the function */
int isEmpty(QueueType *cQ) //int형 isEmpty함수 정의
{
	if(cQ->front == cQ->rear) //cQ의 front와 rear가 같으면
	{
		printf("Queue is empty\n"); //Queue is empty 출력
		return 1;
	}
	else{ //아니면
		return 0;
	}
}

/* complete the function */
int isFull(QueueType *cQ) //int형 isFull함수 정의
{
	if(((cQ->rear+1)%MAX_QUEUE_SIZE) == cQ->front) //cQ의 rear+1을 MAX_QUEUE_SIZE로 나눈 나머지가 cQ의 front와 같으면
	{
		printf("Queue is full\n"); //Queue is full 출력
		return 1;
	}
	else{ //아니면
		return 0;
	}
}


/* complete the function */
void enQueue(QueueType *cQ, element item) //void형 enQueue함수 정의
{
	if(isFull(cQ)) //isFull함수가 1이면
	{
		return; //함수 종료
	}
	else{ //아니면
		cQ->rear = ((cQ->rear+1)%MAX_QUEUE_SIZE); //cQ의 rear에 rear+1을 MAX_QUEUE_SIZE로 나눈 나머지 대입
		cQ->queue[cQ->rear] = item; //cQ의 queue배열의 rear 위치에 item값 대입
		return; //함수 종료
	}
}

/* complete the function */
void deQueue(QueueType *cQ, element *item) //void형 deQueue함수 정의
{
	if(isEmpty(cQ)) //isEmpty함수가 1이면
	{
		return; //함수 종료
	}
	else //아니면
	{
		cQ->front = ((cQ->front+1)%MAX_QUEUE_SIZE); //cQ의 front에 front+1을 MAX_QUEUE_SIZE로 나눈 나머지 대입
		*item = cQ->queue[cQ->front]; //item에 cQ의 queue배열의 front번 위치의 값 대입
		return; //함수 종료
	} 
}


void printQ(QueueType *cQ) //void형 printQ함수 정의
{
	int i, first, last; //int형 i, first, last변수 선언

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; //first에 front+1을 MAX_QUEUE_SIZE로 나눈 나머지 대입
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE; //last에 rear+1을 MAX_QUEUE_SIZE로 나눈 나머지 대입

	printf("Circular Queue : [");

	i = first; //i에 first 대입
	while(i != last){ //i가 last와 같지 않을때까지 반복문 실행
		printf("%3c", cQ->queue[i]); //cQ의 queue배열의 i번 위치의 값 출력
		i = (i+1)%MAX_QUEUE_SIZE; //i에 i+1을 MAX_QUEUE_SIZE로 나눈 나머지 대입

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ) //void형 debugQ함수 정의
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++) //i가 0부터 MAX_QUEUE_SIZE보다 작을때까지 반복문 실행
	{
		if(i == cQ->front) { //i가 front와 같으면
			printf("  [%d] = front\n", i); //front 출력
			continue; //다음으로 이동
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]); //i와 cQ의 queue배열의 i번 위치의 값 출력

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); //front와 rear값 출력
}


