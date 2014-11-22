/////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2013 CMITECH Co., Ltd.
// All rights reserved.
//
// Licensees holding valid CMITECH's CMIRIS SDK licenses may use this file in
// accordance with the CMIRIS SDK License Agreement provided with the
// Software or, alternatively, in accordance with the terms contained in
// an agreement between you and CMITECH.
////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
// emalib.h
//
// Version 1.0.0   Nov. 10, 2013
//                 Initial Public Release
//
// Version 1.0.1   Nov. 29, 2013
//  - add the support of Wiegand out channel 2 for ECU
//  - add USB Power reset feature
//
//
// Description: emalib.h provide the Wiegand, GPIO, and Relay interface in EMA-30 and
//              EMB-30 plus ECU. Please see emaglobal.h for detailed definitions.
//
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef EMALIB_H
#define EMALIB_H

#ifdef WIN32
  #include <windows.h>
  #if defined(EMALIB_LIBRARY)
    #define EMALIBSHARED_EXPORT __declspec(dllexport) __stdcall
  #else
    #define EMALIBSHARED_EXPORT __declspec(dllimport) __stdcall
  #endif
#else
  #define EMALIBSHARED_EXPORT
#endif

#define _EMA_EXPORT EMALIBSHARED_EXPORT

#define EMALIB_MAJOR_VERSION         1
#define EMALIB_MINOR_VERSION         0
#define EMALIB_REVISION              1

#include <stdio.h>
#include "emaglobal.h"

#ifdef __cplusplus
extern "C" {
#endif

    void _EMA_EXPORT  ema_getEMAVersion(unsigned int *major, unsigned int *minor, unsigned int *revision);
    // Get emalib version and revision number for compatibility.
	// Application should check if major version number is consistent.
	//
	// major[out]
	// minor[out]
	// revision[out]

    int _EMA_EXPORT ema_createEMA(EMA_HANDLE *phandle);
    // Create EMA object.
    //
    // phandle[out] - EMA_HANDLE value if successful. Otherwise, NULL.
    // Return value - EMA_ERROR_CANNOT_ALLOC_MEMORY
    //                EMA_SUCCESS

    int _EMA_EXPORT ema_destroyEMA(EMA_HANDLE handle);
    // Destroy EMA object
	//
    // handle[in] - EMA_HANDLE value
	//
    // Return value - EMA_ERROR_INVALID_HANDLE
    //                EMA_SUCCESS

    int _EMA_EXPORT ema_open(EMA_HANDLE handle);
    // Open the device.
    //
    // handle[in] - EMA_HANDLE value
    //
    // Return value - EMA_ERROR_INVALID_HANDLE
    //                EMA_ERROR_IN_ARGUMENTS
    //                EMA_ERROR_DEVICE_OPENED
    //                EMA_ERROR_CANNOT_FIND_DEVICE
    //                EMA_ERROR_FAIL_TO_OPEN_IMAGER_DEVICE
    //                EMA_ERROR_CANNOT_ALLOC_MEMORY
    //                EMA_ERROR_EEPROM_READ_TIMEOUT
    //                EMA_ERROR_EEPROM_VERSION_INVALID
    //                EMA_ERROR_CANNOT_FIND_HAARCASCADE_FILES (DMX, EMX only)
    //                EMA_ERROR_FAIL_TO_OPEN_MOTOR_DEVICE (EMX only)
    //                EMA_SUCCESS

    int _EMA_EXPORT ema_close(EMA_HANDLE handle);
    // Close the currently opened device.
    //
    // handle[in] - EMA_HANDLE value
    //
    // Return value - EMA_ERROR_INVALID_HANDLE
    //                EMA_SUCCESS

    int _EMA_EXPORT ema_writeEvent(EMA_HANDLE handle, EMA_EVENT *event);
    // Write event.
    //
    // handle[in] - EMA_HANDLE value
    // event[out] - event
    //
    // Return value - EMA_ERROR_INVALID_HANDLE
    //                EMA_ERROR_READ_EVENT_CANCELLED: canceled pending readEvent
    //                EMA_ERROR_WAIT_TIMEOUT: timeout
    //                EMA_SUCCESS: event occured

    int _EMA_EXPORT ema_readEvent(EMA_HANDLE handle, EMA_EVENT *event, unsigned int timeout);
    // Read event. EMA_EVENT_...
	//
    // handle[in] - EMA_HANDLE value
	// event[out] - event occurred
	// timeout[in] - time out in milliseconds
	//
    // Return value - EMA_ERROR_INVALID_HANDLE
    //                EMA_ERROR_READ_EVENT_CANCELLED: canceled pending readEvent
    //                EMA_ERROR_WAIT_TIMEOUT: timeout
    //                EMA_SUCCESS: event occured

    int _EMA_EXPORT ema_clearEventQueue(EMA_HANDLE handle);
	// Clear DMEvent queue
	//
    // handle[in] - EMA_HANDLE value
	//
    // Return value - EMA_ERROR_INVALID_HANDLE
    //                EMA_SUCCESS

    int _EMA_EXPORT ema_cancelPendingReadEvent(EMA_HANDLE handle);
    // Cancel pending ema_readEvent()
	//
    // handle[in] - EMA_HANDLE value
	//
    // Return value - EMA_ERROR_INVALID_HANDLE
    //                EMA_SUCCESS


#ifdef __cplusplus
} // extern "C"
#endif

#endif // EMALIB_H
