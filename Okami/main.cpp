#include <windows.h>
#include <d2d1.h>
#pragma comment(lib,"d2d1.lib")

#include <InitGuid.h>

#define DIRECTINPUT_VERSION 0x0800

//#include <dinput8.h>
#pragma comment(lib, "dsound.lib")
#include <dinput.h>
#include <dsound.h>

#include <iostream>
#include <fstream>
#include <string>

struct WaveHeaderType
{
	char chunkId[4];
	unsigned long chunkSize;
	char format[4];
	char subChunkId[4];
	unsigned long subChunkSize;
	unsigned short audioFormat;
	unsigned short numChannels;
	unsigned long sampleRate;
	unsigned long bytesPerSecond;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	char dataChunkId[4];
	unsigned long dataSize;
};


LRESULT _stdcall WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam);

HWND winInit(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(6);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "class";

	RegisterClass(&wc);
	HWND hWnd = CreateWindow(
		"class",
		"Direct2D init",
		WS_OVERLAPPEDWINDOW, 100, 100, 1280, 720,
		NULL, NULL, hInstance, NULL);
	return hWnd;
}

struct Player
{
	int x;
	int y;
	int width;
	int hight;
};

struct Box
{
	int width;
	int hight;
};

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = winInit(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	
	IDirectInput8* pDirInp = NULL;
	if (DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&pDirInp, NULL) != DI_OK)
	{
		return -1;
	}

	IDirectInputDevice8* didev;
	

	if (pDirInp->CreateDevice(GUID_SysKeyboard, &didev, NULL) != DI_OK)
	{
		pDirInp->Release();
		return -1;
	}

	ID2D1Factory* factory = NULL;
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);

	ID2D1HwndRenderTarget* rt = NULL;
	factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hWnd, D2D1::SizeU(1280, 720))
		, &rt);

	//
	/*
	WaveHeaderType waveFileHeader;
	FILE * fp;
	if ((fopen_s(&fp,"Blet.wav", "rb")) == NULL) {
		printf("cannot open this file\n");
		return -1;
	}
	fread(&waveFileHeader, sizeof(waveFileHeader), 1, fp);


    #define MAX_AUDIO_BUF 4   
    #define BUFFERNOTIFYSIZE 192000  
	int sample_rate = 44100;  
	int channels = 2;          
	int bits_per_sample = 16;  

	IDirectSound8* ids = 0;
	DSBUFFERDESC dsbd;
	memset(&dsbd, 0, sizeof(dsbd));
	dsbd.dwSize = sizeof(dsbd);
	dsbd.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GETCURRENTPOSITION2;
	dsbd.dwBufferBytes =  waveFileHeader.dataSize;
 
	dsbd.lpwfxFormat = (WAVEFORMATEX*)malloc(sizeof(WAVEFORMATEX));
	dsbd.lpwfxFormat->wFormatTag = WAVE_FORMAT_PCM;
	(dsbd.lpwfxFormat)->nChannels = channels;
	(dsbd.lpwfxFormat)->nSamplesPerSec = sample_rate;
	(dsbd.lpwfxFormat)->nAvgBytesPerSec = sample_rate * (bits_per_sample / 8)*channels;
	(dsbd.lpwfxFormat)->nBlockAlign = (bits_per_sample / 8)*channels;
	(dsbd.lpwfxFormat)->wBitsPerSample = bits_per_sample;
	(dsbd.lpwfxFormat)->cbSize = 0;
	IDirectSoundBuffer* buff = NULL;

	unsigned char* waveData = new unsigned char[waveFileHeader.dataSize];
	fread(waveData, 1, waveFileHeader.dataSize, fp);

	if (DirectSoundCreate8(NULL, &ids, NULL) != S_OK) {
		return FALSE;
	}
	ids->SetCooperativeLevel(hWnd, DSSCL_NORMAL);

	if (ids->CreateSoundBuffer(&dsbd, &buff, NULL) != S_OK) {
		return FALSE;
	}

	IDirectSoundBuffer8 *m_pDSBuffer8 = NULL;

	if (FAILED(buff->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&m_pDSBuffer8))) {
		return FALSE;
	}

	IDirectSoundNotify8 *m_pDSNotify = NULL;
	


	DSBPOSITIONNOTIFY m_pDSPosNotify[MAX_AUDIO_BUF];
	m_pDSNotify->SetNotificationPositions(MAX_AUDIO_BUF, m_pDSPosNotify);
	m_pDSNotify->Release();
		
	m_pDSBuffer8->SetCurrentPosition(0);
	m_pDSBuffer8->Play(0, 0,DSBPLAY_LOOPING);

	
	*/
	//
	
	ID2D1SolidColorBrush ** brushes = new ID2D1SolidColorBrush*[32];

	for (int i = 0; i < 27; i++)
	{
		rt->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF((float)((i / 9) % 3) / 3.5, (float)((i / 3) % 3) / 3.5, (float)(i % 3) / 3.5)),
			&brushes[i]);
	}
	rt->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(0.8f ,0.8f ,0.8f )),
		&brushes[27]);


	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	MSG msg;

	Box box;

	int **Level = NULL;
	std::string line;
	std::ifstream in("materials/w1l1.txt"); // �������� ���� ��� ������
	int c = 0;
	if (in.is_open())
	{
		while (getline(in, line))
		{
			if (c < 2)
			{
				int S = 0;
				for (int i = 0; i < line.length(); i++)
				{
					S = S * 10 + (int)line[i] - (int)'0';
				}
				if (c == 0)
				{
					box.width = S;
				}
				else
				{
					box.hight = S;
					Level = new int*[box.width];
					for (int i = 0; i < box.width; i++)
					{
						Level[i] = new int[box.hight];
					}
				}
			}
			else
			{
				int S = 0;
				for (int i = 0; i < line.length(); i++)
				{
					if (line[i] == ',')
					{
						Level[(c - 2) / box.hight][(c - 2) % box.hight] = S;
						S = 0;
						c++;
					}
					else
					{
						S = S * 10 + (int)line[i] - (int)'0';
					}
				}
			}
			c++;
		}
	}
	in.close();

	int **Visual = NULL;
	int Vnum = 0;

	std::string line0;
	std::ifstream in0("materials/w1l1Vis.txt"); // �������� ���� ��� ������
	c = 0;
	if (in0.is_open())
	{
		while (getline(in0, line0))
		{
			if (c == 0)
			{
				int S = 0;
				for (int i = 0; i < line0.length(); i++)
				{
					S = S * 10 + (int)line0[i] - (int)'0';
				}
				Vnum = S;

				box.hight = S;
				Visual = new int*[Vnum];
				for (int i = 0; i < Vnum; i++)
				{
					Visual[i] = new int[5];
				}

			}
			else
			{
				int nn = 0;
				int S = 0;
				for (int i = 0; i < line0.length(); i++)
				{
					if (line0[i] == ';')
					{
						Visual[(c - 1)][nn] = S;
						S = 0;
						nn++;
					}
					else
					{
						S = S * 10 + (int)line0[i] - (int)'0';
					}
				}
			}
			c++;
		}
	}
	in0.close();

	Player P;
	P.x = 128; P.y = 480; P.width = 128; P.hight = 72;


	Player End;
	End.x = box.width - 200; End.y = 480; End.width = 120; End.hight = 80;

	Box box0;
	box0.hight = 720; box0.width = 1280;

	bool onAir = false;
	didev->SetDataFormat(&c_dfDIKeyboard);
	didev->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	didev->Acquire();
	//int dx, dy = 0;
	char buffer[256];

	int dy = 0;
	bool flag = true;
	while (flag)
	{   
		//
		/*
		LPVOID buf = NULL;	
		DWORD  buf_len = 0;
		DWORD res = WAIT_OBJECT_0;
		DWORD offset = BUFFERNOTIFYSIZE;
		if ((res >= WAIT_OBJECT_0) && (res <= WAIT_OBJECT_0 + 3)) {
			m_pDSBuffer8->Lock(offset, BUFFERNOTIFYSIZE, &buf, &buf_len, NULL, NULL, 0);
			if (fread(buf, 1, buf_len, fp) != buf_len) {
				//File End  
				//Loop:  
				fseek(fp, 0, SEEK_SET);
				fread(buf, 1, buf_len, fp);
				//Close:  
				//isPlaying=0;  
			}
			m_pDSBuffer8->Unlock(buf, buf_len, NULL, 0);
			offset += buf_len;
			offset %= (BUFFERNOTIFYSIZE * MAX_AUDIO_BUF);
			printf("this is %7d of buffer\n", offset);
		}
		res = WaitForMultipleObjects(MAX_AUDIO_BUF, m_event, FALSE, INFINITE);
		*/
		//
		HRESULT hr = didev->GetDeviceState(sizeof(buffer), buffer);
		if FAILED(hr)
			return 0;
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		rt->BeginDraw();
		rt->Clear(D2D1::ColorF(D2D1::ColorF::White));

		rt->DrawRectangle(
			D2D1::RectF(0, 120, 1280, 360),
			brushes[17], 240.0f, NULL);

		rt->DrawRectangle(
			D2D1::RectF(0, 540, 1280, 660),
			brushes[7], 120.0f, NULL);

		bool Y = (P.y < End.hight + End.y) && (P.y >= End.y);
		bool cond_1 = (P.x >= End.x && (End.x + End.width) >= P.x);
		bool cond_2 = ((P.x + P.width >= End.x && (End.x + End.width) >= P.x + P.width));
		bool cond_3 = (P.x <= End.x) && (P.x + P.width >= End.x + End.width);

		if ((cond_1 || cond_2 || cond_3)) // ( Y && (cond_1 || cond_2 || cond_3))
		{
			flag = false;
		}

		float pw4 = min((float)P.width / 4, (float)P.hight / 4);

		float ew4 = min((float)End.width / 4, (float)End.hight / 4);

		float Player_disp_x;

		if (P.x + 0.6f * 1280 <= box.width && P.x - 0.4f * 1280 >= 0)
		{
			Player_disp_x = 1280 * 0.4f;
			rt->DrawRectangle(
				D2D1::RectF(1280 * 0.4f + pw4, P.y - pw4, 1280 * 0.4f + P.width - pw4, P.y - P.hight + pw4),
				brushes[26], 2 * pw4, NULL);
		}
		else
		{ 
			if (P.x + 0.6f * 1280 > box.width)
			{
				Player_disp_x = P.x - (box.width - 1280);
				rt->DrawRectangle(
					D2D1::RectF(P.x - (box.width - 1280) + pw4, P.y - pw4, P.x + P.width -(box.width - 1280) - pw4, P.y - P.hight + pw4),
					brushes[26], 2 * pw4, NULL);
			}
			else
			{
				Player_disp_x = P.x;
				rt->DrawRectangle(
					D2D1::RectF(P.x + pw4, P.y - pw4, P.x + P.width - pw4, P.y - P.hight + pw4),
					brushes[26], 2 * pw4, NULL);
			}
		}


        
		rt->DrawRectangle(
				D2D1::RectF(Player_disp_x + (End.x - P.x) + ew4, End.y - ew4, Player_disp_x + (End.x - P.x) + End.width - ew4, End.y - End.hight + ew4),
				brushes[27], 2 * ew4, NULL);

		if (buffer[DIK_RIGHT] & 0x80)
			P.x += 7;
		else if (buffer[DIK_LEFT] & 0x80)
			P.x -= 7;

		if (buffer[DIK_LCONTROL] & 0x80 && onAir == false)//
		{
			dy = -37;
			P.y += -1;
		}
		onAir = true;
		for (int i = 0; i < P.width && onAir == true; i++)
		{
			if (Level[P.x + i][P.y + 1] == 1)
			{
				onAir = false;
				dy = 0;
			}
		}
		int DetW, DetH = 0;
		for (int i = 0; i < Vnum; i++)
		{
			DetW = (float)Visual[i][2] / 4;
			DetH = (float)Visual[i][3] / 4;


			int con = 2 * ((int)(box.width - 0.6f * 1280) - 0.4f * 1280) ;
			/*
			if (Visual[i][1] >= 480)
			{
				rt->DrawRectangle(
					D2D1::RectF((Visual[i][0] + 3 * DetW - (2 * P.x) % box0.width + box0.width) % box0.width - 2 * DetW, Visual[i][1] - DetH, (Visual[i][0] + 3 * DetW - (2 * P.x) % box0.width + box0.width) % box0.width, Visual[i][1] - 3 * DetH),
					brushes[Visual[i][4]], 2 * min(DetW, DetH), NULL);
			}
			else
			{

				rt->DrawRectangle(
					D2D1::RectF((Visual[i][0] + 3 * DetW - (int)(P.x / 7) % box0.width + box0.width) % box0.width - 2 * DetW, Visual[i][1] - DetH, (Visual[i][0] + 3 * DetW - (int)(P.x / 7) % box0.width + box0.width) % box0.width, Visual[i][1] - 3 * DetH),
					brushes[Visual[i][4]], 2 * min(DetW, DetH), NULL);
			}
			*/
			
			if (P.x + 0.6f * 1280 <= box.width)
			{
				if(P.x - 0.4f * 1280 >= 0)
				{
					if (Visual[i][1] >= 480)
					{
						if ((Visual[i][0] - 3 * DetW - (2 * (int(P.x - 0.4f * 1280))) % box0.width + box0.width) % box0.width <= (Visual[i][0] - DetW - (2 * (int)(P.x - 0.4f * 1280)) % box0.width + box0.width) % box0.width)
						{
							rt->DrawRectangle(
								D2D1::RectF((Visual[i][0] - 3 * DetW - (2 * (int(P.x - 0.4f * 1280))) % box0.width + box0.width) % box0.width, Visual[i][1] - DetH, (Visual[i][0] - DetW - (2 * (int)(P.x - 0.4f * 1280)) % box0.width + box0.width) % box0.width, Visual[i][1] - 3 * DetH),
								brushes[Visual[i][4]], 2 * min(DetW, DetH), NULL);
						}
						else
						{
							rt->DrawRectangle(
								D2D1::RectF((Visual[i][0] - 3 * DetW - (2 * (int(P.x - 0.4f * 1280))) % box0.width + box0.width) % box0.width, Visual[i][1] - DetH, box0.width, Visual[i][1] - 3 * DetH),
								brushes[Visual[i][4]], 2 * min(DetW, DetH), NULL);
							rt->DrawRectangle(
								D2D1::RectF(0, Visual[i][1] - DetH, (Visual[i][0] - DetW - (2 * (int)(P.x - 0.4f * 1280)) % box0.width + box0.width) % box0.width, Visual[i][1] - 3 * DetH),
								brushes[Visual[i][4]], 2 * min(DetW, DetH), NULL);
						}
					}
					else
					{
						if ((Visual[i][0] - 3 * DetW - (2 * (int(P.x - 0.4f * 1280)))/4 % box0.width + box0.width) % box0.width <= (Visual[i][0] - DetW - (2 * (int)(P.x - 0.4f * 1280))/4 % box0.width + box0.width) % box0.width)
						{
							rt->DrawRectangle(
								D2D1::RectF((Visual[i][0] - 3 * DetW - (2 * (int(P.x - 0.4f * 1280))/4) % box0.width + box0.width) % box0.width, Visual[i][1] - DetH, (Visual[i][0] - DetW - (2 * (int)(P.x - 0.4f * 1280)/4) % box0.width + box0.width) % box0.width, Visual[i][1] - 3 * DetH),
								brushes[Visual[i][4]], 2 * min(DetW, DetH), NULL);
						}
						else
						{
							rt->DrawRectangle(
								D2D1::RectF((Visual[i][0] - 3 * DetW - (2 * (int(P.x - 0.4f * 1280)))/4 % box0.width + box0.width) % box0.width, Visual[i][1] - DetH, box0.width, Visual[i][1] - 3 * DetH),
								brushes[Visual[i][4]], 2 * min(DetW, DetH), NULL);
							rt->DrawRectangle(
								D2D1::RectF(0, Visual[i][1] - DetH, (Visual[i][0] - DetW - (2 * (int)(P.x - 0.4f * 1280))/4 % box0.width + box0.width) % box0.width, Visual[i][1] - 3 * DetH),
								brushes[Visual[i][4]], 2 * min(DetW, DetH), NULL);
						}
		
					}
				}
				else
				{
					if ((Visual[i][0] - 3 * DetW + box0.width) % box0.width <= (Visual[i][0] - DetW + box0.width) % box0.width)
					{
						rt->DrawRectangle(
							D2D1::RectF((Visual[i][0] - 3 * DetW + box0.width) % box0.width, Visual[i][1] - DetH, (Visual[i][0] - DetW + box0.width) % box0.width, Visual[i][1] - 3 * DetH),
							brushes[Visual[i][4]], 2 * min(DetW, DetH), NULL);
					}
					else
					{
						rt->DrawRectangle(
							D2D1::RectF((Visual[i][0] - 3 * DetW + box0.width) % box0.width, Visual[i][1] - DetH, box0.width, Visual[i][1] - 3 * DetH),
							brushes[Visual[i][4]], 2 * min(DetW, DetH), NULL);
						rt->DrawRectangle(
							D2D1::RectF(0, Visual[i][1] - DetH, (Visual[i][0] - DetW + box0.width) % box0.width, Visual[i][1] - 3 * DetH),
							brushes[Visual[i][4]], 2 * min(DetW, DetH), NULL);
					}
					

				}
			}
			else
			{
				if ((Visual[i][0] - 3 * DetW - (con) % box0.width + box0.width) % box0.width < (Visual[i][0] - DetW - (con) % box0.width + box0.width) % box0.width)
				{
					if (Visual[i][1] >= 480)
					{
						if ((Visual[i][0] - 3 * DetW - (con) % box0.width + box0.width) % box0.width < (Visual[i][0] - DetW - (con) % box0.width + box0.width) % box0.width)
						{
							rt->DrawRectangle(
								D2D1::RectF((Visual[i][0] - 3 * DetW - (con) % box0.width + box0.width) % box0.width, Visual[i][1] - DetH, (Visual[i][0] - DetW - (con) % box0.width + box0.width) % box0.width, Visual[i][1] - 3 * DetH),
								brushes[Visual[i][4]], 2 * min(DetW, DetH), NULL);
						}
						else
						{
							rt->DrawRectangle(
								D2D1::RectF(((Visual[i][0] - 3 * DetW - con) % box0.width + box0.width) % box0.width, Visual[i][1] - DetH, box0.width, Visual[i][1] - 3 * DetH),
								brushes[Visual[i][4]], 2 * min(DetW, DetH), NULL);
							rt->DrawRectangle(
								D2D1::RectF(0, Visual[i][1] - DetH, ((Visual[i][0] - DetW - con) % box0.width + box0.width) % box0.width, Visual[i][1] - 3 * DetH),
								brushes[Visual[i][4]], 2 * min(DetW, DetH), NULL);
						}
					}
					else
					{
						if ((Visual[i][0] - 3 * DetW - (con / 4) % box0.width + box0.width) % box0.width < (Visual[i][0] - DetW - (con / 4) % box0.width + box0.width) % box0.width)
						{
							rt->DrawRectangle(
								D2D1::RectF((Visual[i][0] - 3 * DetW - (int)(con / 4) % box0.width + box0.width) % box0.width, Visual[i][1] - DetH, (Visual[i][0] - DetW - (int)(con / 4) % box0.width + box0.width) % box0.width, Visual[i][1] - 3 * DetH),
								brushes[Visual[i][4]], 2 * min(DetW, DetH), NULL);
						}
						else
						{
							rt->DrawRectangle(
								D2D1::RectF((Visual[i][0] - 3 * DetW - (int)(con / 4) % box0.width + box0.width) % box0.width, Visual[i][1] - DetH, box0.width, Visual[i][1] - 3 * DetH),
								brushes[Visual[i][4]], 2 * min(DetW, DetH), NULL);
							rt->DrawRectangle(
								D2D1::RectF(0, (Visual[i][0] - DetW - (int)(con / 4) % box0.width + box0.width) % box0.width, Visual[i][1] - 3 * DetH),
								brushes[Visual[i][4]], 2 * min(DetW, DetH), NULL);
						}
					}
				}

				/*
				
				if (Visual[i][1] >= 480)
				{
					rt->DrawRectangle(
						D2D1::RectF((Visual[i][0] + 3 * DetW - (P.x) % box0.width + box0.width) % box0.width - 2 * DetW, Visual[i][1] - DetH, (Visual[i][0] + 3 * DetW - (P.x) % box0.width + box0.width) % box0.width, Visual[i][1] - 3 * DetH),
						brushes[Visual[i][4]], 2 * min(DetW, DetH), NULL);
				}
				else
				{

					rt->DrawRectangle(
						D2D1::RectF((Visual[i][0] + 3 * DetW - (int)(con) % box0.width + box0.width) % box0.width - 2 * DetW, Visual[i][1] - DetH, (Visual[i][0] + 3 * DetW - (int)(con ) % box0.width + box0.width) % box0.width, Visual[i][1] - 3 * DetH),
						brushes[Visual[i][4]], 2 * min(DetW, DetH), NULL);
				}
				*/
				
			}
		    
		    
		

		}

		if (onAir == true)
		{
			int Znak = dy / max(abs(dy), 0.01);
			for (int i = 0; i < abs(dy); i++)
			{
				P.y += Znak;
				for (int j = 0; j < P.width; j++)
				{
					if (Level[P.x + j][P.y + Znak] == 1)
					{
						P.y -= Znak;
						dy = 0;
						onAir = false;
					}
				}
			}
			int k = P.y;
			dy += 3;
		}


		//P.y = 480 - 

		rt->EndDraw();
	}

	for (int i = 0; i < 28; i++)
	{
		if (brushes[i] != NULL)
			brushes[i]->Release();
	}

	if (rt != NULL)
		rt->Release();
	if (factory != NULL)
		factory->Release();
	Sleep(500);



	return 0;
}

LRESULT _stdcall WndProc(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}