#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <winres.h>
#include "C:\Users\Linus\Desktop\Skola\Ar2\Operativsystem\LabTre\LabbTv-Server-master\wrapper.h"
#include <stdio.h>
#include "C:\Users\Linus\Desktop\Skola\Ar2\Operativsystem\LabTre\LabTre\resource.h"

BOOLEAN CALLBACK mainDialogProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
BOOLEAN CALLBACK addDialogProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
void enterPlanet(HWND hwnd);
void readFromServerThread(void);

//Handles to the windows
HWND mainDial;
HWND AddDial;

//Dialog 2

HWND pName;
HWND pXPos;
HWND pYPos;
HWND pMass;
HWND pLife;
HWND pXVel;
HWND pYVel;
HWND btExit;
HWND btAddPlanet;

//Dialog 1
HWND activePlanetsBox;
HWND btnSave;
HWND btnLoad;
HWND btnAdd;
HWND btnToSend;
HWND activePlanetsNum;
HWND btnExit;
HWND localPlanets;
HWND ServerPlanets;
HWND messageBoxServer;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow ) {
	
	
	
	//MessageBox(NULL, "It works man?\n", "A cool MboxA", 0);
	
	//mainDial = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, mainDialogProc, 0);
	AddDial = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG2), NULL, addDialogProc, 0);

	mainDial = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, mainDialogProc, 0);


	threadCreate(readFromServerThread, NULL);

	if (!mainDial)
	{
		MessageBox(NULL, "Could not create Dialog box", "Dialoge Error", MB_ICONERROR);
		return 1;
	}

	if (!AddDial)
	{
		MessageBox(NULL, "Could not create Dialog box", "Dialoge Error", MB_ICONERROR);
		return 1;
	}
	ShowWindow(mainDial, nCmdShow);
	//ShowWindow(AddDial, nCmdShow);
	
	//UpdateWindow(mainDial);
	MSG msg;
	int a = 0;
	while ((a = GetMessage(&msg, 0, 0, 0))!=0)
	{
		if (a == -1)
		{
			return -1;
		}
		if (!IsDialogMessage(mainDial, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
	}

	Sleep(100);
	//system("pause");
	return (int)msg.wParam;
	
}

BOOLEAN CALLBACK mainDialogProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{

	activePlanetsBox = GetDlgItem(mainDial, box_activeplanets);
	btnSave = GetDlgItem(mainDial, button_save);
	btnAdd = GetDlgItem(mainDial, button_add);
	btnToSend = GetDlgItem(mainDial, button_ToSend);
	activePlanetsNum = GetDlgItem(mainDial, NumberOfActivePlanets);
	btnLoad = GetDlgItem(mainDial, button_load);
	btnExit = GetDlgItem(mainDial, button_exit);
	localPlanets = GetDlgItem(mainDial, box_localplanets);
	activePlanetsBox = GetDlgItem(mainDial, box_activeplanets);
	messageBoxServer = GetDlgItem(mainDial, Box_messagesServer);
	
	switch (Message)
	{
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case button_exit:
			{

				MessageBox(NULL, "ät skit", "", 0);
				PostQuitMessage(0);
				
				break;
			}
			case button_add:
			{
				SetWindowPos(AddDial, HWND_TOP, 600, 250, 500, 250, NULL);
				ShowWindow(AddDial, SW_SHOW);
				return TRUE;
				break;
			}
			case button_load:
			{
				
				break;
			}
			case button_save:
			{
				
				break;
			}
			case button_ToSend:
			{
				
				break;

			}
			}
		case WM_CLOSE:
		{
			DestroyWindow(hwnd);
			
			return TRUE;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return FALSE;
		}
	}

	return FALSE;
}

BOOLEAN CALLBACK addDialogProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{

	pName = GetDlgItem(AddDial, planet_Name);
	pXPos = GetDlgItem(AddDial, planet_xpos);
	pYPos = GetDlgItem(AddDial, planet_ypos);
	pMass = GetDlgItem(AddDial, planet_mass);
	pLife = GetDlgItem(AddDial, planet_Life);
	pXVel = GetDlgItem(AddDial, planet_xVel);
	pYVel = GetDlgItem(AddDial, planet_yVel);
	btAddPlanet = GetDlgItem(AddDial, btn_planetAdd);
	GetDlgItem(AddDial, btn_ExitAdd);

	switch (Message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case btn_planetAdd:
		{
			
			enterPlanet(hwnd);
			
			
			break;
		}
		
		case btn_ExitAdd:
		{

			DestroyWindow(hwnd);
			break;
		}
		}
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
		return TRUE;
		//break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return TRUE;
	}
	}

	return FALSE;
}

void enterPlanet(HWND hwnd)
{
	planet_type *newPlanet = NULL;
	printf(" Please enter your planets name:");
	fgets(newPlanet->name, 20, stdin);
	printf("\n Please enter your planets x-axis pos:");
	scanf_s("%lf", &newPlanet->sx);
	printf("\n Please enter your planets y-axis pos:");
	scanf_s("%lf", &newPlanet->sy);
	printf("\n Please enter your planets x-axis velocity:");
	scanf_s("%lf", &newPlanet->vx);
	printf("\n Please enter your planets y-axis velocity:");
	scanf_s("%lf", &newPlanet->vy);
	printf("\n Please enter your planets lifetime: ");
	scanf_s("%d", &newPlanet->life);
	printf("\n Please enter your planets mass: ");
	scanf_s("%lf", &newPlanet->mass);
	getchar();
	int i = GetCurrentProcessId();
	char hej[30];
	sprintf(hej, "%d", i);
	strcpy(newPlanet->pid, hej);

}

void readFromServerThread(void) {


	char messageFromMailbox[50];
	char SlotWithPid[50] = "\\\\.\\mailslot\\";
	int i = GetCurrentProcessId();
	char PidString[30];
	sprintf(PidString, "%lu", i);
	strcat(SlotWithPid, PidString);
	HANDLE hRead = mailslotCreate(SlotWithPid); // erika är bäst
	if (hRead == INVALID_HANDLE_VALUE) {
		printf("Failed to get a handle to the mailslot!");
		return;
	}
	while (TRUE)
	{
		DWORD bytesRead = mailslotRead(hRead, messageFromMailbox, strlen(messageFromMailbox));
		if (bytesRead > 0)
		{
			printf("%s", messageFromMailbox);
		}
		Sleep(1000);
	}
}

