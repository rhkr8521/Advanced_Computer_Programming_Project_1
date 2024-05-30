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

rain rains[21];	//
int lives = MAX_LIVES;
char buffer[20];

int mainpage_p() { //메인페이지 출력용 함수
	system("cls");
	printf("------------------------------------\n");
	for (int i = 0; i < 10; i++) {
		printf("\n");
		if (i == 4)printf("        뿌슝파슝 타자연습");
	}
	printf("------------------------------------\n");
	printf("--------------모드 선택-------------\n");
	printf(" 1.일반 타자 모드\n");
	printf(" 2.장문 타자 모드\n");
	printf(" 3.나가기\n");

	int test = 0; //입력받는 함수 숫자만 받게 함
	int cnt = 0; //여러번 오타 낸거 카운트용
	while (1) {
		scanf("%d", &test);
		if (test == 1) { printf("시작합니다.\n"); break; }
		else if (test == 2) { printf("시작합니다\n"); break; } //[issue]장문모드 만들면 수정
		else if (test == 3) { printf("뿌슝빠슝 타자연습을 종료합니다.\n"); break; }
		else { printf("1,2,3 중에서만 입력해주세요\n"); cnt++; }
		if (cnt >= 10) { printf("긴급탈출"); break; }
	}

	return test;
}

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

// 난이도
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


void long_typing(char** long_typing_target[], int* correctCount, int* life, int wordCount) { //장문타자 실행 함수
	getchar(); //드가기 전 버퍼 초기화
	int i = 0;//타이핑 대상 출력용 배열 접근 인덱스
	int b = 0; //아래 for문 돌리기용
	double total_start = 0; //총 걸린 시간 측정용 변수
	double total_end = 0;
	double total_sec = 0;
	clock_t start, end; //문단마다 걸린 시간 측정용 변수
	double sec = 0.0;
	total_start = clock(); // 총 걸린 시간 측정 시작
	while (*life > 0) { //라이프가 남아 있다면 계속 돌려
		start = clock(); //문단 시작시간 측정
		printf("\n%s\n", long_typing_target[i]); //타자 대상 출력


		for (b = 0; b < strlen(long_typing_target[i]); b++) { printf("-"); } //단어 길이에 따라 구분선 생성
		printf("\n");

		char input[101]; //입력 받는 코드
		fgets(input, sizeof(input), stdin);

		end = clock(); //문단입력 끝 시간 측정
		sec = (double)(end - start) / CLOCKS_PER_SEC; //문단 측정 시간계산
		printf("\n[ 시간 ] %.2lf초\n\n", sec);

		for (b = 0; b < strlen(long_typing_target[i]); b++) { printf("-"); }//단어 길이에 따라 구분선 생성
		if (input[strlen(input) - 1] == '\n') { input[strlen(input) - 1] = '\0'; } //개행문자 제거
		printf("\n\n");

		if (strcmp(input, long_typing_target[i]) == 0) { //입력이 정확하다면 
			(*correctCount)++; //맞힌 수 증가
			i++;//다음 문단
		}
		else {
			(*life)--; //생명 감소
			printf("\n남은 목숨:%d\n", *life);
			i++; //다음문단
		}
		total_end = clock(); //총 걸린 시간의 끝 측정
		if (i >= wordCount) { break; } //모든 단어를 입력할 시 종료

	}
	total_sec = (double)(total_end - total_start) / CLOCKS_PER_SEC; //총 시간 계산
	printf("=======================\n");
	printf("\n총 걸린 시간 %.2lf초\n\n", total_sec);
	printf("=======================\n");
}

int main(void)
{
	int a = mainpage_p(); //메인페이지 선택

	if (a == 1) {   //1선택시 단어떨어지기 모드 시작
		int num;	// 난이도 입력받는 함수

		srand(time(NULL));	// 랜덤
		initRains();  // 초기화 해줘야 if (strcmp(rains[20].word, " ") != 0)가 제대로 실행됨.

		printf("난이도를 설정하세요 (1~5) : ");
		scanf("%d", &num);
		printf("곧 시작 됩니다. 준비하세요! 난이도 : Lv%d\n", num);//임시로 해놓음
		Sleep(1000);
		printf("             3\n");
		Sleep(1000);
		printf("             2\n");
		Sleep(1000);
		printf("             1\n");
		Sleep(1000);
		system("cls");
		getchar();

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

				int found = 0;

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

	else if (a == 2) {  //장문모드 선택
		system("cls");
		srand((unsigned int)time(NULL)); // 난수 초기화

		char* clown[] = { //주제 : 리쌍 광대
			"오 내가 웃고있나요",
			"모두 거짓이겠죠",
			"날 보는 이들의 눈빛 속에는",
			"슬픔이 젖어있는데",
			"내 이름은 광대 내 직업은 수많은 관객",
			"그 앞에 웃음을 파는 일",
			"슬퍼도 웃으며 내 모습을 감추는게 철칙",
			"오 이런 내 처질 손가락질 하며 날 모욕해도",
			"더 크게 웃고 난 땀으로 목욕하고",
			"음악이 꺼지고 막이 내리고 밤이오면",
			"별빛에 몸을 씻고",
			"눈부시게 광낸 구드를 신고",
			"달에게 청혼하듯 손을 내밀어",
			"얼음 위를 미끄러지듯 앞으로 달려",
			"아무도 아무도 모르게 조용히 흐르는 이 시간에",
			"외롭게 홀로 핀 꽃 한송이에 난 반해",
			"사랑을 나누려 나는 간다네.",NULL
		};
		char* Bam_Yang_gang[] = { //주제 : 밤양갱 1시간버전
			"떠나는 길에 네가 내게 말했지~",
			"떠나는 길에 네가 내게 말했지~",
			"떠나는 길에 네가 내게 말했지",
			"떠나는 길에 네가 내게 말했지~",
			"떠나는 길에 네가 내게 말했지!",
			"떠나는 길에 네가 내게 말했지",
			"떠나는 길에 네가 내게 말했지~",
			"떠나는 길에 네가 내게 말했지",
			"떠나는 길에 네가 내게 말했지!",
			"떠나는 길에 네가 내게 말했지",
			"떠나는 길에 네가 내게 말했지~",
			"떠나는 길에 네가 내게 말했지",
			"떠나는 길에 네가 내게 말했지!",
			"떠나는 길에 네가 내게 말했지~",
			"떠나는 길에 네가 내게 말했지~",
			NULL
		};

		char* test_word[] = {
			"테스트중입니다",
			"테스트중입니다2",
			"테스트중입니다3",
			NULL
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		int choice_subject = 0; //주제선택 변수
		char* choice_subject_target = NULL; //주제선택 이름
		char** choice_subject_txt = NULL; //포인터의 포인터
		int correctCount = 0; // 맞힌 단어 수
		int life = 5; // 초기 생명
		printf("주제를 선택하세요\n"); //여기 조금 반복문? 등 사용으로 모듈화 필요
		printf("1.광대\n");
		printf("2.밤양갱\n");
		printf("3.테스트용\n");

		scanf("%d", &choice_subject); //주제선택 시 관련변수들을 설정
		if (choice_subject == 1) { choice_subject_target = "광대"; choice_subject_txt = clown; }
		else if (choice_subject == 2) { choice_subject_target = "밤양갱"; choice_subject_txt = Bam_Yang_gang; }
		else if (choice_subject == 3) { choice_subject_target = "테스트 입니다."; choice_subject_txt = test_word; }
		else {
			printf("대상에 없는 주제입니다. 다시 실행해 주세요");
			return 0;
		}
		int wordCount = 0;//단어 수 계산
		char** temp = choice_subject_txt;//선택한 주제의 단어 연산
		while (*temp != NULL) { 
			wordCount++;
			temp++;
		}
		printf("곧 시작 됩니다. 준비하세요!\n     제목: %s\n", choice_subject_target);//임시로 해놓음
		Sleep(1000);
		printf("             3\n");
		Sleep(1000);
		printf("             2\n");
		Sleep(1000);
		printf("             1\n");
		Sleep(1000);
		system("cls");
		long_typing(choice_subject_txt, &correctCount, &life, wordCount); // 게임 실행}
		// 게임 종료 메시지
		if (life == 0) {
			printf("게임 오버! 모든 생명을 잃었습니다.\n");
		}
		else {
			printf("게임 끝! 얼마나 입력하셨을까요?\n");
		}
		printf("맞힌 단어 수: %d\n", correctCount);
		printf("정확도 : %0.lf%%\n", (double)correctCount/wordCount * 100);
		return 0;
	}
}
