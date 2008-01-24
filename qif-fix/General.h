/*
 * qif_fix
 *
 * General.h
 *
 * Copyright (C) 2008 by Geoffrey Coram. All rights reserved.
 * 
 */

/* Include Files */
#define _INC_OLE
#include <windows.h>
#undef  _INC_OLE
#include <windowsx.h>
#include <commctrl.h>
#ifndef _WIN32_WCE_PPC
#include <commdlg.h>
#endif
#include <winsock.h>

/* Define */
#define WINDOW_TITLE			TEXT("qif_fix")
#define STR_MUTEX				TEXT("_qif_fix_Mutex_")
#define MAIN_WND_CLASS			TEXT("qiffixMainWndClass")
#define IDR_MENU_WINDOW			101
#define IDI_ICON_MAIN			103

#define STR_ERR_MEMALLOC			TEXT("Memory Allocation error")
#define STR_NO_FILE					TEXT("No file selected; exiting.")
#define STR_FILE_OPEN				TEXT("File to convert")
#define STR_FILE_SAVE				TEXT("Save as")
#define STR_FILE_FILTER				TEXT("All Files (*.*)\0*.*\0\0")
#define STR_QIF_FILTER				TEXT("QuickenImport Files (*.qif)\0*.qif\0All Files (*.*)\0*.*\0\0")

#define BUF_SIZE				256					//of process Buffer size
#define MULTI_BUF_SIZE			1024

// File
BOOL filename_select(HWND hWnd, TCHAR *ret, TCHAR *DefExt, TCHAR *filter, int Action);
long file_get_size(TCHAR *FileName);
char *file_read(TCHAR *path, long FileSize);
BOOL file_read_select(HWND hWnd, TCHAR **buf);
char *file_convert(char *buf);
BOOL file_save(HWND hWnd, TCHAR *FileName, TCHAR *Ext, char *buf, int len);

// main
void *mem_alloc(const DWORD size);
void *mem_calloc(const DWORD size);
void mem_free(void **mem);

/* End of source */
