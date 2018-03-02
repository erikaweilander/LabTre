#include <windows.h>
#include <winres.h>
#include "C:\Users\Linus\Desktop\Skola\Ar2\Operativsystem\LabTre\LabTre\resource.h"

BOOLEAN CALLBACK mainDialogProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
BOOLEAN CALLBACK addDialogProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
HWND mainDial;
HWND AddDial;
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow ) {

	//MessageBox(NULL, "It works man?\n", "A cool MboxA", 0);
	
	//mainDial = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, mainDialogProc, 0);
	//AddDial = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG2), 0, addDialogProc, 0);

	mainDial = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, mainDialogProc);
	if (!mainDial)
	{
		MessageBox(NULL, "Could not create Dialog box", "Dialoge Error", MB_ICONERROR);
		return 1;
	}
	ShowWindow(mainDial, nCmdShow);
	UpdateWindow(mainDial);
	MSG msg;

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	
	system("pause");
	return (int)msg.wParam;
	//ShowWindow(AddDial, nCmdShow);

	
	//system("pause");
	//return 1;

}

BOOLEAN CALLBACK mainDialogProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case button_exit: {
				PostQuitMessage(0);
				return TRUE;
				break;
				}	
			}
		case WM_CLOSE:
		{
			DestroyWindow(hwnd);
			return TRUE;
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return TRUE;
		}
	}

	return TRUE;
}

BOOLEAN CALLBACK addDialogProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{


	return TRUE;
}