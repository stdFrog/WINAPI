#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <memory.h>
#define LEAPYEAR(i) (((Year % 4 == 0) && (Year % 100 != 0)) || Year % 400 == 0) ? 1 : 0

char* szHelpUserName;
char* szHelp[] = {
	"\nHello This is my first CUI Project",
	"\nI appreciate your concern. thank you",
	"This program has the following options:",
	"\n/a: displays the calendar for the specified date on the screen.",
	"\nIf no options are specified, the current month's calendar is displayed on the screen.",
	"The basic syntax is: \"Calendar.exe [/a] [yyyy] [mm]\"",
	"The above programs make no system changes and do not use any registry or system files.",
	"So please use it well.",
	"If you have time, please drop by github and send us your feedback. thank you!",
	"\nlink:  \"https://github.com/stdFrog/WINAPI\"  \n"
};


char* szHelpNone[] = {
	"\nHello This is my first CUI Project",
	"I appreciate your concern. thank you",
	"if you want to use this program, check out the following tips.",
	"type: \"Calendar.exe --help\"\n",
};


char* szHelpValidNumber[] ={
	"Error: Invalid Value",
	"Valid numeric ranges are: 1900 ~ 3000"
};

const char* pSpace = "   ";


char UserName[128];
int cYear, cMonth, cDay, cwDay, clDay;
int fDay, fwDay;


int GetFirstDate();
int GetLastDate(int, int);
int	InputArgvToInt(char*[]);
BOOL GetLeap(int);

int GetUser();
void DisplayHelp();
void DisplayHelpNone();
void DisplayHelpValidNumber();
void InsertUserName();

int GetCalendarforInput(int, int);
void GetCurrentLocalDate();
void DisplayCurrentMonth();
void DisplayDate();
void DisplayInputCalendar(int, int);


int main(int argc, char* argv[])
{
	int OptionNums;

	if(GetUser()){
		InsertUserName();
	}

	GetCurrentLocalDate();
	OptionNums = argc;

	switch(OptionNums){
		case 1:
			DisplayCurrentMonth();
			break;

		case 2:
			if(strcmp(argv[1],"--help") == 0 || strcmp(argv[1],"/?") == 0){
				DisplayHelp();
				break;
			}else{
				DisplayHelpNone();
				break;
			}
		case 4:
			if(strcmp(argv[1], "/a") == 0){
				InputArgvToInt(argv);
				break;	
			}else{
				DisplayHelpNone();
				break;
			}
		default:
			DisplayHelpNone();
			break;
	}
	free(szHelpUserName);
	return 0;
}











void GetCurrentLocalDate(){
	time_t t;
	struct tm* tMap;

	time(&t);
	tMap = localtime(&t);

	cwDay = tMap->tm_wday;
	cDay = tMap->tm_mday;
	cYear = tMap->tm_year + 1900;
	cMonth = tMap->tm_mon + 1;
}

int GetFirstDate(){
	int Temp = cDay % 7;
	return 7 - (--Temp);
}

void PrintNewLineAndSpace(int Days){
	if(Days%7==6){printf("\n");}
	else{printf("%s", pSpace);}
}

void DisplayDate(int Year, int Month){
	printf("\n             %d-%d               \n", Year, Month);
	printf("Sun  Mon  Tue  Wed  Thu  Fri  Sat\n");
}

void DisplayCurrentMonth(){
	int i,j;

	fwDay = cwDay + GetFirstDate();
	clDay = GetLastDate(cYear, cMonth);

	DisplayDate(cYear, cMonth);

	for(i=0; i<fwDay; i++){
		printf("     ");
	}

	for(i=1, j=fwDay; i<=clDay; i++, j++){
		printf("%2d", i);
		PrintNewLineAndSpace(j);
	}
}

void DisplayHelpNone(){
	for(int i=0; i<sizeof(szHelpNone)/sizeof(szHelpNone[0]); i++){
		printf("%s\n", szHelpNone[i]);
	}
}

void DisplayHelp(){
	for(int i=0; i<sizeof(szHelp)/sizeof(szHelp[0]); i++){
		printf("%s\n", szHelp[i]);
	}
}

void DisplayHelpValidNumber(){
	for(int i=0; i<sizeof(szHelpValidNumber)/sizeof(szHelpValidNumber[0]); i++){
		printf("%s", szHelpValidNumber[i]);
	}
}

int GetUser(){
	DWORD strlength;
	strlength = sizeof(UserName);

	// 현재 strlen은 널 종료문자를 포함하지 않는다.
	// 반면 아래 함수는 널 종료문자를 포함한 길이를 반환한다.
	GetUserName(UserName,&strlength);
	return (int)strlength-1;
}

void InsertUserName(){
	int length, NameLength, Hello=6;

	strcat(UserName, ", ");
	NameLength = strlen(UserName);

	length = strlen(szHelp[0]);
	szHelpUserName = (char*)malloc(sizeof(char) * (NameLength + length)+1);

	strcpy(szHelpUserName, szHelp[0]);
	length = strlen(szHelpUserName)+1;

	// 이거 잘 새겨놓을 것
	memmove(szHelpUserName+Hello+NameLength, szHelpUserName+Hello, length-Hello);
	// memcpy 마지막인수로 넘기는 값은 정수형값이 아니라 size_t를 사용한다.
	memcpy(szHelpUserName+Hello+1, UserName, strlen(UserName));

	szHelp[0] = szHelpUserName;
	szHelpNone[0] = szHelpUserName;
}













BOOL GetLeap(int Year){
	return LEAPYEAR(Year);
}

int GetLastDate(int Year, int Month){
	BOOL leap = GetLeap(Year);

	switch(Month){
		case 1:
			return 31;
		case 2:
			if(leap){return 29;}
			return 28;
		case 3:
			return 31;
		case 4:
			return 30;
		case 5:
			return 31;
		case 6:
			return 30;
		case 7:
			return 31;
		case 8:
			return 31;
		case 9:
			return 30;
		case 10:
			return 31;
		case 11:
			return 30;
		case 12:
			return 31;
		default:
			printf("Error: The entered value is incorrect.");
			return 0;
	}
	return 0;
}

int GetCalendarforInput(int Year, int Month){
	int temp = 0, date;

	for(int i=1; i<Year; i++){
		temp = temp + 365 + GetLeap(i);
	}
	for(int i=1; i<Month; i++){
		temp = temp + GetLastDate(Year, i);
	}

	date = (temp+1) % 7;
	return date;
}

void DisplayInputCalendar(int Year, int Month){
	int i,j;
	int Date = GetCalendarforInput(Year, Month);
	int LastDate = GetLastDate(Year, Month);

	DisplayDate(Year, Month);

	for(i=0; i<Date; i++){
		printf("     ");
	}

	for(i=1, j=Date; i<=LastDate; j++, i++){
		printf("%2d", i);
		PrintNewLineAndSpace(j);
	}
}

int	InputArgvToInt(char* argv[]){
	char* yChar = argv[2];
	char* mChar = argv[3];
	int yLength, mLength;
	int i,j, sYear=0, sMonth=0;
	
	yLength = strlen(yChar);
	mLength = strlen(mChar);

	for(i=0, j=1000; i<yLength; j/=10, i++){
		if(yChar[i] < '0' || yChar[i] > '9'){
			DisplayHelpValidNumber();
			break;
		}
		yChar[i] = yChar[i]-0x30;
		sYear = sYear+(yChar[i] * j);
	}

	for(i=0, j=1; i<mLength; j*=10, i++){
		if(mChar[i] < '0' || mChar[i] > '9'){
			DisplayHelpValidNumber();
			break;
		}
		mChar[i] = mChar[i]-0x30;
		sMonth = (sMonth*j) + mChar[i];
	}

	if(3000 >= sYear && 1900 <= sYear){
		if(0 <= sMonth && 12 >= sMonth){
			DisplayInputCalendar(sYear, sMonth);
		}
	}else{
		DisplayHelpValidNumber();
	}
	return 0;
}
