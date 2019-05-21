#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Remove unused part from Windows header
// Windows hearder
#include <windows.h>


// DirectShow BaseClasses
#include <streams.h>

# include<initguid.h>
// {f5b87c1d-b6b6-4e93-895e-440f8891c084}

DEFINE_GUID(CLSID_RealSenseDS,
	0xf5b87c1d, 0xb6b6, 0x4e93, 0x89, 0x5e, 0x44, 0x0f, 0x88, 0x91, 0xc0, 0x84);


