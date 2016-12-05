//시리얼 포트 열기 기본 소스
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <atlstr.h> 

DWORD WINAPI Receive(void *lpVoid);
void GotoXY(int x, int y)
{
	COORD pos = { x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// 커서의 x 좌표를 조사한다. 
int WhereX()
{
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;


	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);
	return BufInfo.dwCursorPosition.X;
}


// 커서의 y좌표를 조사한다. 
int WhereY()
{
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;


	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);
	return BufInfo.dwCursorPosition.Y;
}

int i;
int n;
int PortNum = 0;
char buf[1] = { 0 };	// write buffer
char buff[2] = { 0 };	// read buffer

DWORD readBytes = 0, writtenBytes = 0;

HANDLE hcomm;
DCB sPS; //시리얼포트 상태저장
OVERLAPPED osWrite = { 0 }, osRead = { 0 };	// 오버랩 구조체

int main()
{

	printf("포트를 입력하세요 ex) COM3 -> 3: ");
	scanf("%d", &PortNum);

	switch (PortNum){
	case 0:
		hcomm = CreateFile(L"COM0", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
		break;
	case 1:
		hcomm = CreateFile(L"COM1", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
		break;
	case 2:
		hcomm = CreateFile(L"COM2", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
		break;
	case 3:
		hcomm = CreateFile(L"COM3", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
		break;
	case 4:
		hcomm = CreateFile(L"COM4", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
		break;
	case 5:
		hcomm = CreateFile(L"COM5", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
		break;
	case 6:
		hcomm = CreateFile(L"COM6", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
		break;
	case 7:
		hcomm = CreateFile(L"COM7", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
		break;
	case 8:
		hcomm = CreateFile(L"COM8", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
		break;
	case 9:
		hcomm = CreateFile(L"COM9", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
		break;
	default:
		hcomm = CreateFile(L"COM3", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
		break;
	}
	//파일이름(포트명),읽고쓰기,0,0, 존재할때 연다,일반적일때 연다,0 == 열린핸들값을 반환한다.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	SetupComm(hcomm, 4096, 4096);	// 버퍼 크기 설정	(핸들, 입력버퍼, 출력버퍼)
	PurgeComm(hcomm, PURGE_TXCLEAR | PURGE_RXCLEAR);	//버퍼 청소
	///////////////////////////////////////시리얼 포트 설정/////////////////////////////////////////////////////
	GetCommState(hcomm, &sPS);

	sPS.DCBlength = sizeof(sPS);// 속도 

	sPS.BaudRate = 9600; // 보우레이트 설정
	sPS.ByteSize = 8;          //데이터빗설정
	sPS.Parity = NOPARITY;   //페리티설정
	sPS.StopBits = ONESTOPBIT;  //스톱빗설정

	sPS.fOutxCtsFlow = 0;
	sPS.fOutxDsrFlow = 0;
	sPS.fRtsControl = RTS_CONTROL_DISABLE;
	sPS.fDtrControl = DTR_CONTROL_DISABLE;

	SetCommState(hcomm, &sPS);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	////////////////////////////////////////////////// 여기까지 포트 설정 끝 ///////////////////////////////////////

	//멀티 쓰레드 리시브부분
	HANDLE hThread = CreateThread(NULL, 0, ::Receive, NULL, 0, NULL);

	//트랜스밋 부분
	while (1)
	{
		buff[0] = '\0';

		if (kbhit() != 0)
		{
			buf[0] = getch();
			while (1)
			{
				if (WriteFile(hcomm, buf, 1, &writtenBytes, &osWrite) == 0)
					break;

				if (GetLastError() == 997)
					printf("잠시만 기다려주세요....\n");
				else
				{
					_tprintf(_T("Error Code : %x\n"), GetLastError());
					return 0;
				}
			}
		}
	}
	CloseHandle(hcomm);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return 0;
}

DWORD WINAPI Receive(void *lpVoid)
{
	while (1)
	{
		buff[0] = '\0';
		if (ReadFile(hcomm, buff, 2, &readBytes, &osRead) != 0){
			
			if (buff[0] == -32){
				switch (buff[1])
				{
				case 'K':
					GotoXY(WhereX() - 1, WhereY());
					break;
				case 'P':
					GotoXY(WhereX(), WhereY() + 1);
					break;
				case 'H':
					GotoXY(WhereX(), WhereY() - 1);
					break;
				case 'M':
					GotoXY(WhereX() + 1, WhereY());
					break;
				default:
					break;
				}
			}
			else if (buff[0] != NULL ){
				printf("%c", buff[0]);
			}
		}

		else
		{
			if (GetLastError() == 997)
				printf("잠시만 기다려주세요....\n");
			else
			{
				_tprintf(_T("Error Code : %x\n"), GetLastError());
				return 0;
			}
		}
	}
	CloseHandle(hcomm);
	return 0;
}