// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include "targetver.h"

#include <Windows.h>

#include <stdio.h>
#include <tchar.h>

#include <atlbase.h>
#include <CommCtrl.h>
#include <memory>

#include <iostream>

#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "atls.lib")

/******************************************************************
*                                                                 *
*  Macros                                                         *
*                                                                 *
******************************************************************/

#define MILLISECONDS_PER_SEC 1000
#define FRAMES_PER_SEC 25
#define SECONDS_PER_MINUTE 60
#define MINUTE_PER_HOUR 60
#define HOUR_PER_DAY 24

#ifdef WIN64
#define FileHandle intptr_t
#else
#define FileHandle long
#endif

#define RM_STY(ORG, STY) ORG&~STY
#define ADD_STY(ORG, STY) ORG|STY
#define MOD_STYLE(hWND, STYLE, METHOD) SetWindowLong(hWND, GWL_STYLE, METHOD(GetWindowLong(hWND, GWL_STYLE), STYLE))
#define MOD_STYLE_EX(hWND, STYLE, METHOD) SetWindowLong(hWND, GWL_EXSTYLE, METHOD(GetWindowLong(hWND, GWL_EXSTYLE), STYLE))

#define RemoveStyle(hWND, Style) MOD_STYLE(hWND, Style, RM_STY)
#define RemoveStyleEx(hWND, Style) MOD_STYLE_EX(hWND, Style, RM_STY)
#define AddStyle(hWND, Style) MOD_STYLE(hWND,Style, ADD_STY)
#define AddStyleEx(hWND, Style) MOD_STYLE_EX(hWND, Style, ADD_STY)

#define ApplyStyle(hWND) SetWindowPos(hWND,NULL,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED)

#define GET_UKAGAKA(id) GetUkagakaByTag(id)
#define GET_UAKGAKA_BY_TAG(id) GetUkagakaByTag(id)
#define GET_UKAGAKA_BY_HWDN(hwnd) UkagakaInstances_HWND[hwnd]

template<class ppT>
inline void SafeRelease(
	ppT **ppInterfaceToRelease
) {
	try {
		if (dynamic_cast<IUnknown*>(*ppInterfaceToRelease) != NULL) {

			if (*ppInterfaceToRelease != NULL) {

				(*ppInterfaceToRelease)->Release();
				(*ppInterfaceToRelease) = NULL;

			}
		}
	}
	catch (std::exception exc) {

	}
}

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

