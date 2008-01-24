/*
 * qif_fix
 *
 * main.c
 *
 * Copyright (C) 2008 by Geoffrey Coram. All rights reserved.
 * 
 */

/* Include Files */
#include "General.h"

HINSTANCE hInst;
static HWND InitInstance(HINSTANCE hInstance);

/*
 * InitInstance
 */
static HWND InitInstance(HINSTANCE hInstance)
{
	HWND hwndMain =  NULL;
	WNDCLASS wc;

	wc.style = 0;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU_WINDOW);
//	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.lpfnWndProc = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_MAIN));
	wc.hbrBackground = (HBRUSH)COLOR_BTNSHADOW;
	wc.lpszClassName = MAIN_WND_CLASS;

	if (!RegisterClass(&wc)) {
		return NULL;
	}

	hwndMain = CreateWindow(MAIN_WND_CLASS,
		WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW,
		0, 0,
		240,
		320,
		NULL, NULL, hInstance, NULL);

	if (!hwndMain) {
		return NULL;
	}

	ShowWindow(hwndMain, SW_SHOW);
	UpdateWindow(hwndMain);
	return hwndMain;
}


/*
 * WinMain - main routine
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	HWND hWnd = NULL;
	HANDLE hMutex = NULL;
	char *buf;

	hInst = hInstance;

	hMutex = CreateMutex(NULL, TRUE, STR_MUTEX);
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		if (hMutex != NULL) {
			CloseHandle(hMutex);
		}
		return 0;
	}

#ifdef MAKE_WINDOW
	hWnd = InitInstance(hInst);
	if (hWnd == NULL) {
		if (hMutex != NULL) {
			CloseHandle(hMutex);
		}
		return 0;
	}
#endif

	if (file_read_select(hWnd, &buf) == TRUE) {
		char *outbuf;
		outbuf = file_convert(buf);
		if (outbuf != NULL) {
			file_save(hWnd, NULL, TEXT(".qif"), outbuf, strlen(outbuf));
			mem_free(&outbuf);
		} else {
			MessageBox(hWnd, STR_ERR_MEMALLOC, WINDOW_TITLE, MB_OK);
		}
		mem_free(&buf);
	} else {
		MessageBox(hWnd, STR_NO_FILE, WINDOW_TITLE, MB_OK);
	}

	if (hMutex != NULL) {
		CloseHandle(hMutex);
	}
	return 0;
}


/***
 * Console startup
 */

#ifdef DO_CRT
void __cdecl WinMainCRTStartup(void)
{
    STARTUPINFO stinfo;
	char *cmdline;
	int ret;

	//Compilation
	cmdline = GetCommandLine();
    if (*cmdline == '"') {
		for (cmdline++; *cmdline != '\0' && *cmdline != '"'; cmdline++);
		if (*cmdline != '\0') cmdline++;
	} else {
		for (; *cmdline != '\0' && *cmdline != ' '; cmdline++);
	}
	for (; *cmdline == ' '; cmdline++);

	//of command line Call
	stinfo.cb = sizeof(STARTUPINFO);
	stinfo.dwFlags = STARTF_USESHOWWINDOW;
	GetStartupInfo(&stinfo);

	//WinMain of information when starting End
	ret = WinMain(GetModuleHandle(NULL), NULL, cmdline, stinfo.wShowWindow);
	//of acquisition
	ExitProcess(ret);
}
int main(void) {
	return 0;
}
#endif


/*
 * mem_alloc - バッファを確保
 */
void *mem_alloc(const DWORD size)
{
	return LocalAlloc(LMEM_FIXED, size);
}

/*
 * mem_calloc - 初期化してバッファを確保
 */
void *mem_calloc(const DWORD size)
{
	return LocalAlloc(LPTR, size);
}

/*
 * mem_free
 */
void mem_free(void **mem)
{
	if (*mem != NULL) {
		LocalFree(*mem);
		*mem = NULL;
	}
}

/* End of source */
