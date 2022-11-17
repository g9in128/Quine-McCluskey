#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<Windows.h>

#define YES_MARK 'o'
#define NO_MARK 'x'

typedef struct Node {																	//노드 구조체
	int* group;
	int size, ones;
	char check;
	char* bin;
	struct Node* next;
} Node;

Node* column = NULL;																	//무관항이 아닌 항의 리스트 

Node* choice(int variables);															//입력함수 
int compare(Node* a, Node* b);															//노드비교함수 
int containsNode(Node* arr, int data);													//노드에 포함되는지 비교
Node* copyNode(Node* node);																//노드복사함수 
int countOne(Node* node);																//1수세기함수 
int deleteColumn(Node** pi, int row, Node** c1, int column, int** table);				//세로제거함수 
Node* deleteNode(Node* head, char* ch);													//리스트에서 노드제거함수 
Node* extractEPI(Node* pi);																//EPI추출함수 
Node* extractPI(Node* list, Node* pi);													//PI추출함수 
Node* freeNode(Node* list);																//리스트할당해제함수 
Node* getNode(int num, int size);														//노드생성함수 
Node* insertNode(Node* head, Node* data);												//리스트에 노드추가함수 
int lengthNode(Node* list);																//리스트 길이 반환함수 
Node* merge(Node* a, Node* b);															//두 항을 결합하는 함수 
Node* mergeAll(Node* c1);																//리스트를 비교해서 결합하는함수 
int orderBin(char ch);																	//노드 비교할때 '0','1','-' 를 비교하는함수 
void printEpiTable(Node** pi, int row, Node** c1, int column, int** table, Node* epi);	//EPI테이블 출력함수 
void printList(Node* list);																//리스트 출력함수 
void printList2(Node* list);															//리스트 출력함수2 
Node** toArr(Node* list, int size);														//리스트를 배열로 바꾸는함수 
char* toBin(int num, int size);															//2진수 변환 함수	

int main() {
	int variables, input;
	do {
		printf("**변수의 개수를 입력하십시오 (2 < 변수 개수 < 10): ");			//변수 개수 입력
		input = scanf("%d", &variables);
		getchar();
		system("cls");
	} while (input != 1 || variables > 10 || variables <= 2);					//유효성검사
	 
	Node* c1 = choice(variables); 												//처음 항 입력 
	Node* c2 = NULL;
	Node* pi = NULL;
	system("cls");
	int i = 0;
	do {
		printf("\ncolumn %d\n\n", i++);
		c2 = mergeAll(c1);
		pi = extractPI(c1, pi);
		printList2(c1);
		system("pause");
		system("cls");
		freeNode(c1);
		c1 = c2;
		c2 = NULL;
	} while (c1 != NULL);														//비교하면서 PI추출 
	system("cls");
	printf("\nPI\n\n");
	printList(pi);
	system("pause");
	system("cls");
	
	Node* epi = extractEPI(pi);													//PI를 배열로 바꿔서 EPI추출함수에 넣음 
	freeNode(pi);
	printf("\n\nEPI\n\n");
	printList(epi);
	
	freeNode(epi);
	freeNode(column);
	printf("프로그램을 종료합니다.....\n");
}

Node* getNode(int num, int size) {
	Node* node = (Node*)malloc(sizeof(Node));
	node->group = (int*)malloc(sizeof(int));
	node->group[0] = num;
	node->size = 1;
	node->bin = toBin(num, size);
	node->ones = countOne(node);
	node->next = NULL;
	node->check = NO_MARK;
	return node;
}

Node* insertNode(Node* head, Node* data) {
	Node* node = head;
	if (node == NULL) {
		return data;
	}
	int comp = compare(data, node);
	if (comp < 0) {
		data->next = node;
		return data;
	}
	if (comp == 0) return head;
	while (node->next != NULL) {
		comp = compare(data, node->next);
		if (comp == 0) return head;

		if (comp < 0) break;
		node = node->next;
	}
	Node* tmp = node->next;
	node->next = data;
	node->next->next = tmp;
	return head;
}

Node* deleteNode(Node* head, char* ch) {
	Node* node = head;
	if (node == NULL) {
		return NULL;
	}
	if (strcmp(node->bin, ch) == 0) {
		node = node->next;
		free(head);
		return node;
	}
	while (node != NULL && node->next != NULL) {
		if (strcmp(node->next->bin, ch) == 0) {
			Node* tmp = node->next;
			node->next = node->next->next;
			free(tmp);
		}
		node = node->next;
	}
	return head;
}

Node* freeNode(Node* list) {
	if (list == NULL) return NULL;
	Node* tmp = list->next;
	free(list->bin);
	free(list->group);
	free(list);
	return freeNode(tmp);
}

Node* copyNode(Node* node) {
	Node* cpy = (Node*)malloc(sizeof(Node));
	cpy->bin = (char*)malloc(strlen(node->bin) + 1);
	strcpy(cpy->bin, node->bin);
	cpy->check = NO_MARK;
	cpy->group = (int*)malloc(sizeof(int) * node->size);
	for (int i = 0; i < node->size; i++) {
		cpy->group[i] = node->group[i];
	}
	cpy->ones = node->ones;
	cpy->size = node->size;
	cpy->next = NULL;
	return cpy;
}

int lengthNode(Node* list) {
	int length = 0;
	Node* node = list;
	while (node != NULL) {
		length++;
		node = node->next;
	}
	return length;
}

int containsNode(Node* arr, int data) {
	Node* node = arr;
	while (node != NULL) {
		for (int i = 0; i < node->size; i++) {
			if (node->group[i] == data) return 1;
		}
		node = node->next;
	}
	return 0;
}

Node** toArr(Node* list, int size) {
	if (list == NULL) return NULL;
	int i = 0;
	Node** arr = (Node**)malloc(sizeof(Node*) * size);
	Node* node = list;
	while (node != NULL) {
		arr[i++] = copyNode(node);
		node = node->next;
	}
	return arr;
}

int compare(Node* a, Node* b) {						
	if (a->size < b->size) return 1;							//크기 비교 
	else if (a->size > b->size) return -1;

	if (a->ones > b->ones) return 1;							// 1 개수 비교
	else if (a->ones < b->ones) return -1;

	char ch1, ch2;
	int i = 0;
	do {
		ch1 = a->bin[i];
		ch2 = b->bin[i++];
		if (orderBin(ch1) != orderBin(ch2)) 
			return orderBin(ch1) > orderBin(ch2) ? 1 : -1;
	} while (ch1 != '\0' && ch2 != '\0');

	return 0;
}

int orderBin(char ch) {
	switch (ch) {
	case '0':
		return 0;
	case '-':
		return 1;
	case '1':
		return 2;
	default:
		return ch + 2;
	}
}

char* toBin(int num, int size) {				
	int tmp = num;
	char* ch = (char*)malloc(size + 1);
	ch[size] = '\0';
	for (int i = size - 1; i >= 0; i--) {
		ch[i] = '0' + (tmp & 1);
		tmp = tmp >> 1;
	}
	return ch;
}

int countOne(Node* node) {				
	int i = 0, cnt = 0;
	char ch = node->bin[i];
	while (ch != '\0') {
		cnt += ch == '1' ? 1 : 0;
		ch = node->bin[++i];
	}
	return cnt;
}

Node* merge(Node* a, Node* b) {						
	if (abs(a->ones - (b->ones)) != 1) {								//1차이 아닐떄 리턴
		return NULL;
	}
	int length = strlen(a->bin);
	char* tmp = (char*)malloc(length + 1);
	strcpy(tmp, a->bin);												//a 노드의 2진수 부분을 복사
	Node* node = NULL;
	for (int i = 0; i < length; i++) {
		if (tmp[i] == '0') {											// 0 하나를 1로 바꿈
			tmp[i] = '1';
		}
		else {
			continue;
		}
		if (strcmp(tmp, b->bin) == 0) {									// 바꿔서 비교했을때 같으면
			node = (Node*)malloc(sizeof(Node));
			node->size = a->size + b->size;
			node->ones = min(a->ones, b->ones);
			node->group = (int*)malloc(sizeof(int) * node->size);
			node->next = NULL;
			node->check = NO_MARK;
			for (int j = 0; j < a->size; j++) {
				node->group[j] = a->group[j];
			}
			for (int j = a->size; j < node->size; j++) {
				node->group[j] = b->group[j - (a->size)];
			}															//두개를 합치고
			tmp[i] = '-';												//겹치는 부분을 '-'로 바꿈
			node->bin = tmp;
			break;
		}
		else {															//아니면 원상복구
			tmp[i] = '0';
		}
	}
	return node;
}

Node* mergeAll(Node* c1) {
	Node* node = c1;
	Node* c2 = NULL;
	while (node != NULL) {
		Node* cpy = node->next;
		while (cpy != NULL && cpy->ones - node->ones <= 1) {
			Node* m = merge(node, cpy);
			if (m != NULL) {
				c2 = insertNode(c2, m);
				node->check = YES_MARK;
				cpy->check = YES_MARK;
			}
			cpy = cpy->next;
		}
		node = node->next;
	}
	return c2;
}

Node* extractPI(Node* list, Node* pi) {
	Node* node = list;
	while (node != NULL) {
		if (node->check == NO_MARK) {
			pi = insertNode(pi, copyNode(node));
		}
		node = node->next;
	}
	return pi;
}

Node* extractEPI(Node* pi) {
	if (pi == NULL) return NULL;											//비어있을때
	
	int pisize = lengthNode(pi),colsize = lengthNode(column);
	Node** piA = toArr(pi, pisize),** colA = toArr(column,colsize);
	int** table = (int**)malloc(sizeof(int*) * colsize);					//epi 테이블 생성 
	
	for (int i = 0; i < colsize; i++) {
		table[i] = (int*)malloc(sizeof(int) * pisize);
		for (int j = 0; j < pisize; j++) {
			table[i][j] = 0;
			for (int k = 0; k < piA[j]->size; k++) {
				if (piA[j]->group[k] == colA[i]->group[0]) {
					table[i][j] = 1;
					break;
				}
			}
		}
	}
	
	
	Node* epi = NULL;														//return 할 리스트 생성 
	printEpiTable(piA, pisize, colA, colsize, table, epi);
	for (int i = 0; i < colsize; i++) {
		int cnt = 0, idx;
		for (int j = 0; j < pisize; j++) {									//하나만 있는 pi 찾기 
			if (table[i][j] == 1) {
				idx = j;
				cnt++;
			}
		}
		if (cnt == 1) {
			for (int k = 0; k < colsize; k++) {
				if (table[k][idx] != 1)table[k][idx] = -1;
			}
			Node* node = piA[idx];
			node->check = YES_MARK;
			Node* cpy = copyNode(node);
			cpy->check = YES_MARK;
			epi = insertNode(epi, cpy);
		}
	}
	printEpiTable(piA, pisize, colA, colsize, table, epi);
	int deletedCol = deleteColumn(piA, pisize, colA, colsize, table);		//포함되는 값 제거 
	printEpiTable(piA, pisize, colA, colsize, table, epi);
	
	
	while (deletedCol < colsize) {
		int max = 0, idx;
		Node* node = NULL;
		for (int i = 0; i < pisize; i++) {
			if (piA[i]->check == NO_MARK) {
				int cnt = 0;
				for (int j = 0; j < colsize; j++) {
					if (table[j][i] == 1) cnt++;
				}
				if (cnt > max || (cnt == max && cnt != 0 && node->size < piA[i]->size)) {
					max = cnt;
					node = piA[i];
					idx = i;
				}
			}
		}
		if (node != NULL) {
			Node* cpy = copyNode(node);
			cpy->check = YES_MARK;
			epi = insertNode(epi, cpy);
			for (int i = 0; i < colsize; i++) {
				if (table[i][idx] != 1)table[i][idx] = -1;
			}
			node->check = YES_MARK;
		}
		printEpiTable(piA, pisize, colA, colsize, table, epi);
		deletedCol += deleteColumn(piA, pisize, colA, colsize, table);
		printEpiTable(piA, pisize, colA, colsize, table, epi);
	}
	
	
	for (int i = 0;i<colsize;i++) {
		free(colA[i]);
	}
	for (int i = 0;i <pisize;i++) {
		free(piA[i]);
	}
	for (int i = 0; i < colsize; i++) {
		free(table[i]);
	}
	free(table);
	free(colA);
	free(piA);
	return epi;
}

int deleteColumn(Node** pi, int row, Node** c1, int column, int** table) {				
	int deletedCol = 0;
	for (int i = 0; i < row; i++) {
		if (pi[i]->check == YES_MARK) {											//EPI에 포함되어있는 항을 제거 
			for (int j = 0; j < column; j++) {
				if (table[j][i] == 1) {
					for (int k = 0; k < row; k++) {
						table[j][k] = -1;
					}
					deletedCol++;
				}
			}
		}
	}
	return deletedCol;
}

Node* choice(int variables) {
	int input, inn, cnt, menu, max = (int)round(pow(2, variables)) - 1;
	Node* list = NULL,* col = NULL,* dk = NULL;
	while (1) {
		//리스트에 추가할지 제거할지
		do {
			system("cls");
			printList2(list);
			printf("**최소항을 설정하십시오.\n0. 항 삭제\n1. 항추가\n2. 무관항 추가\n:");
			input = scanf("%d", &menu);
			if (input != 1) return list;
		} while (menu > 2 || menu < 0);
		
		//입력
		printf("**최소항을 입력하십시오 (0 <= 항 <= %d) : ", max);
		input = scanf("%d", &inn);
		if (input != 1) {
			getchar();
			continue;
		}
		if (inn > max || inn < 0) continue;		// 유효성 검사
		if (menu > 0) {				//추가
			Node* node = getNode(inn, variables);
			list = insertNode(list, node);
			if (menu == 1) {
				column = insertNode(column,copyNode(node));
			}
		}
		else {					//제거
			char* ch = toBin(inn, variables);
			list = deleteNode(list, ch);
			column = deleteNode(column, ch);
		}
		system("cls");
	}
	return list;
}

void printList(Node* list) {
	if (list == NULL) {
		printf("0\n\n");
		return;
	}
	Node* node = list;
	while (node != NULL) {							//변수명 출력
		int i = 0;
		char ch = node->bin[i];
		if (list->size == (int)round(pow(2, strlen(list->bin)))) printf("1");
		else {
			while (ch != '\0') {
				if (ch == '-') {
					ch = node->bin[++i];
					continue;
				}
				putc('A' + i, stdout);
				if (ch == '0') putc('\'', stdout);
				ch = node->bin[++i];
			}
			if (node->next != NULL) printf(" + ");
		}
		node = node->next;
	}
	printf("\n\n");
	if (list != NULL) {
		putc('{', stdout);
	}
	node = list;
	while (node != NULL) {
		printf(" %s(", node->bin);
		for (int i = 0; i < node->size; i++) {
			printf("%d", node->group[i]);
			if (containsNode(column, node->group[i])) {
				putc(' ', stdout);
			}
			else {
				putc('*', stdout);
			}
			if (i < node->size - 1) putc(',', stdout);
		}
		printf(") %c ", node->check);
		if (node->next != NULL) putc(',', stdout);
		node = node->next;
	}
	if (list != NULL) {
		putc('}', stdout);
	}
	printf("\n\n\n");
}

void printList2(Node* list) {
	if (list == NULL) {
		printf("비어있음\n\n");
		return;
	}
	for (int i = 0; i < 50; i++) {
		putc('+', stdout);
	}
	putc('\n', stdout);
	Node* node = list;
	while (node != NULL) {
		printf("+ %2d + %s(", node->ones, node->bin);

		for (int i = 0; i < node->size; i++) {
			printf("%3d", node->group[i]);
			if (containsNode(column, node->group[i])) {
				putc(' ', stdout);
			}
			else {
				putc('*', stdout);
			}
			if (i < node->size - 1) putc(',', stdout);
		}
		printf(")\t+ ");
		int i = 0;
		char ch = node->bin[i];
		while (ch != '\0') {
			if (ch == '-') {
				ch = node->bin[++i];
				continue;
			}
			putc('A' + i, stdout);
			if (ch == '0') putc('\'', stdout);
			else putc(' ', stdout);
			ch = node->bin[++i];
		}
		for (int i = 0; i < node->size - 1; i++) {
			putc(' ',stdout);
		}
		printf("\t+ %c\n", node->check == YES_MARK ? 'o' : 'x');
		node = node->next;
	}
	for (int i = 0; i < 50; i++) {
		putc('+', stdout);
	}
	printf("\n\n");
}

void printEpiTable(Node** pi, int row, Node** c1, int column, int** table, Node* epi) {
	printf("\nEPI table\n\n");
	int length = strlen(pi[0]->bin);
	for (int i = 0; i < length / 4; i++) {
		putc('\t', stdout);
	}
	for (int i = 0; i < column; i++) {
		printf("%d\t", c1[i]->group[0]);				//처음 항들 표시 
	}
	putc('\n', stdout);
	for (int i = 0; i < row; i++) {
		for (int j = -1; j < column; j++) {
			if (j == -1) {
				printf("%s\t", pi[i]->bin);				//pi 정보 표시 
				continue;
			}								
			switch (table[j][i]) {						//고려하지 않음 = '-' 없음 ' ' 있음 'x' 
			case -1:
				printf("-\t");
				break;
			case 0:
				printf("\t");
				break;
			case 1:
				printf("x\t");
			}
		}
		putc('\n', stdout);
	}
	printf("\nEPI\n\n");
	printList(epi);
	system("pause");
	system("cls");
}

//코드 참조 https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=lghlove0509&logNo=220955178807
//코드 참조 https://gist.github.com/steelbear/ad1752922f46dbab72f81b95f41bafa7
//코드 참조 https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=tb_elec_engineer&logNo=220983651675 

