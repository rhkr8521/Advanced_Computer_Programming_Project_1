#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <string.h>

#define MAX_WORDS 100	// 최대 단어수
#define MAX_LIVES 3		// 목숨 개수

// 단어 배열
char* words[MAX_WORDS] = {
		"사과", "바나나", "당근", "오렌지", "수박",
		"딸기", "포도", "레몬", "라임", "체리",
		"키위", "파인애플", "복숭아", "배", "자두",
		"멜론", "파파야", "블루베리", "무화과", "아보카도",
		"귤", "참외", "레드향", "망고", "크랜베리",
		"레몬", "크렌베리", "낑깡", "두리안", "레이크",
		"포테이토", "소세지", "파프리카", "허니", "자몽",
		"감귤", "감", "포도", "바이올린", "귤",
		"거미", "단감", "토마토", "케일", "누룽지",
		"콩", "밥", "야채", "과일", "식사",
		"샌드위치", "피자", "파스타", "떡볶이", "케이크",
		"쿠키", "치즈", "밀크쉐이크", "아이스크림", "컵케이크",
		"커피", "차", "주스", "맥주", "와인",
		"소주", "막걸리", "칵테일", "사이다", "레모네이드",
		"스무디", "에이드", "콜라", "탄산수", "음료수",
		"수정과", "한과", "찹쌀떡", "씨앗호떡", "호떡",
		"꿀떡", "인절미", "송편", "메론빵", "팥빵",
		"크림빵", "호빵", "케이크", "초코렛", "마카롱",
		"마카다미아", "헤이즐넛", "피칸", "아몬드", "호박씨",
		"해바라기씨", "밤", "잣", "코코넛", "피스타치오",
};

// 떨어질 단어에 대한 구조체
typedef struct {
	int x;	// 떨어질 단어의 x좌표 (창의 최상단 내)
	char word[20];	// 떨어질 단어
} rain;

rain rains[21];
int lives = MAX_LIVES;
char buffer[20];

void initRains() {
	for (int i = 0; i < 21; i++) {
		rains[i].x = 0;
		strcpy(rains[i].word, " ");
	}
}

// word를 랜덤 x에 생성, 기존 단어들을 한 줄씩 아래로
void makeRain() {

	// y좌표 관련이 X, 새로 출력하여 한 줄씩 내리는 것
	for (int i = 20; i >= 0; i--) {	// i가 위아래 입력 (y좌표) 말하는 것
		strcpy(rains[i].word, rains[i - 1].word);	// 한 줄 내릴 단어 복사
		rains[i].x = rains[i - 1].x;	// 같은 위치로
	}

	rains[0].x = rand() % 53;	// 랜덤으로 떨어질 단어의 x좌표 설정
	strcpy(rains[0].word, words[rand() % MAX_WORDS]); // 새 단어를 맨 위에 추가
}

void printScreen() {

	system("cls");	// 화면을 지우고

	for (int i = 0; i < 20; i++) {
		printf("%*s\n", rains[i].x, rains[i].word);
		// 형식(%*s) : 넓이, 표시한 문자열
	}
	printf("~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~\n");
	printf("[ 목숨 ] %d\n", lives);
	printf("입력 >> %s", buffer);
}

// 난이도 1sec=1000 알려줄것
int difficulty(int num) {
	int n;

	if (num == 1)
		n = 1000;
	else if (num == 2)
		n = 700;
	else if (num == 3)
		n = 400;
	else if (num == 4)
		n = 250;
	else
		n = 100;

	return n;
}

int main(void)
{
	int num;	// 난이도 입력받는 함수

	srand(time(NULL));	// 랜덤

	initRains();  // 초기화 해줘야 if (strcmp(rains[20].word, " ") != 0)가 제대로 실행됨.

	printf("난이도를 설정하세요 (1~5) : ");
	scanf("%d", &num);

	getchar();  // 처음에 스페이스바만 눌렀을때 목숨 -1 되는 거 방지

	while (lives > 0) {

		Sleep(difficulty(num));  // 난이도 별로 떨어지는 시간
		makeRain();
		printScreen();

		if (_kbhit()) {
		// 사용자가 키보드를 누를 때만 내부 블록을 실행하도록

			fgets(buffer, sizeof(buffer), stdin);
			// fgets(입력받을 값을 저장할 버퍼, 버퍼의 크기, 버퍼가 데이터를 가져올 파일 포인터)
			// stdin = 표준 입력 파일

			buffer[strcspn(buffer, "\n")] = '\0';
			// getchar랑 똑같은데, getchar로 하면 fgets이랑 호환이 안 되어 이걸로 작성.

			int found = 0;  // 변수 설명

      // 단어 올바르게 입력하면 지우는 것
			for (int i = 0; i < 20; i++) {
				if (strcmp(rains[i].word, buffer) == 0) {
					strcpy(rains[i].word, " ");
					found = 1;
					break;
				}
			}
			if (!found) {
				lives--;  // 틀리게 입력시 목숨 -1
			}

			memset(buffer, 0, sizeof(buffer));  // 입력 버퍼 초기화
		}
		if (strcmp(rains[20].word, " ") != 0) {  // 배열의 마지막 인덱스에 단어가 있다면
			lives--;  // 목숨 감소
		}

	}
	printScreen();	// 목숨 0으로 만들기 위하여
	printf("게임 오버!!\n");

	return 0;
}