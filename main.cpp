//�ø��� ��Ʈ ���� �⺻ �ҽ�
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

// Ŀ���� x ��ǥ�� �����Ѵ�. 
int WhereX()
{
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;


	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);
	return BufInfo.dwCursorPosition.X;
}


// Ŀ���� y��ǥ�� �����Ѵ�. 
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
DCB sPS; //�ø�����Ʈ ��������
OVERLAPPED osWrite = { 0 }, osRead = { 0 };	// ������ ����ü

int main()
{

	printf("��Ʈ�� �Է��ϼ��� ex) COM3 -> 3: ");
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
	//�����̸�(��Ʈ��),�а���,0,0, �����Ҷ� ����,�Ϲ����϶� ����,0 == �����ڵ鰪�� ��ȯ�Ѵ�.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	SetupComm(hcomm, 4096, 4096);	// ���� ũ�� ����	(�ڵ�, �Է¹���, ��¹���)
	PurgeComm(hcomm, PURGE_TXCLEAR | PURGE_RXCLEAR);	//���� û��
	///////////////////////////////////////�ø��� ��Ʈ ����/////////////////////////////////////////////////////
	GetCommState(hcomm, &sPS);

	sPS.DCBlength = sizeof(sPS);// �ӵ� 

	sPS.BaudRate = 9600; // ���췹��Ʈ ����
	sPS.ByteSize = 8;          //�����ͺ�����
	sPS.Parity = NOPARITY;   //�丮Ƽ����
	sPS.StopBits = ONESTOPBIT;  //���������

	sPS.fOutxCtsFlow = 0;
	sPS.fOutxDsrFlow = 0;
	sPS.fRtsControl = RTS_CONTROL_DISABLE;
	sPS.fDtrControl = DTR_CONTROL_DISABLE;

	SetCommState(hcomm, &sPS);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	////////////////////////////////////////////////// ������� ��Ʈ ���� �� ///////////////////////////////////////

	//��Ƽ ������ ���ú�κ�
	HANDLE hThread = CreateThread(NULL, 0, ::Receive, NULL, 0, NULL);

	//Ʈ������ �κ�
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
					printf("��ø� ��ٷ��ּ���....\n");
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
				printf("��ø� ��ٷ��ּ���....\n");
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