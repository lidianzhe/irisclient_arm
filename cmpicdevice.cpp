#include "cmpicdevice.h"
#include "cimageinfo.h"
#include <QDebug>

#if defined(WIN32) && defined(_DEBUG)
	 #define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
	 #define new DEBUG_NEW
#endif

#include "reportinghook.h"

CMPICDevice::CMPICDevice()
{
}

void CMPICDevice::eventHandle(EventCode eventCode, CImageInfo *imageInfo) {
	emit eventReceived(eventCode, imageInfo);
}
