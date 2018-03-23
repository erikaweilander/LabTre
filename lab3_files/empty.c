#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "C:\Users\Linus\Desktop\Skola\Ar2\Operativsystem\LabTre\LabbTv-Server-master\wrapper.h"
#include <stdio.h>
#include "C:\Users\Linus\Desktop\Skola\Ar2\Operativsystem\LabTre\LabTre\resource.h"


BOOLEAN CALLBACK mainDialogProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
BOOLEAN CALLBACK addDialogProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
void enterPlanet(HWND hwnd);
void readEventFromServerThread(void);
void readLifeFromServerThread(void);
void loadPlanetsToLocalList(planet_type *planet);
void loadPlanetsToServerList(planet_type *planet);
void addPlanetToServerListBox(planet_type *planet);
void removePlanetfromLocalList(planet_type *planet);
void removePlanetfromServerList(planet_type *planet);

//planet listor och mailslot
planet_type *serverHead = NULL;
planet_type *localHead = NULL;
planet_type *localList = NULL;
planet_type *serverList = NULL;
LPTSTR Slot = TEXT("\\\\.\\mailslot\\mailslot");
//Handles to the windows and mailslot
HWND mainDial;
HWND AddDial;
HWND test;
HANDLE hWrite;

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
//HWND activePlanetsNum;
HWND btnExit;
HWND localPlanets;
HWND ServerPlanets;
HWND messageBoxServer;


float GetDlgItemFloat(HWND hwnd, int nCtrlID) {

	HWND h = GetDlgItem(hwnd,nCtrlID);
	float fVal = 0.00;
	char text[256];
	GetWindowTextA(h, text, sizeof(text));
	sscanf(text, "%f",&fVal);
	return fVal;

}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow ) {
	
	
	
	//MessageBox(NULL, "It works man?\n", "A cool MboxA", 0);
	
	//mainDial = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, mainDialogProc, 0);
	AddDial = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG2), NULL, addDialogProc, 0);

	mainDial = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, mainDialogProc, 0);
	//test = CreateDialogParam(hInstance, MAKEINTRESOURCE(105), NULL, mainDialogProc, 0);

	//ShowWindow(test, SW_SHOW);
	threadCreate(readEventFromServerThread, NULL);
	threadCreate(readLifeFromServerThread, NULL);

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

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!IsWindow(mainDial) || !IsDialogMessage(mainDial, &msg))
		{
			if (!IsWindow(AddDial) || !IsDialogMessage(AddDial, &msg))
			{
				
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				
			}
		}
	}
	/*int a = 0;
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
	
	}*/



	Sleep(100);
	//system("pause");
	return (int)msg.wParam;
	
}

BOOLEAN CALLBACK mainDialogProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{

	btnSave = GetDlgItem(mainDial, button_save);
	btnAdd = GetDlgItem(mainDial, button_add);
	btnToSend = GetDlgItem(mainDial, button_ToSend);
	//activePlanetsNum = GetDlgItem(mainDial, NumberOfActivePlanets);
	btnLoad = GetDlgItem(mainDial, button_load);
	btnExit = GetDlgItem(mainDial, button_exit);
	localPlanets = GetDlgItem(mainDial, box_localplanets);
	activePlanetsBox = GetDlgItem(mainDial, box_activeplanets);
	messageBoxServer = GetDlgItem(mainDial, Box_messagesServer);
	int amount = 0;
	HANDLE F;
	
	switch (Message)
	{
		case WM_CLOSE:
			PostQuitMessage(0);
			DestroyWindow(hwnd);
			return TRUE;
		

		case WM_COMMAND:

			switch (LOWORD(wParam))
			{
			case button_exit:

				//MessageBox(NULL, "ät skit", "", 0);
				PostQuitMessage(0);
				break;

			case button_add:

				SetWindowPos(AddDial, HWND_TOP, 600, 250, 500, 250, NULL);
				ShowWindow(AddDial, SW_SHOW);
				break;

			case button_load:

				F = OpenFileDialog("mainDial", GENERIC_READ, OPEN_EXISTING);
				if (F == INVALID_HANDLE_VALUE)
					MessageBox(NULL, "Could not open file", "Error when opening file", 0);
				else
				{

					planet_type *readPlanet = malloc(sizeof(planet_type));
					DWORD bytesRead;
					while (TRUE)
					{

						ReadFile(F, readPlanet, sizeof(planet_type), &bytesRead, NULL);
						int i = GetCurrentProcessId();
						char pidToString[30];
						sprintf(pidToString, "%d", i);
						strcpy(readPlanet->pid, pidToString);
						if (bytesRead == 0)
							break;
						else {
							SendMessage(localPlanets, LB_ADDSTRING, 0, (LPARAM)readPlanet->name);
							loadPlanetsToLocalList(readPlanet);
						}
					}

					CloseHandle(F);
				}
				break;

			case button_save:

				amount = SendMessage(localPlanets, LB_GETCOUNT, 0, 0);
				F = OpenFileDialog("mainDial", GENERIC_WRITE, OPEN_EXISTING);
				DWORD BytesWrote;
				if (F == INVALID_HANDLE_VALUE)
					MessageBox(NULL, "Could not open file", "Error when opening file", 0);
				if (amount > 0)
				{
					planet_type *current = localList;
					while (current != NULL)
					{
						WriteFile(F, current, sizeof(planet_type), &BytesWrote, NULL);
						current = current->next;
						amount--;
					}
				}
				else
				{
					CloseHandle(F);
					break;
				}
				break;

			case button_ToSend:

				hWrite = mailslotConnect(Slot);
				DWORD bytesWritten;
				if (Slot == INVALID_HANDLE_VALUE)
				{
					printf("Could not connect to mailslot");
					SendMessage(messageBoxServer, "Could not connect to mailslot", 0, 0);
					break;
				}
				else
				{
					int amount = SendMessage(localPlanets, LB_GETCOUNT, 0, 0);

					if (amount > 0)
					{
						planet_type *current = localList;
						addPlanetToServerListBox(current);
						while (current != NULL)
						{
							bytesWritten = mailslotWrite(hWrite, current, sizeof(planet_type));
							if (bytesWritten != -1)
								printf("data sent to server (bytes = %d)\n", bytesWritten);
							else
							{
								printf("failed sending data to server\n");
								SendMessage(messageBoxServer, "failed sending data to server\n", 0, 0);
							}


							loadPlanetsToServerList(current);
							removePlanetfromLocalList(current);
							current = localList;
							amount--;
						}


					}
					else
					{
						break;
					}


					break;


				}
			}
			return TRUE;
	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
		
	
	default: 
		DefWindowProc(hwnd, Message, wParam, lParam);
		return FALSE;
		
	}
	//return 
	//return FALSE;
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
	//localPlanets = GetDlgItem(AddDial, box_localplanets);

	switch (Message)
	{
		case WM_CLOSE:
		{
			DestroyWindow(hwnd);
			return TRUE;
			//break;
		}
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case btn_planetAdd:
			{

				enterPlanet(hwnd);
				ShowWindow(hwnd, SW_HIDE);
				SetWindowText(pName, "");
				SetWindowText(pXPos, "");
				SetWindowText(pYPos, "");
				SetWindowText(pMass, "");
				SetWindowText(pLife, "");
				SetWindowText(pXVel, "");
				SetWindowText(pYVel, "");

				break;
			}

			case btn_ExitAdd:
			{

				ShowWindow(AddDial, SW_HIDE);
				break;
			}

			case planet_Name:
			{
			
				break;
			}

			case planet_xpos:
			{
			
				break;
			}

			case planet_ypos:
			{

			
				break;
			}

			case planet_Life:
			{
			
				break;


			}
			case planet_xVel:
			{
			
				break;
			}

			case planet_yVel:
			{
		
				break;
			}

			case planet_mass:
			{
			
				break;
			}
			}

			return TRUE;

		case WM_DESTROY:
		{
			//PostQuitMessage(0);
			return TRUE;
		}
		default: 
			//DefWindowProc(hwnd, Message, wParam, lParam);
			return FALSE;
	
	
	}
	//return FALSE;
	//return DefWindowProc(hwnd, Message, wParam, lParam);
	//return FALSE;
}

void enterPlanet(HWND hwnd)
{
	planet_type *newPlanet = malloc(sizeof(planet_type));
	int textLen = SendMessage(pName, WM_GETTEXTLENGTH,0,0);
	char buffer[20];
	if (textLen < 1) 
	{
		strcpy_s(buffer, sizeof(buffer), "nameless");
		strcpy_s(newPlanet->name, sizeof(buffer),buffer);
	}
	else 
	{
		SendMessage(pName, WM_GETTEXT, 20, buffer);
		strcpy_s(newPlanet->name, sizeof(buffer),buffer);
	}

	
	newPlanet->sx = GetDlgItemFloat(hwnd, planet_xpos);
	newPlanet->sy = GetDlgItemFloat(hwnd, planet_ypos);
	newPlanet->mass = GetDlgItemFloat(hwnd, planet_mass);
	newPlanet->life = GetDlgItemInt(hwnd, planet_Life, 0, 0);
	newPlanet->vx = GetDlgItemFloat(hwnd, planet_xVel);
	newPlanet->vy = GetDlgItemFloat(hwnd, planet_yVel);
	newPlanet->next = NULL;
	int i = GetCurrentProcessId();
	char hej[30];
	sprintf(hej, "%d", i);
	strcpy(newPlanet->pid, hej);
	loadPlanetsToLocalList(newPlanet);
	SendMessage(localPlanets, LB_ADDSTRING, 0, (LPARAM)newPlanet->name);
	free(newPlanet);

}

void readEventFromServerThread(void) {
	
	char messageFromMailbox[50];
	char SlotWithPid[50];
	sprintf(SlotWithPid, "\\\\.\\mailslot\\%d\\events", GetCurrentProcessId());
	char nameBuffer[50];
	HANDLE hReadEvent = mailslotCreate(SlotWithPid); // erika är bäst
	if (hReadEvent == INVALID_HANDLE_VALUE) {
		SendMessage(messageBoxServer, "Failed to get a handle to the mailslot!", 0, 0);
		printf("Failed to get a handle to the mailslot!");
		return;
	}
	while (TRUE)
	{
		DWORD bytesRead = mailslotRead(hReadEvent, &messageFromMailbox, strlen(messageFromMailbox));
		if (bytesRead > 0)
		{
			planet_type *eventPointer = serverHead;
			planet_type *temp = serverHead;
			char *msg = messageFromMailbox;
			
			while (eventPointer != NULL)
			{    
				if (strstr(msg, eventPointer->name))
				{
					removePlanetfromServerList(eventPointer);
					eventPointer = serverHead;
					/*SendMessage(activePlanetsBox, LB_RESETCONTENT, 0, NULL);
					while (temp != NULL)
					{
						sprintf(nameBuffer, "%s  -  Life: %d", temp->name, temp->life);
						SendMessage(activePlanetsBox, LB_ADDSTRING, 0, (LPARAM)nameBuffer); //Lägger till de aktiva planeterna igen efter reset
						temp = temp->next;
					}*/
					break;
				}
				if (eventPointer == NULL)
					break;

				eventPointer = eventPointer->next;
			}

			for (int i = 0; i < 50; i++)
			{
				if (msg[i] == '.')
				{
					msg[i + 1] = '\0';
					break;
				}
			}
			SendMessage(messageBoxServer, LB_ADDSTRING, 0, (LPARAM)msg);
			
		}
		
	}
}

void readLifeFromServerThread(void) {
	typedef struct planet_info
	{
		char name[20];
		int life;
		char pid[30];
	}planet_info;

	planet_info thisPlanet;
	char messageFromMailbox[50];
	char SlotWithPid[50];
	sprintf(SlotWithPid, "\\\\.\\mailslot\\%d\\life", GetCurrentProcessId());
	int updateTime = 100;
	char nameBuffer[50];
	HANDLE hReadLife = mailslotCreate(SlotWithPid); // erika är bäst
	if (hReadLife == INVALID_HANDLE_VALUE) {
		SendMessage(messageBoxServer, "Failed to get a handle to the mailslot!", 0, 0);
		printf("Failed to get a handle to the mailslot!");
		return;
	}
	while (TRUE)
	{
		DWORD bytesRead = mailslotRead(hReadLife, &thisPlanet, sizeof(thisPlanet));
		if (bytesRead > 0)
		{
			planet_type *lifePointer = serverHead;
			planet_type *temp = serverHead;
			while (lifePointer != NULL)
			{
				char *msg = messageFromMailbox;
				if (!strcmp(thisPlanet.name, lifePointer->name))
				{
					lifePointer->life = thisPlanet.life;
				}
				if (updateTime > 0)
					updateTime--;
				else if (updateTime <= 0)
				{
					
					SendMessage(activePlanetsBox, LB_RESETCONTENT, 0, NULL);
					updateTime = 100;
					
					while (temp != NULL) 
					{
						sprintf(nameBuffer, "%s  -  Life: %d", temp->name, temp->life);
						SendMessage(activePlanetsBox, LB_ADDSTRING, 0, (LPARAM)nameBuffer);
						temp = temp->next;
					}

				}

				if (lifePointer == NULL)
					break;
				UpdateWindow(AddDial);
				lifePointer = lifePointer->next;
			}
		}
		
	}

}

void loadPlanetsToLocalList(planet_type *planet)
{
	
	planet_type *newPlanet = malloc(sizeof(planet_type));
	memcpy(newPlanet, planet, sizeof(planet_type));
	if (localList == NULL)
	{
		localList = newPlanet;
		localHead = localList;
		localList->next = NULL;
	}
	else
	{
		planet_type *current = localList;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = newPlanet;
		current->next->next = NULL;
	}
}
void loadPlanetsToServerList(planet_type *planet)		// SJUKT FUL KOD! BLÄ,HATA,GÅR EMOT ALLA MINA PRINCIPER
{
	planet_type *newPlanet = malloc(sizeof(planet_type));
	memcpy(newPlanet, planet, sizeof(planet_type));
	if (serverList == NULL)
	{
		serverList = newPlanet;
		serverHead = newPlanet;
		serverList->next = NULL;
	}
	else
	{
		planet_type *current = serverList;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = newPlanet;
		current->next->next = NULL;
	}
}
void addPlanetToServerListBox(planet_type *planet)
{
	//hämtar antalet saker i listan
	int amount = SendMessage(localPlanets, LB_GETCOUNT, 0, 0);
	char nameBuffer[50];
	if (amount > 0)
	{
		planet_type *current = localList;
		
		while (current != NULL)		//skickar över och tar bort i lokala listan
		{
			sprintf(nameBuffer, "%s  -  Life: %d", current->name, current->life);
			SendMessage(activePlanetsBox, LB_ADDSTRING, 0, (LPARAM)nameBuffer);
			current = current->next;
			amount--;
			SendMessage(localPlanets, LB_DELETESTRING, amount, NULL);
		}
	}
	UpdateWindow(localPlanets);
}

void removePlanetfromLocalList(planet_type *planet)
{
	
	planet_type *planetToRemove = planet;
	planet_type *temp;


	if (planet == localList)
	{
		if (localList->next != NULL)
		{
			temp = localList;
			localList = localList->next;
			free(temp);
			temp = NULL;
		}
		else
		{
			free(localList);
			localList = NULL;
		}
	}
	else
	{
		planet_type *currentPlanet = localList;
		while (currentPlanet->next != NULL)
		{
			if (planet == currentPlanet->next)
			{
				if (currentPlanet->next != NULL)
				{
					temp = currentPlanet->next->next;
					free(currentPlanet->next);
					currentPlanet->next = temp;
					break;
				}
				else
				{
					free(currentPlanet->next);
					currentPlanet->next = NULL;
					break;
				}
			}
			currentPlanet = currentPlanet->next;
		}
	}
}
void removePlanetfromServerList(planet_type *planet)
{

	planet_type *planetToRemove = planet;
	planet_type *temp;


	if (planet == serverHead)
	{
		if (serverHead->next != NULL)
		{
			temp = serverHead;
			serverHead = serverHead->next;
			free(temp);
			temp = NULL;
		}
		else
		{
			free(serverHead);
			serverHead = NULL;
		}
	}
	else
	{
		planet_type *currentPlanet = serverHead;
		while (currentPlanet->next != NULL)
		{
			if (planet == currentPlanet->next)
			{
				if (currentPlanet->next != NULL)
				{
					temp = currentPlanet->next->next;
					free(currentPlanet->next);
					currentPlanet->next = temp;
					break;
				}
				else
				{
					free(currentPlanet->next);
					currentPlanet->next = NULL;
					break;
				}
			}
			currentPlanet = currentPlanet->next;
		}
	}
	
}

