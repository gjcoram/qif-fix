/*
 * qif_fix
 *
 * File.c
 *
 * Copyright (C) 2008 by Geoffrey Coram. All rights reserved.
 * 
 */

/* Include Files */
#include "General.h"

extern HINSTANCE hInst;  // Local copy of hInstance

/*
 * filename_select
 */
BOOL filename_select(HWND hWnd, TCHAR *ret, TCHAR *DefExt, TCHAR *filter, BOOL is_open)
{
	OPENFILENAME of;
	TCHAR path[MULTI_BUF_SIZE];

	ZeroMemory(&of, sizeof(OPENFILENAME));
	of.lStructSize = sizeof(OPENFILENAME);
	of.hInstance = hInst;
	of.hwndOwner = hWnd;
	if (filter == NULL) {
		of.lpstrFilter = STR_FILE_FILTER;
	} else {
		of.lpstrFilter = filter;
	}
	of.nFilterIndex = 1;
	if (is_open) {
		of.lpstrTitle = STR_FILE_OPEN;
		*path = TEXT('\0');
	} else {
		of.lpstrTitle = STR_FILE_SAVE;
		lstrcpy(path, ret);
	}
	of.lpstrFile = path;
	of.nMaxFile = BUF_SIZE - 1;
	of.lpstrDefExt = DefExt;

	of.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

	//File selective dialogue is indicated
	if (is_open) {
		of.Flags |= OFN_FILEMUSTEXIST;
		if (GetOpenFileName((LPOPENFILENAME)&of) == FALSE) {
			return FALSE;
		}
	} else {
		if (GetSaveFileName((LPOPENFILENAME)&of) == FALSE) {
			return FALSE;
		}
	}

	lstrcpy(ret, path);
	return TRUE;
}

/*
 * file_get_size - ファイルのサイズを取得する
 */
long file_get_size(TCHAR *FileName)
{
	WIN32_FIND_DATA FindData;
	HANDLE hFindFile;

	if ((hFindFile = FindFirstFile(FileName, &FindData)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	FindClose(hFindFile);

	if ((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
		// ディレクトリではない場合はサイズを返す
		return (long)FindData.nFileSizeLow;
	}
	return -1;
}


/*
 * file_read - of file name The file is read
 */
char *file_read(TCHAR *path, long FileSize)
{
	HANDLE hFile;
	DWORD ret;
	char *cBuf;

	//The file is opened
	hFile = CreateFile(path, GENERIC_READ, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == NULL || hFile == (HANDLE)-1) {
		return NULL;
	}
	cBuf = (char *)mem_calloc(FileSize + 1);
	if (cBuf == NULL) {
		CloseHandle(hFile);
		return NULL;
	}

	if (ReadFile(hFile, cBuf, FileSize, &ret, NULL) == FALSE) {
		mem_free(&cBuf);
		CloseHandle(hFile);
		return NULL;
	}
	CloseHandle(hFile);
	return cBuf;
}

/*
 * file_read_select - The file is opened
 */
BOOL file_read_select(HWND hWnd, TCHAR **buf)
{
	TCHAR path[BUF_SIZE];
	char *cBuf;
	long FileSize;

	*buf = NULL;

	//Select file to import
	*path = TEXT('\0');
	if (filename_select(hWnd, path, TEXT("qif"), STR_QIF_FILTER, TRUE) == FALSE) {
		return TRUE;
	}

	//Acquisition size of file
	FileSize = file_get_size(path);
	if (FileSize <= 0) {
		return TRUE;
	}

	cBuf = file_read(path, FileSize);
	if (cBuf == NULL) {
		return FALSE;
	}

	*buf = cBuf;
	return TRUE;
}

/*
 * file_convert - convert 2-digit years to 4-digit
 */
char *file_convert(char *buf)
{
	char *nbuf, *p, *q;
	int len = strlen(buf);

	nbuf = (char *)mem_alloc(len*2 + 10); // overkill
	if (nbuf == NULL) {
		return NULL;
	}
	p = buf;
	q = nbuf;
	while (*p != '\0') {
		if (*p == 'D' && p > buf && (*(p-1) == '\r' || *(p-1) == '\n')) {
			// date
			int slashes = 0;
			while (*p != '\0' && *p != '\n' && *p != '\r') {
				if (*p == '/') {
					slashes++;
				}
				*(q++) = *(p++);
				if (slashes == 2) {
					// 50-99 assumed to be 1950-1999
					if (*p == '9' || *p == '8' || *p == '7' || *p == '6' || *p == '5') {
						*(q++) = '1';
						*(q++) = '9';
					} else {
						*(q++) = '2';
						*(q++) = '0';
					}
					break;
				}
			}
		}
		*(q++) = *(p++);
	}
	*q = '\0';
	return nbuf;
}

/*
 * file_save
 */
BOOL file_save(HWND hWnd, TCHAR *FileName, TCHAR *Ext, char *buf, int len)
{
	HANDLE hFile;
	TCHAR path[BUF_SIZE];
	DWORD ret;

	// ファイルに保存
	if (FileName == NULL) {
		*path = TEXT('\0');
	} else {
		lstrcpy(path, FileName);
	}

	if (filename_select(hWnd, path, Ext, NULL, FALSE) == FALSE) {
		return TRUE;
	}

	// 保存するファイルを開く
	hFile = CreateFile(path, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == NULL || hFile == (HANDLE)-1) {
		return FALSE;
	}
	if (WriteFile(hFile, buf, len, &ret, NULL) == FALSE) {
		CloseHandle(hFile);
		return FALSE;
	}
	CloseHandle(hFile);
	return TRUE;
}

/* End of source */
