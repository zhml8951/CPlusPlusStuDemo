#pragma once
#include "framework.h"

#define NUM_LINES ( (int) (sizeof(system_tricks) / sizeof(system_tricks[i])) )

struct
{
	int iIndex;
	const TCHAR* szLabel;
	const TCHAR* szDesc;
}
system_tricks[] = {
	SM_CXSCREEN, TEXT("SM_CXSREEN"), TEXT("Screen width in pixels"),
	SM_CYSCREEN, TEXT("SM_CYSCREEN"), TEXT("Screen height in pixels"),
	SM_CXVSCROLL, TEXT("SM_CXVSCROLL"), TEXT("Vertical scroll width"),
	SM_CYHSCROLL, TEXT("SM_CYHSCROLL"), TEXT("Horizontal Scroll height")
};
