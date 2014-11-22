/////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2011~2012 CMITECH Co., Ltd.
// All rights reserved.
//
// Licensees holding valid CMITECH's CMIRIS SDK licenses may use this file in
// accordance with the CMIRIS SDK License Agreement provided with the
// Software or, alternatively, in accordance with the terms contained in
// an agreement between you and CMITECH.
////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
// cmirislib.h
//
// Version 1.0.0   March 27, 2012
//                 Initial Public Release
//
// Version 1.0.2   April 26, 2012
// - Fixed the bug that the device manager did not send the power suspended and resume events.
// - USB Driver has been upgraded from version 1.4.2.216 to 1.6.0.0413 where one critical but
//   rarely happening bug was fixed. It should be noted that the WinXP SP3 and hotfix for
//   Usbehci.sys (http://support.microsoft.com/kb/969238) should be installed in WinXP before
//   the USB driver installation. The new driver is also Microsoft WHQL Certified.
// - The capture algorithm of BMT-20 has been improved as following.
//  1.	The white LED will be turned on when it starts capturing in auto mode.
//  2.	The focus measurement routine has been improved, so that it will capture
//      the images within the operating range which is 0 ~ 10mm from the subject¡¯s forehead.
//      The depth of field of the optical system of BMT-20 is 30mm to cover the wide range of
//      the relative height difference between the eyes and the forehead.
//  3.	Auto exposure will maintain the intensity of sclera area as 140 to 160 for most people
//      to achieve the consistent image quality.
//  4.	The maximum allowed movement has been set to around 7mm/sec in each X and Y direction
//      internally to ensure the good image quality. Therefore, cmi_setMaxXYMovement() will
//      not be no longer effective in BMT-20.
// - In DMX-10E and EMX-10E, event queues will be cleared internally if the subject is not
//   detected for performance enhancement in slower PC.
// - In preview event, if the buffer provided by application for the preview image is not
//   available, the oldest event will be removed and the new preview event will be appended
//   to the preview event queue. Therefore, the maximum number of preview events in the queue
//   is determined by the requested maximum number of preview frames, numPreviewFrames,
//   in cmi_getBufferSize()
//
// Version 1.0.3   June 27, 2012
// - Fixed the bug that the device arrival events were not sent correctly when the BMT-20
//   and EMX-10/DMX-10 are connected to the PC together.
// - When the only one eye is selected to be captured, both eyes information, if available,
//   will be utilized for consistent capture performance.
// - In ¡°driver/¡± directory, ¡°CMITECH Imager_1.6.0.0413_Common_Files.msm¡± has been added
//   for default installation directory ¡°C:\Program Files\Common Files\CMITECH\¡±.
//   The default installation directory of ¡°CMITECH Imager_1.6.0.0413.msm¡± is
//   ¡°C:\Program Files\CMITECH\¡±.
//
// Version 1.0.4   Sep. 8, 2012
// - Fixed the bug for DMX-10/EMX-10 that the capture process was sometimes halted in a slower
//   processor like Atom.
// - The user interaction performance of face detecting and tracking in EMX-10 has been improved.
// - The OpenCV version has been upgraded from 2.1.0 to 2.4.2.
// - In DMX-10/EMX-10, the monochrome high resolution (960 x 720) face image will be captured
//   just after the capture of iris images and sent through the faceImage pointer which is the
//   member of CMI_IMAGE_INFO structure at the event of CMI_EVENT_IRIS_IMAGES_SELECTED.
//   This face image will be only available with the firmware version 1.0.92 or higher with
//   CMIRIS SDK version 1.0.4 or higher.
// - In DMX-10/EMX-10, the capture speed will be faster if the argument maxNumUnselectedImages
//   is set to zero in the function call cmi_getBufferSize (CMI_HANDLE handle, int
//   numPreviewFrames, int maxNumUnselectedImages, int *bufferSize).
// - In BMT-20, the parameter values for gaze detection have been relaxed to cover up various
//   gaze styles.
//
// Version 1.0.5   Oct. 22, 2012
//  BMT-20:
//  - Fast auto mode (CMI_BMT_FAST_AUTO_MODE) is added for authentication purpose. Please
//    do not use this mode in enrollment process.
//  - Fixed the bug in BMT auto capture mode that could not capture even the image is good.
//    This rarely occured in slower processor.
//  - Capture algorithm has been improved to enhance the capture speed.
//
//  DMX-10/EMX-10:
//  - Face finding and eye tracking has been improved.
//  - Added the feature that the user can select left or right eye in DMX/EMX model.
//    This can be done through cmi_setWhichEye().
//
// Version 1.0.7   Jan. 18, 2013
//  - Capture algorithm has been fully optimized for embedded processor. Linux and Anroid
//    are supported.
//  - Microsoft Visual Studio 2010 x86 and x64 versions are included.
//  BMT-20:
//  - User mode (CMI_BMT_USER_MODE) is added to provide the full resolution preview images
//    to application. Now application can turn on the white LED and investigate the pupil
//    contraction for liveness check. Due to the full resolution preview image size in
//    "User mode", cmi_setBMTMode() should be called before cmi_getBufferSize().
//    Please refer to BMTDemo_src for detailed information.
//  - Fixed minor bugs.
//
//  DMX-10/EMX-10:
//  - Face and eye tracking has been improved.
//  - Motor speed of EMX-10 has been optimized with the new firmware (V1.0.94 for before
//    2012 July and V1.1.11 for after 2012 Aug.).
//  - Fixed the case that the iris image cannot be captured in the operating range.
//  - Fixed minor bugs.
//
// Version 1.0.72   Feb. 28, 2013
//  BMT-20:
//  - For backward compatibility, cmi_setBMTMode() is needed to be called only when the
//    BMTMode changes to or from the "User Mode".
//
//  DMX-10/EMX-10:
//  - Fixed the bug that crashes in the single eye capture mode when Application Verifier
//    is enabled.
//
// Version 1.0.8   March. 26, 2013
//  - Fixed minor memory leaks
//  - Fixed crash with libusb (Linux and Android)
//
//  DMX-10/EMX-10:
//  - Fixed the bug that crashes in the single eye capture mode.
//  - Captured face image size has been reduced from 720x960 to 360x480
//  - cmirisLib.dll searches /haarcascades dir by the following order
//    1. the dir where cmirisLib.dll is installed.
//    2. application's current directory
//
// Version 1.0.81   April 6, 2013
//  - Fixed the bug that does not stop Windows Message getting thread correctly
//    in cmi_destroyCMIris().
//
//  DMX-10/EMX-10:
//  - Fixed the bug that hangs in the single eye capture mode with non-zero
//    unselected images.
//
// Version 1.0.82   May 20, 2013
//  BMT-20:
//  - Added Low Bandwidth BMT mode for Low power CPU in Windows.
//    In Linux and Android, this mode will be the same as the default mode.
//    CMI_BMT_AUTO_LB_MODE, CMI_BMT_MANUAL_LB_MODE, CMI_BMT_FAST_AUTO_LB_MODE
//
// Version 1.0.84   June 20, 2013
//  BMT-20/DMX-10/EMX-10:
//  - In Linux and Android, if the image data are not fully transferred,
//    it will issue the event CMI_EVENT_ERROR_VIDEO_STREAM_MISMATCH and
//    close the device.
//
// Version 1.0.92   Aug. 30, 2013
//  - Added OEM Driver support
//  - CMIUpgrade.exe bug on firmware verification in V1.0.7 and V1.0.8 was fixed.
//  - In Linux and Android, USB capture(libusb) error handling was improved.
//  BMT-20:
//  - New firmware for lower USB bandwidth
//  EMX-10/DMX-10:
//  - Slow CPU version of cmirisLib.dll was added for Atom or old Pentium processor
//  - Image capture was improved.
//  - New firmware to lower the temperature inside for continuous operation.
//  EMA-30/EMB-30:
//  - Tamper switch and function key events are added in Device Manager Event
//    Please see cmiglobal.h for added events in EMA-30 and EMB-30. These features
//    are not supported in EMX-10
//
// Version 1.0.96   Nov. 1, 2013
//  BMT-20:
//  - The default maxXYMovement value was changed to 0.5mm to 0.8mm per 60ms
//  EMX-10/DMX-10:
//  - The default maxXYMovement value was changed to 1.0mm to 1.6mm per 67ms
//  EMA-30/EMB-30:
//  - The default maxXYMovement value was changed to 1.0mm to 1.6mm per 67ms
//  - cmicolorLib(V0.9.0) which is a color conversion library of Bayer format of
//    the face image is added.
//
// Version 1.0.97   Nov. 14, 2013
//  - MIRLIN License file on EMX/EMA/EMB/BMT
//  EMX-10/DMX-10:
//  EMA-30/EMB-30:
//  - Move up/down press/release events are added in Deveice Manager Event.
//
///////////////////////////////////////////////////////////////////////////////////////////
// cmirisLib2.h
//
// Version 2.0.0   Nov. 28, 2013
//  - Beta release: Evaluation purpose only. Interface will be changed in the initial release.
//  - return values are changed to int from unsigned int(DWORD)
//  - Smart Sensors Ltd's MIRLIN Iris recognition algorithm was included in cmirisLib2
//  EMX-10/DMX-10:
//  EMA-30/EMB-30:
//  BMT-20
//
// Version 2.0.1   Nov. 29, 2013
//  - make capture sound after the captures are completed.
//
// Version 2.0.3   Jan. 3, 2014
//  - Freezing bugs in ARM Linux have been fixed.
//
// Version 2.0.4   Jan. 18, 2014
// BMT-20:
//  - relaxed the parameter for gaze detection
//  - improve the iris detection algorithm
//
// Version 2.0.41   Jan. 23, 2014
// EMX-30:
//  - Bug fix of segmentation fault in Linux
//
// Version 2.0.5   Feb. 7, 2014
// EMX-30:
//  - Auto adjust of target intensity to avoid the sclera saturation.
//
// Version 2.0.7   Feb. 23, 2014
// EMX family:
//  - Improved color indication when the user is close to EMX.
//  - The current indicator color is now available in the field of leftPosition or
//    rightPosition in CMI_IMAGE_INFO structure.
//  - Color face image parameters are added in cmiglobal.h
//  - USB arrival/removal detection bug is fixed in Linux
//
// Version 2.0.8   March 22, 2014
// EMX family:
//  - Further improvement in color indication.
// EMA/EMB:
//  - Improved user detection with proximity sensor.
//
// Version 2.1.0   July 4, 2014
// EMX family:
//  - Rare case of failure in motor open was fixed.
//  - Improvement in color indication.
//  - DMX color indication is now the same as EMX
//  - Added the new event CMI_EVENT_INDICATOR, which notifies the indicator color
//    change and the moment when the voice cue is played. 2 voice cues were implemented,
//    "Please come closer" and "Please move back".
//  - Either eye capture mode was added
//  - Iris image margins were set as ISO/IEC 19794-6:2011(E) specified, so that
//    the capture volume was increased.
//    Horizontal: >= 0.6*Iris Radius, Vertical: >= 0.2*Iris Radius
//  - cmi_closeDevice() lockup problem is fixed.
//  - cmi_setFaceCaptureMode(), cmi_getetFaceCaptureMode() were added for color face image
//  - Minor bugs were fixed
//  - cmi_getMotorReturnToInitEnabled() and cmi_setMotorReturnToInitEnabled() were added
//
// Version 2.1.3   Oct. 3, 2014
// EMX family:
//  - The depth of field is now adjustable with cmi_setDepthOfField().
//    The center of depth of field is 330mm from the front surface of EMX.
//    The minimum and default of each depth of field is 15mm,
//    so total depth of field is 30mm. This is the depth of field for enrollment.
//    The maximum is 35mm for inside and 25mm for outside, total maximum depth of field
//    is 60mm, which is for recognition for physical access control application.
//  - added cmi_setPSDEnabled() to enable/disable the use of the proximity sensor in EMA/EMB.
//    If disabled, the CPU has more burden, but the response is faster.
//  - fixed bugs in the driver and SDK when USB device is removed and arrives
//
// Description: cmirislib2.h provide the interface with the iris image capture device
//              BMT-20, DMX-10, EMX-10, DMX-30, and EMX-30 developed by CMITECH.
//              Please see cmiglobal.h for detailed definitions.
//
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CMIRISLIB2_H
#define CMIRISLIB2_H

#ifdef WIN32
  #include <windows.h>
  #if defined(CMIRISLIB_LIBRARY)
	#define CMIRISLIBSHARED_EXPORT __declspec(dllexport) __stdcall
  #else
	#define CMIRISLIBSHARED_EXPORT __declspec(dllimport) __stdcall
  #endif
#else
    #if __GNUC__ >= 4
        #define CMIRISLIBSHARED_EXPORT __attribute__ ((visibility ("default")))
    #else
        #define CMIRISLIBSHARED_EXPORT
    #endif  // __GNUC_C >= 4
#endif

#define _CMI_EXPORT CMIRISLIBSHARED_EXPORT

#define CMIRISLIB_MAJOR_VERSION         2
#define CMIRISLIB_MINOR_VERSION         1
#define CMIRISLIB_REVISION              3

///////////////////////////////////////////////////////////////////
// Smart Sensors Ltd MIRLIN Algorithm implementation
///////////////////////////////////////////////////////////////////
#define CMI_MIR_ENROL_TEMPLATE_SIZE              579
#define CMI_MIR_MATCH_ROT_0_TEMPLATE_SIZE        579   // 0 rotations
#define CMI_MIR_MATCH_ROT_5_TEMPLATE_SIZE        2879  // 5 rotations
#define CMI_MIR_MATCH_ROT_7_TEMPLATE_SIZE        4029  // 7 rotations
#define CMI_MIR_MATCH_ROT_9_TEMPLATE_SIZE        5179  // 9 rotations
#define CMI_MIR_MATCH_ROT_13_TEMPLATE_SIZE       7479  // 13 rotations - default
#define CMI_MIR_MATCH_ROT_25_TEMPLATE_SIZE       14379 // 25 rotations

#define CMI_MIR_DO_NOT_SHOW_SEGMENTATION 0
#define CMI_MIR_SHOW_SEGMENTATION        1

// Error return values in addition to values defined in cmiglobal.h
#define CMI_MIR_LEFT_TEMPLATE_GENERATED_ONLY     -1024
#define CMI_MIR_RIGHT_TEMPLATE_GENERATED_ONLY    -1025
#define CMI_MIR_ERROR_FAIL_TO_GENERATE_TEMPLATES -1026
#define CMI_MIR_ERROR_INVALID_LICENSE            -1027
#define CMI_MIR_ERROR_UNKNOWN_LICENSE            -1028
#define CMI_MIR_ERROR_NO_INPUT_IMAGES            -1029
#define CMI_MIR_ERROR_WRONG_TEMPLATE_SIZE        -1030
#define CMI_MIR_ERROR_FAIL_TO_COMPARE_TEMPLATES  -1031
#define CMI_MIR_ERROR_EMPTY_ENROL_TEMPLATE       -1032


///////////////////////////////////////////////////////////////////
// End of Smart Sensors Ltd MIRLIN Algorithm implementation
///////////////////////////////////////////////////////////////////


#include <stdio.h>
#include "cmiglobal.h"


#ifdef __cplusplus
extern "C" {
#endif

	void _CMI_EXPORT  cmi_getCMIrisVersion(DWORD *major, DWORD *minor, DWORD *revision);
	// Get cmirislib version and revision number for compatibility.
	// Application should check if major version number is consistent.
	//
	// major[out]
	// minor[out]
	// revision[out]

    int _CMI_EXPORT cmi_createCMIris(int model, CMI_HANDLE *phandle);
	// Create CMIris. As soon as it is created, it can receive device manager event
	// Please refer to cmi_readDMEvent()
	//
	// model[in] - CMI_DMX_EMX_MODEL or CMI_BMT_MODEL
	// phandle[out] - CMI_HANDLE value if successful. Otherwise, NULL.
	// Return value - CMI_ERROR_CANNOT_ALLOC_MEMORY
	//                CMI_ERROR_INVALID_MODEL - if model is invalid. Only CMI_DMX_EMX_MODEL
	//                                          and CMI_BMT_MODEL are allowed at this moment
	//                CMI_SUCCESS

    int _CMI_EXPORT cmi_destroyCMIris(CMI_HANDLE handle);
	// Destroy CMIris.
	//
	// handle[in] - CMI_HANDLE value
	//
	// Return value - CMI_ERROR_INVALID_HANDLE
	//                CMI_SUCCESS

    int _CMI_EXPORT cmi_readDMEvent(CMI_HANDLE handle, CMI_DM_EVENT *event, DWORD timeout);
	// Read Device Manager event. DEVICE_ARRIVAL, DEVICE_REMOVAL, POWER_SUSPEND, POWER_RESUMED
	// Device Manager event will be available right after cmi_createCMIris() is called
	// until the destroy call cmi_destroyCMIris().
	//
	// handle[in] - CMI_HANDLE value
	// event[out] - event occurred
	// timeout[in] - time out in milliseconds
	//
	// Return value - CMI_ERROR_INVALID_HANDLE
	//                CMI_ERROR_READ_EVENT_CANCELLED: canceled pending readDMEvent
	//                CMI_ERROR_WAIT_TIMEOUT: timeout
	//                CMI_SUCCESS: event occured

    int _CMI_EXPORT cmi_clearDMEventQueue(CMI_HANDLE handle);
	// Clear DMEvent queue
	//
	// handle[in] - CMI_HANDLE value
	//
	// Return value - CMI_ERROR_INVALID_HANDLE
	//                CMI_SUCCESS

    int _CMI_EXPORT cmi_cancelPendingReadDMEvent(CMI_HANDLE handle);
	// Cancel pending cmi_readDMEvent()
	//
	// handle[in] - CMI_HANDLE value
	//
	// Return value - CMI_ERROR_INVALID_HANDLE
	//                CMI_SUCCESS

    int _CMI_EXPORT cmi_readEvent(CMI_HANDLE handle, CMI_EVENT *event, DWORD timeout);
	// Read event. CMI_EVENT_...
	//
	// handle[in] - CMI_HANDLE value
	// event[out] - event occurred
	// timeout[in] - time out in milliseconds
	//
	// Return value - CMI_ERROR_INVALID_HANDLE
	//                CMI_ERROR_READ_EVENT_CANCELLED: canceled pending readEvent
	//                CMI_ERROR_WAIT_TIMEOUT: timeout
	//                CMI_SUCCESS: event occured

    int _CMI_EXPORT cmi_clearEventQueue(CMI_HANDLE handle);
	// Clear DMEvent queue
	//
	// handle[in] - CMI_HANDLE value
	//
	// Return value - CMI_ERROR_INVALID_HANDLE
	//                CMI_SUCCESS

    int _CMI_EXPORT cmi_cancelPendingReadEvent(CMI_HANDLE handle);
	// Cancel pending cmi_readDMEvent()
	//
	// handle[in] - CMI_HANDLE value
	//
	// Return value - CMI_ERROR_INVALID_HANDLE
	//                CMI_SUCCESS

    int _CMI_EXPORT cmi_openDevice(CMI_HANDLE handle, CMI_DEVICE_INFO *pdeviceInfo);
	// Open the specified device by pdeviceInfo.
	// Only one device should be open at any time for guranteed performance
	//
	// handle[in] - CMI_HANDLE value
	// pdeviceInfo[in] - one of CMI_DEVICE_INFO found in cmi_getAttachedDevices()
	//
	// Return value - CMI_ERROR_INVALID_HANDLE
	//                CMI_ERROR_IN_ARGUMENTS
	//                CMI_ERROR_DEVICE_OPENED
	//                CMI_ERROR_CANNOT_FIND_DEVICE
	//                CMI_ERROR_FAIL_TO_OPEN_IMAGER_DEVICE
	//                CMI_ERROR_CANNOT_ALLOC_MEMORY
	//                CMI_ERROR_EEPROM_READ_TIMEOUT
	//                CMI_ERROR_EEPROM_VERSION_INVALID
	//                CMI_ERROR_CANNOT_FIND_HAARCASCADE_FILES (DMX, EMX only)
	//                CMI_ERROR_FAIL_TO_OPEN_MOTOR_DEVICE (EMX only)
	//                CMI_SUCCESS

    int _CMI_EXPORT cmi_closeDevice(CMI_HANDLE handle);
	// Close the currently opened device.
	//
	// handle[in] - CMI_HANDLE value
	//
	// Return value - CMI_ERROR_INVALID_HANDLE
	//                CMI_SUCCESS

    int _CMI_EXPORT cmi_getBufferSize(CMI_HANDLE handle, int numPreviewFrames,
										int maxNumUnselectedImages, int *bufferSize);
	// Get buffer size for the specified number of preview frames (1 frame contains one eye image)
	// and iris images captured. "maxNumUnselectedImages" indicates maximum number of captured
	// but not selected images the application wants to keep. This is for DMX/EMX only.
	// If numPreviewFrames == 0, the event CMI_EVENT_PREVIEW_FRAME will contain the image
	// information only without the image itself.
	// This function should be called after cmi_openDevice() and before cmi_startCapture().
	// Otherwise, CMI_ERROR_CALL_GET_BUFFER_SIZE_FIRST will be returned in cmi_startCapture().
	// After cmi_stopCapture() or cmi_forceCapture(), this function does not need to be called
	// again, if the allocated buffer is not freed.
	//
	// handle[in] - CMI_HANDLE value
	// numPreviewFrames[in] - maximum number of preview frames the queue can hold.
	// maxNumUnselectedImages[in] - maximum number of unselected captured images.
	// bufferSize[out] - necessary buffer size

	// Return value - CMI_ERROR_INVALID_HANDLE
	//                CMI_ERROR_IN_ARGUMENTS: input value is out of range
	//                    numPreviewFrames <= 128 && maxNumUnselectedImages <= 64
	//                CMI_SUCCESS

    int _CMI_EXPORT cmi_startCapture(CMI_HANDLE handle, int bufferSize, unsigned char *buffer);
	// Start capture.
	//
	// handle[in] - CMI_HANDLE value
	// bufferSize[in] - working buffer size allocated by application
	// buffer[in] - working buffer allocated by application
	//
	// Return value - CMI_ERROR_INVALID_HANDLE
	//                CMI_ERROR_CALL_GET_BUFFER_SIZE_FIRST
	//                CMI_ERROR_IN_ARGUMENTS
	//                CMI_ERROR_BUFFER_SIZE_TOO_SMALL
	//                CMI_ERROR_DEVICE_CLOSED
	//                CMI_ERROR_DEVICE_STARTED
	//                CMI_ERROR_EEPROM_VERSION_INVALID
	//                CMI_ERROR_FAIL_TO_SEND_COMMAND
	//                CMI_SUCCESS

    int _CMI_EXPORT cmi_releasePreviewBuffer(CMI_HANDLE handle, unsigned char *buffer);
	// Release preview buffer to re-use in the next CMI_EVENT_PREVIEW_FRAME_INFO event.
	// For each image pointer, leftImage, rightImage and faceImage, if it is not NULL
	// this function should be called separately.
	//
	// handle[in] - CMI_HANDLE value
	// buffer[in] - leftImage, rightImage, or faceImage in CMI_IMAGE_INFO
	// Return value - CMI_ERROR_INVALID_HANDLE
	//                CMI_ERROR_INVALID_BUFFER - buffer is invalid
	//                CMI_SUCCESS

    int _CMI_EXPORT cmi_stopCapture(CMI_HANDLE handle);
	// Stop capturing. It will clean the event queue and return after all images
	// go through. cmi_cancelPendingReadEvent() can be called if necessary.
	//
	// handle[in] - CMI_HANDLE value
	//
	// Return value - CMI_ERROR_INVALID_HANDLE
	//                CMI_ERROR_DEVICE_CLOSED
	//                CMI_ERROR_DEVICE_STOPPED
	//                CMI_ERROR_FAIL_TO_SEND_COMMAND
	//                CMI_SUCCESS

    int _CMI_EXPORT cmi_forceCapture(CMI_HANDLE handle);
	// Force to capture.  In Auto capture mode, this function forces to turn on white LED
	// if it is not on yet and it will wait until the white LED is on for at least 0.5 secs
	// before taking images. In manual mode, white LED is on just after cmi_startCapture()
	// is called and it also wait until the white LED is on at least 0.5 secs before capture.
	//
	// handle[in] - CMI_HANDLE value
	//
	// Return value - CMI_ERROR_INVALID_HANDLE
	//                CMI_ERROR_DEVICE_CLOSED
	//                CMI_ERROR_DEVICE_STOPPED
	//                CMI_ERROR_DEVICE_UPSIDE_DOWN
	//                CMI_ERROR_FAIL_TO_SEND_COMMAND
	//                CMI_ERROR_WAIT_TIMEOUT - wait 5 secs and failed to capture. It should not happen
    //                CMI_ERROR_INVALID_MODE
    //                CMI_SUCCESS

	//////////////////////////////////////////////////////////////////////////////////////
	// Getters and Setters
	// dt = 1/15 sec for DMX/EMX and dt = 1/16.5 for BMT
	//
	// Common Return values - CMI_ERROR_INVALID_HANDLE
	//                        CMI_SUCCESS
	//
	// Additional return values for some steers - CMI_ERROR_IN_ARGUMENTS
	//////////////////////////////////////////////////////////////////////////////////////

    int _CMI_EXPORT cmi_getWhichEye(CMI_HANDLE handle, int *whichEye);

    int _CMI_EXPORT cmi_setWhichEye(CMI_HANDLE handle, int whichEye);
	// Get/Set whichEye option
    // Valid in BMT/DMX/EMX model
	//
	// handle[in] - CMI_HANDLE value
    // whichEye[in/out] - CMI_LEFT_EYE, CMI_RIGHT_EYE, CMI_EITHER_EYE, CMI_BOTH_EYES(default)
	//
	// Additional Return value - CMI_ERROR_IN_ARGUMENTS: invalid whichEye

    int _CMI_EXPORT cmi_getIrisMargins(CMI_HANDLE handle, int *left, int *right, int *top, int *bottom);

    int _CMI_EXPORT cmi_setIrisMargins(CMI_HANDLE handle, int left, int right, int top, int bottom);
	// Get/Set IrisMargins option
	//
	// handle[in] - CMI_HANDLE value
	// left, right, top, bottom[in/out] - percentage margin to the iris radius

    int _CMI_EXPORT cmi_getGazeDetectionEnabled(CMI_HANDLE handle, int *enable);

    int _CMI_EXPORT cmi_setGazeDetectionEnabled(CMI_HANDLE handle, int enable);
	// Get/Set Gaze Detection Enable option
	//
	// handle[in] - CMI_HANDLE value
	// enable[in/out] - CMI_TRUE(default), CMI_FALSE
	//
	// Additional Return value - CMI_ERROR_IN_ARGUMENTS: invalid enable


    int _CMI_EXPORT cmi_getLivenessDetectionEnabled(CMI_HANDLE handle, int *enable);

    int _CMI_EXPORT cmi_setLivenessDetectionEnabled(CMI_HANDLE handle, int enable);
	// Get/Set Liveness Detection Enable option. Not implemented yet
	//
	// handle[in] - CMI_HANDLE value
	// enable[in/out] - CMI_TRUE, CMI_FALSE(default)
	//
	// Additional Return value - CMI_ERROR_IN_ARGUMENTS: invalid enable

    int _CMI_EXPORT cmi_getTargetIntensity(CMI_HANDLE handle, unsigned char *intensity);

    int _CMI_EXPORT cmi_setTargetIntensity(CMI_HANDLE handle, unsigned char intensity);
	// Get/Set target intensity of captured iris image (360x360 pixels).
	// Valid only in DMX/EMX model
	//
	// handle[in] - CMI_HANDLE value
	// intensity[in/out] - default is 150.

    int _CMI_EXPORT cmi_getMaxXYMovement(CMI_HANDLE handle, int *maxXYMovement);

    int _CMI_EXPORT cmi_setMaxXYMovement(CMI_HANDLE handle, int maxXYMovement);
	// Get/Set maximum movement allowed in XY plane during dt secs. Unit is micron (1/1000 mm)
	//
	// handle[in] - CMI_HANDLE value
	// maxXYMovement[in/out] - defaults are 1000u(DMX/EMX) and 500u(BMT).

    int _CMI_EXPORT cmi_getMaxZMovement(CMI_HANDLE handle, int *maxZMovement);

    int _CMI_EXPORT cmi_setMaxZMovement(CMI_HANDLE handle, int maxZMovement);
	// Get/Set maximum movement allowed in Z direction during dt secs. Unit is micron (1/1000 mm)
	// DMX/EMX only
	//
	// handle[in] - CMI_HANDLE value
	// maxZMovement[in/out] - defaults are 12000u.

    int _CMI_EXPORT cmi_getBMTMode(CMI_HANDLE handle, int *mode);

    int _CMI_EXPORT cmi_setBMTMode(CMI_HANDLE handle, int mode);
	// Get/Set BMT capture mode.
	//
	// handle[in] - CMI_HANDLE value
    // mode[in/out] - CMI_BMT_AUTO_MODE(default), CMI_BMT_MANUAL_MODE, CMI_BMT_FAST_AUTO_MODE
    //                CMI_BMT_USER_MODE
	//
	// Additional Return value - CMI_ERROR_IN_ARGUMENTS: invalid mode

    int _CMI_EXPORT cmi_getLeftOffset(CMI_HANDLE handle, int *leftXOffset, int *leftYOffset);

    int _CMI_EXPORT cmi_setLeftOffset(CMI_HANDLE handle, int leftXOffset, int leftYOffset);
	// Get/Set the offset of left iris image in BMT manual mode
	//
	// handle[in] - CMI_HANDLE value
	// leftXOffset, leftYOffset[in/out] - x, y offset of 640x480 image in 880x520 image
	//
	// Additional Return value - CMI_ERROR_IN_ARGUMENTS: invalid range

    int _CMI_EXPORT cmi_getRightOffset(CMI_HANDLE handle, int *rightXOffset, int *rightYOffset);

    int _CMI_EXPORT cmi_setRightOffset(CMI_HANDLE handle, int rightXOffset, int rightYOffset);
	// Get/Set the offset of right iris image in BMT manual mode
	//
	// handle[in] - CMI_HANDLE value
	// rightXOffset, rightYOffset[in/out] - x, y offset of 640x480 image in 880x520 image
	//
	// Additional Return value - CMI_ERROR_IN_ARGUMENTS: invalid range

    int _CMI_EXPORT cmi_getIndicatorBrightness(CMI_HANDLE handle, unsigned char *red, unsigned char *green,
									 unsigned char *brightGreen, unsigned char *blue);

    int _CMI_EXPORT cmi_setIndicatorBrightness(CMI_HANDLE handle, unsigned char red, unsigned char green,
									 unsigned char brightGreen, unsigned char blue);
	// Get/Set the RGB indicator brightness. brightGreen is only valid in DMX/EMX
	//
	// handle[in] - CMI_HANDLE value
	// red, green, brightGreen, blue[in/out] - RGB brightness. 0 ~ 255

    int _CMI_EXPORT cmi_getWhiteBrightness(CMI_HANDLE handle, unsigned char *white);

    int _CMI_EXPORT cmi_setWhiteBrightness(CMI_HANDLE handle, unsigned char white);
	// Get/Set white LED brightness in BMT
	//
	// handle[in] - CMI_HANDLE value
	// red, green, brightGreen, blue[in/out] - white brightness. 0 ~ 255

    int _CMI_EXPORT cmi_getOperatingRangeCenter(CMI_HANDLE handle, int *operatingRangeCenter);
	// Get the center of the operating range in mm in DMX/EMX
	//
	// handle[in] - CMI_HANDLE value
	// operatingRangeCenter[out] - current value is 330mm

    int _CMI_EXPORT cmi_getDepthOfField(CMI_HANDLE handle, int *depthOfField);
	// Get the depth of field in mm in DMX/EMX
	//
	// handle[in] - CMI_HANDLE value
	// depthOfField[out] - current value is 30mm



    //////////////////////////////////////////////////////////////////////////////////////////////////
    // Smart Sensors  Ltd. MIRLIN Algorithm
    //////////////////////////////////////////////////////////////////////////////////////////////////

    int _CMI_EXPORT cmi_mir_getVersion(CMI_HANDLE handle, DWORD *major, DWORD *minor, DWORD *revision);
    // Get version of MIRLIN Algorithm. The current version number is 2.32.0
    //
    // handle[in] - CMI_HANDLE value
    // major[out]
    // minor[out]
    // revision[out]

    int _CMI_EXPORT cmi_mir_getLicenseLevel(CMI_HANDLE handle, int *level);
    // Get License Level of MIRLIN Algorithm. Currently there is only one level, which is 1
    //
    // handle[in] - CMI_HANDLE value
    // level[out] - 0 (MIRLIN disabled), 1 (MIRLIN enabled)

    int _CMI_EXPORT cmi_mir_getEnrolTemplates(CMI_HANDLE handle, unsigned char *leftEnrolTemplate,
                                              unsigned char *rightEnrolTemplate, int templateSize,
                                              CMI_IMAGE_INFO *imageInfo, int showSegmentation);
    // Get enroll templates generated by MIRLIN Algorithm
    //
    // handle[in] - CMI_HANDLE value
    // leftEnrolTemplate[out] - 579 bytes template
    // rightEnrolTemplate[out] - 579 bytes template
    // templateSize[in] - should be CMI_MIR_ENROL_TEMPLATE_SIZE = 579
    // imageInfo[in] - CMI_IMAGE_INFO structure returned from the event CMI_EVENT_IRIS_IMAGES_SELECTED
    // showSegmentation[in] - whether to show the iris and pupil segmentation on the images in imageInfo
    //                        CMI_MIR_DO_NOT_SHOW_SEGMENTATION or CMI_MIR_SHOW_SEGMENTATION
    //
    // Additional Return value - CMI_MIR_ERROR_UNKNOWN_LICENSE: cannot read the license
    //                           CMI_MIR_ERROR_INVALID_LICENSE: no MIRLIN license in the device
    //                           CMI_MIR_ERROR_WRONG_TEMPLATE_SIZE: wrong template size
    //                           CMI_MIR_ERROR_NO_INPUT_IMAGES: both iris images are NULL
    //                           CMI_MIR_ERROR_FAIL_TO_GENERATE_TEMPLATES: fail to generate both templates
    //                           CMI_MIR_LEFT_TEMPLATE_GENERATED_ONLY: only left iris template was generated
    //                           CMI_MIR_RIGHT_TEMPLATE_GENERATED_ONLY: only right iris template was generated
    //                           CMI_SUCCESS: both iris templates were generated

    int _CMI_EXPORT cmi_mir_getMatchTemplates(CMI_HANDLE handle, unsigned char *leftMatchTemplate,
                                              unsigned char *rightMatchTemplate, int templateSize,
                                              CMI_IMAGE_INFO *imageInfo, int showSegmentation);
    // Get match templates generated by MIRLIN Algorithm
    //
    // handle[in] - CMI_HANDLE value
    // leftMatchTemplate[out] - templateSize bytes template
    // rightMatchTemplate[out] - templateSize bytes template
    // templateSize[in] - should be one of the following sizes. The default is 7479 for 13 rotations
    //                    CMI_MIR_MATCH_ROT_0_TEMPLATE_SIZE        579   // 0 rotations
    //                    CMI_MIR_MATCH_ROT_5_TEMPLATE_SIZE        2879  // 5 rotations
    //                    CMI_MIR_MATCH_ROT_7_TEMPLATE_SIZE        4029  // 7 rotations
    //                    CMI_MIR_MATCH_ROT_9_TEMPLATE_SIZE        5179  // 9 rotations
    //                    CMI_MIR_MATCH_ROT_13_TEMPLATE_SIZE       7479  // 13 rotations - default
    //                    CMI_MIR_MATCH_ROT_25_TEMPLATE_SIZE       14379 // 25 rotations
    //
    // imageInfo[in] - CMI_IMAGE_INFO structure returned from the event CMI_EVENT_IRIS_IMAGES_SELECTED
    // showSegmentation[in] - whether to show the iris and pupil segmentation on the images in imageInfo
    //                        CMI_MIR_DO_NOT_SHOW_SEGMENTATION or CMI_MIR_SHOW_SEGMENTATION
    //
    // Additional Return value - CMI_MIR_ERROR_UNKNOWN_LICENSE: cannot read the license
    //                           CMI_MIR_ERROR_INVALID_LICENSE: no MIRLIN license in the device
    //                           CMI_MIR_ERROR_WRONG_TEMPLATE_SIZE: wrong template size
    //                           CMI_MIR_ERROR_NO_INPUT_IMAGES: both iris images are NULL
    //                           CMI_MIR_ERROR_FAIL_TO_GENERATE_TEMPLATES: fail to generate both templates
    //                           CMI_MIR_LEFT_TEMPLATE_GENERATED_ONLY: only left iris template was generated
    //                           CMI_MIR_RIGHT_TEMPLATE_GENERATED_ONLY: only right iris template was generated
    //                           CMI_SUCCESS: both iris templates were generated

    int _CMI_EXPORT cmi_mir_compareTemplate(CMI_HANDLE handle, unsigned char *enrolTemplate,
                                            unsigned char *matchTemplate, int matchTemplateSize,
                                            float *hammingDistance);
    // Compare enroll template with match template by MIRLIN Algorithm
    //
    // handle[in] - CMI_HANDLE value
    // enrolTemplate[in] - 579 bytes template
    // matchTemplate[in] - matchTemplateSize bytes template
    // matchTemplateSize[in] - matchTemplate size, which should be one of match template sizes defined above.
    // hammingDistance[out] - hamming distance between two templates. If both templates are perfectly matched
    //                        the value is 0 and it is 0.5 if two templates are totally non-matched.
    //                        The recommended threshold is 0.15
    //
    // Additional Return value - CMI_MIR_ERROR_UNKNOWN_LICENSE: cannot read the license
    //                           CMI_MIR_ERROR_INVALID_LICENSE: no MIRLIN license in the device
    //                           CMI_MIR_ERROR_WRONG_TEMPLATE_SIZE: wrong template size
    //                           CMI_MIR_ERROR_EMPTY_ENROL_TEMPLATE: enrol template is all zeros.
    //                             This is useful when one valid iris template is enrolled and the other
    //                             iris enrol template is stored as zeros.
    //                           CMI_MIR_ERROR_FAIL_TO_COMPARE_TEMPLATES: fail to compare two templates

    //////////////////////////////////////////////////////////////////////////////////////////////////
    // End of Smart Sensors Ltd. MIRLIN Algorithm
    //////////////////////////////////////////////////////////////////////////////////////////////////



    int _CMI_EXPORT cmi_getIndicatorEventEnabled(CMI_HANDLE handle, int *enabled);

    int _CMI_EXPORT cmi_setIndicatorEventEnabled(CMI_HANDLE handle, int enabled);
    // Get/Set color indicator event enabled or disabled
    //
    // handle[in] - CMI_HANDLE value
    // enabled[in/out] - CMI_TRUE, CMI_FALSE

    int _CMI_EXPORT cmi_getVoiceCueTimeInterval(CMI_HANDLE handle, int *firstTimeInterval_frames,
                                                  int *nextTimeInterval_frames);

    int _CMI_EXPORT cmi_setVoiceCueTimeInterval(CMI_HANDLE handle, int firstTimeInterval_frames,
                                                  int nextTimeInterval_frames);
    // Get/Set voice cue time interval.
    // firstTimeInterval_frames is the time interval for the first voice cue in number of frames.
    // 1 sec corresponds to 30 frames. default is 75 frames (2.5 sec)
    // nextTimeInterval_frames is the time interval for the rest of voice cues, whose default
    // is 90 frames (3 sec).
    //
    // handle[in] - CMI_HANDLE value
    // firstTimeInterval_frames[in/out] - If it is less than 15, CMI_ERROR_IN_ARGUMENTS will return
    // nextTimeInterval_frames[in/out] - If it is less than 15, CMI_ERROR_IN_ARGUMENTS will return

    int _CMI_EXPORT cmi_getMaxFrameDiffInEitherEyeMode(CMI_HANDLE handle, int *maxFrameDiffinEitherEyeMode);

    int _CMI_EXPORT cmi_setMaxFrameDiffInEitherEyeMode(CMI_HANDLE handle, int maxFrameDiffinEitherEyeMode);
    // Get/Set maximum number of frames difference allowed for both eyes capture in "Either Eye" mode.
    // 3 to 9 is allowed and the values outside this range will result in CMI_ERROR_IN_ARGUMENTS return.
    // If this value is set to 3, it only capture both eyes if the frame difference is 3 and it will capture
    // one eye otherwise. This is the fastest "Either Eye" capture mode.
    //
    // handle[in] - CMI_HANDLE value
    // maxFrameDiffinEitherEyeMode[in/out] - If it not in the range of 3 to 9, CMI_ERROR_IN_ARGUMENTS will return
    //                                       Default is 9.

    int _CMI_EXPORT cmi_getFaceCaptureMode(CMI_HANDLE handle, int *faceCaptureMode);

    int _CMI_EXPORT cmi_setFaceCaptureMode(CMI_HANDLE handle, int faceCaptureMode);
    // Get/Set face image capture mode. This function should be called before cmi_openDevice().
    // CMI_FACE_FULL_RESOLUTION is only supported in Windows and it should be set for color face image
    // in EMX-30 or DMX-30.
    // Once the face full resolution bayer image is captured, it can be converted
    // to color image with the library cmicolorLib.dll. The full resolution is 720x960
    // and the half resolution is 360x480
    //
    // handle[in] - CMI_HANDLE value
    // faceCaptureMode[in/out] - CMI_FACE_FULL_RESOLUTION or CMI_FACE_HALF_RESOLUTION
    //                           default is CMI_FACE_HALF_RESOLUTION


    int _CMI_EXPORT cmi_getMotorReturnToInitEnabled(CMI_HANDLE handle, int *enabled);

    int _CMI_EXPORT cmi_setMotorReturnToInitEnabled(CMI_HANDLE handle, int enabled);
    // Get/Set whether to return to initial motor position after the images are captured in EMX/EMB/EMA
    // The default is enabled. If this is diabled, it will stay at the last position after the images
    // are captured and this may be useful to verify a user after successful enrollment.
    // cmi_setMotorReturnToInitEnabled() should be called before cmi_startCapture().
    //
    // handle[in] - CMI_HANDLE value
    // enabled[in/out] - CMI_TRUE, CMI_FALSE

    int _CMI_EXPORT cmi_getInOutDepthOfField(CMI_HANDLE handle, int *inDepthOfField,
                                          int *outDepthOfField);

    int _CMI_EXPORT cmi_setInOutDepthOfField(CMI_HANDLE handle, int inDepthOfField,
                                           int outDepthOfField);
    // Get/Set the inside and outside depth of field.
    // The center of depth of field is 330mm from the front surface of EMX.
    // The minimum and default of each depth of field is 15mm, so total depth of field is 30mm.
    // This is the recommended depth of field for enrollment. The maximum is 35mm for inside
    // and 25mm for outside, total maximum depth of field is 60mm, which is for recognition
    // for physical access control application.
    //
    // handle[in] - CMI_HANDLE value
    // insideDepthOfField[in/out] - If it not in the range of 15 to 35, CMI_ERROR_IN_ARGUMENTS will
    //                              be returned. Default is 15(mm).
    // outsideDepthOfField[in/out] - If it not in the range of 15 to 25, CMI_ERROR_IN_ARGUMENTS will
    //                              be returned. Default is 15(mm).
    // Additional Return value - CMI_ERROR_DEVICE_STARTED: when the set function is called after start.

    int _CMI_EXPORT cmi_getPSDEnabled(CMI_HANDLE handle, int *enabled);

    int _CMI_EXPORT cmi_setPSDEnabled(CMI_HANDLE handle, int enabled);
    // Enable/disable the proximity sensor to trigger start capture in EMA and EMB.
    // This fusnction does not have any effect on EMX. The default is enabled.
    //
    // handle[in] - CMI_HANDLE value
    // enabled[in/out] - CMI_TRUE, CMI_FALSE


#ifdef __cplusplus
} // extern "C"
#endif

#endif // CMIRISLIB2_H
