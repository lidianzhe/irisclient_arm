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
// cmiglobal.h
//
// Version 1.0.0   March 27, 2012
//                 Initial Public Release
//
// Version 1.0.9   July. 2, 2013
//  EMA-30/EMB-30:
//  - Added tamper switch and function key event in Device Manager Event
//
// Description: cmiglobal.h defines the constants and struct for cmirislib.h
//              Please see cmirislib.h for detailed interface.
//
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CMIGLOBAL_H
#define CMIGLOBAL_H

#if defined(__linux__)
#include <stdint.h>
typedef uint32_t DWORD;
#endif

////////////////////////////////////////////////////////////////////////////////////////
// DMX, EMX model and BMT model
// used for "model" in cmi_getCMIrisLibVersion(int *major, int *minor, int *revision)
////////////////////////////////////////////////////////////////////////////////////////

#define CMI_DMX_EMX_MODEL         0

#define CMI_BMT_MODEL             1

////////////////////////////////////////////////////////////////////////////////////////
// Model name and serial number
////////////////////////////////////////////////////////////////////////////////////////

#define CMI_MODEL_NAME_MAX_SIZE    8   // ex. BMT-20

#define CMI_SERIAL_NUMBER_MAX_SIZE 13  // ex. BA1205A000123

#define CMI_HARDWARE_REV_MAX_SIZE  11  // ex. 1.0.1

#define CMI_FIRMWARE_REV_MAX_SIZE  11  // ex. 1.1.13

typedef struct _CMI_DEVICE_INFO {
	int cbSize;                                       // size of this struct
	char modelName[CMI_MODEL_NAME_MAX_SIZE+1];
	char serialNumber[CMI_SERIAL_NUMBER_MAX_SIZE+1];
	char hardwareRev[CMI_HARDWARE_REV_MAX_SIZE+1];
	char firmwareRev[CMI_FIRMWARE_REV_MAX_SIZE+1];
} CMI_DEVICE_INFO;

////////////////////////////////////////////////////////////////////////////////////////
// Iris margins in the image
////////////////////////////////////////////////////////////////////////////////////////

#define CMI_MARGINS_FAIL         0             // Cannot find the eye

#define CMI_LEFT_OK              (1<<0)

#define CMI_RIGHT_OK             (1<<1)

#define CMI_TOP_OK               (1<<2)

#define CMI_BOTTOM_OK            (1<<3)

#define CMI_ALL_MARGINS_OK       (CMI_LEFT_OK | CMI_RIGHT_OK | CMI_TOP_OK | CMI_BOTTOM_OK)

////////////////////////////////////////////////////////////////////////////////////////
// Option to capture which eye
////////////////////////////////////////////////////////////////////////////////////////

#define CMI_LEFT_EYE                            (1<<0)

#define CMI_RIGHT_EYE                           (1<<1)

#define CMI_BOTH_EYES                           (CMI_LEFT_EYE | CMI_RIGHT_EYE)

#define CMI_EITHER_EYE                          (1<<2 | CMI_BOTH_EYES)

////////////////////////////////////////////////////////////////////////////////////////
// Option to capture face image
////////////////////////////////////////////////////////////////////////////////////////

#define CMI_FACE_FULL_RESOLUTION                 1

#define CMI_FACE_HALF_RESOLUTION                 3

////////////////////////////////////////////////////////////////////////////////////////
// boolean type information
////////////////////////////////////////////////////////////////////////////////////////

#define CMI_TRUE                                 1

#define CMI_FALSE                                0

#define CMI_UNKNOWN                             -1

/////////////////////////////////////////////////////////////////////////////////////////
// BMT Model Specific
/////////////////////////////////////////////////////////////////////////////////////////

// In BMT-20, the captured image size of each eye is 880x520, but the area of 640x480
// pixels will be returned as an captured iris image.
// BMT cannot measure the distance of each eye from the device,
// but it will estimate the eye position in 3 states; far, close, and in range.
// If the distance cannot be estimated, it will return CMI_EYE_UNKNOWN.
// The current version of CMIRIS SDK always return CMI_EYE_UNKNOWN.

#define CMI_EYE_UNKNOWN          -1

#define	CMI_EYE_FAR              0

#define CMI_EYE_CLOSE            1

#define CMI_EYE_IN_RANGE         2

// BMT Mode (Max image size is 880x520) - Windows only
#define CMI_BMT_AUTO_MODE        0     // Automatic capture unless cmi_forceCapture() is called

#define CMI_BMT_MANUAL_MODE      1     // Captures only when cmi_forceCapture() is called

#define CMI_BMT_FAST_AUTO_MODE   2     // Automatic fast capture unless cmi_forceCapture() is called
                                       // added in V1.0.5
#define CMI_BMT_USER_MODE        3     // Full resolution image will be provided in preview event.
                                       // Application should select the images and call cmi_stopCapture()
                                       // added in V1.0.6

// BMT Low Bandwidth Mode (Max image size is 640x480) - Windows/Linux/Android. Added in V1.0.82
#define CMI_BMT_AUTO_LB_MODE      16   // Low bandwidth, Automatic capture unless cmi_forceCapture() is called

#define CMI_BMT_MANUAL_LB_MODE    17   // Low bandwidth, Captures only when cmi_forceCapture() is called

#define CMI_BMT_FAST_AUTO_LB_MODE 18   // Low bandwidth, Automatic fast capture unless cmi_forceCapture() is called

///////////////////////////////////////////////////////////////////////////////////////////////

typedef void *CMI_HANDLE;

typedef struct _CMI_IMAGE_INFO {
	int cbSize;                                    // size of this struct

	//----------------------------------------------------------------------------
	// Image info.
	// if xxxxImage == NULL, image is not available, and width, height are 0
	// For BMT, faceImage is always NULL
	// For DMX/EMX, the current version SDK does not provide the face image.
	//----------------------------------------------------------------------------
	unsigned char *leftImage, *rightImage, *faceImage;
	int leftWidth, rightWidth, faceWidth;
	int leftHeight, rightHeight, faceHeight;
	// resolution is sub sampling factor
	// full resolution: 1, half resolution: 2, quad resolution: 4, ...
    // for color face image, value is + 16
	int leftResolution, rightResolution, faceResolution;
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	// Iris information in the image
	// If not avaiable, the "int" values will be -1 or CMI_UNKNOWN
	//----------------------------------------------------------------------------
	int leftIrisCenterX, rightIrisCenterX;
	int leftIrisCenterY, rightIrisCenterY;
	int leftIrisRadius, rightIrisRadius;               // pixels
	int leftPupilRadius, rightPupilRadius;             // pixels - TBI(To Be Implemented)
	int leftExposedIrisArea, rightExposedIrisArea;     // percentage - TBI
	unsigned char leftIrisMargin;                      // enough iris margins? see enum IrisMargin
	unsigned char rightIrisMargin;                     // enough iris margins? see enum IrisMargin
	int leftLiveness, rightLiveness;                   // Live eye detection result - TBI
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	// The eyes position information in 3D space
	//----------------------------------------------------------------------------
	int leftDistance;          // Distance(unit: micron) of the left eye from the front of device.
	int rightDistance;         // DMX/EMX only

    int leftPosition;           // Estimated eye position in BMT
    int rightPosition;          // Indicator LED color in EMX family

	int leftXYMovement;         // XY Movement of Eye in micron during dt seconds
	int rightXYMovement;        // XY-plane is perpendicular to the optical axis of camera

	int leftZMovement;          // Z Movement of Eye in micron during dt seconds. DMX/EMX only
	int rightZMovement;         // +Z-direction is from the device to the eye.

	int ipd;                     // Inter-Pupilary Distance in micron. DMX/EMX only
	int rollAngle;               // Roll angle of a line drawn between the centers of the left
								 // and right irises. Unit is 1/10 degree and it is positive if the
								 // left iris is higher than right iris. (counter-clockwise)
								 // TBI
	int rollAngleUncertainty;    // Roll angle uncertainty in 1/10 degrees. TBI
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	// Additional Information - CMI_YES, CMI_NO, CMI_DONT_KNOW
	//----------------------------------------------------------------------------
	int doesLeftLookFront;       // Does left eye look front?
	int doesRightLookFront;      // Does right eye look front?

	int isLeftImageQualityOK;    // is left/right iris image quality OK?
	int isRightImageQualityOK;

	int isDeviceUpSideDown;      // Is device up side down? BMT only
	int isDeviceStablized;       // Is device stablized - TBI

	int avgProcessingTime;       // average image processing time in ms for each capured frame.
} CMI_IMAGE_INFO;


typedef struct _CMI_EVENT {
	int cbSize;
	int event;
	CMI_IMAGE_INFO imageInfo;
} CMI_EVENT;

typedef struct _CMI_DM_EVENT {
	int cbSize;
	int event;
	CMI_DEVICE_INFO deviceInfo;
} CMI_DM_EVENT;

////////////////////////////////////////////////////////////////////////////////////////
// Return values
////////////////////////////////////////////////////////////////////////////////////////

#define CMI_SUCCESS                              0

#define CMI_ERROR_WAIT_TIMEOUT                  -1  // Timeout Error

#define CMI_ERROR_READ_EVENT_CANCELLED          -2

#define CMI_ERROR_INVALID_HANDLE                -3  // Invalid CMI_HANDLE in argument

#define CMI_ERROR_INVALID_MODEL                 -4

#define CMI_ERROR_FAIL_TO_SEND_COMMAND          -5

#define CMI_ERROR_INVALID_BUFFER                -6

#define CMI_ERROR_CALL_GET_BUFFER_SIZE_FIRST    -7

#define CMI_ERROR_BUFFER_SIZE_TOO_SMALL         -8

#define CMI_ERROR_CANNOT_FIND_DEVICE            -9

#define CMI_ERROR_DEVICE_OPENED                 -10  // try to open when the device is already opened

#define CMI_ERROR_DEVICE_CLOSED                 -11  // try to do action when the device is already closed

#define CMI_ERROR_DEVICE_STARTED                -12  // try to start when the device have already started

#define CMI_ERROR_DEVICE_STOPPED                -13  // Capture process has not started yet.

#define CMI_ERROR_DEVICE_UPSIDE_DOWN            -14  // when forceCapture() is called.

#define CMI_ERROR_IN_ARGUMENTS                  -15  // Error in arguments

#define CMI_ERROR_FAIL_TO_OPEN_IMAGER_DEVICE    -16  // maybe USB cable is not well connected when cmi_openDevice()

#define CMI_ERROR_EEPROM_READ_TIMEOUT           -17  // Time out in reading EEPROM during cmi_openDevice().
													 // cmi_closeDevice() should be called in the applicaion
// EMX model only
#define CMI_ERROR_FAIL_TO_OPEN_MOTOR_DEVICE     -18 // Failed to open Motor Device in cmi_openDevice().
													// Capture will work although motor is not working

#define CMI_ERROR_EEPROM_VERSION_INVALID        -19 // EEPROM version is invalid in cmi_openDevice()
													// cmi_closeDevice() have to be called in the applicaion
// DMX EMX only
#define CMI_ERROR_CANNOT_FIND_HAARCASCADE_FILES -20 // Cannot find ./haarcasecade/haarcascade_frontalface_alt.xml
													// cmi_closeDevice() have to be called in the applicaion

#define CMI_ERROR_CANNOT_ALLOC_MEMORY           -21 // If you get this error after calling cmi_startCapture(),
													// cmi_closeDevice() should be called in the application

#define CMI_ERROR_INVALID_MODE                  -22 // cmi_forceCapture() will return if this function is called
                                                    // in CMI_BMT_USER_MODE. Added in V1.0.6
#define CMI_ERROR_UDEV_FAILED                   -23 // libudev failed. libudev is to detect device arrival
                                                    // and removal in Linux

#define CMI_ERROR_BEGINTHREAD_FAILED            -24 // Window message pumping thread failed


////////////////////////////////////////////////////////////////////////////////////////
// Event value for CMI_EVENT structure (Image capture event)
////////////////////////////////////////////////////////////////////////////////////////

#define CMI_EVENT_PREVIEW_FRAME_INFO             0  // Intermediate left or right Iris Image for display
													// and image information

#define CMI_EVENT_CAPTURE_DONE                   1  // Capture finished and can play the capture sound
													// CMI_IMAGE_INFO leftImage and rightImage are NULL.
// CMI_DMX_EMX_MODEL model only
#define CMI_EVENT_IRIS_IMAGE_UNSELECTED          2  // Unselected captured Image within Operating Range
													// This event repeats for all unselected images
													// after capture finished

#define CMI_EVENT_IRIS_IMAGES_SELECTED           3  // Selected left and right Iris Images are returned
													// IPD will be available for DMX and EMX
// Error occured during image capture
#define CMI_EVENT_ERROR_VIDEO_STREAM_MISMATCH    4  // Internal error (possibly hardware problem)
													// Application should call cmi_closeDevice().

#define CMI_EVENT_INDICATOR                      64 // When color indicator is changed or
                                                    // voice cue is necessary

////////////////////////////////////////////////////////////////////////////////////////
// Event value for CMI_DM_EVENT structure (Device Manager event)
////////////////////////////////////////////////////////////////////////////////////////

#define CMI_EVENT_DEVICE_ARRIVAL                 5  // Device arrived

#define CMI_EVENT_DEVICE_REMOVAL                 6  // Device is removed

#define CMI_EVENT_POWER_SUSPENDED                7  // PC power is suspended. close() will be called internally

#define CMI_EVENT_POWER_RESUMED                  8

////////////////////////////////////////////////////////////////////////////////////////
// Event value for CMI_DM_EVENT structure (PSD Status) - EMA/EMB only
////////////////////////////////////////////////////////////////////////////////////////

#define CMI_EVENT_PSD_OBJECT_IN_RANGE            9

#define CMI_EVENT_PSD_OBJECT_OUT_RANGE           10

////////////////////////////////////////////////////////////////////////////////////////
// Event value for CMI_DM_EVENT structure (Tamper switch and Function Key) - EMA/EMB only
////////////////////////////////////////////////////////////////////////////////////////

#define CMI_EVENT_TAMPER_SWITCH_OPEN             11

#define CMI_EVENT_TAMPER_SWITCH_CLOSED           12

#define CMI_EVENT_MODE_KEY_PRESSED               13

#define CMI_EVENT_MODE_KEY_RELEASED              14

#define CMI_EVENT_FUNC_KEY_1_PRESSED             15

#define CMI_EVENT_FUNC_KEY_1_RELEASED            16

#define CMI_EVENT_FUNC_KEY_2_PRESSED             17

#define CMI_EVENT_FUNC_KEY_2_RELEASED            18

#define CMI_EVENT_FUNC_KEY_3_PRESSED             19

#define CMI_EVENT_FUNC_KEY_3_RELEASED            20

#define CMI_EVENT_FUNC_KEY_4_PRESSED             21

#define CMI_EVENT_FUNC_KEY_4_RELEASED            22

#define CMI_EVENT_MOVE_UP_PRESSED                23

#define CMI_EVENT_MOVE_UP_RELEASED               24

#define CMI_EVENT_MOVE_DOWN_PRESSED              25

#define CMI_EVENT_MOVE_DOWN_RELEASED             26

////////////////////////////////////////////////////////////////////////////////////////
// Current Indicator LED color - leftPosition/rightPosition in CMI_IMAGE_INFO, EMX/EMA/EMB only
////////////////////////////////////////////////////////////////////////////////////////

#define CMI_INDICATOR_COLOR_OFF                   0

#define CMI_INDICATOR_COLOR_RED                   1

#define CMI_INDICATOR_COLOR_GREEN                 2

#define CMI_INDICATOR_COLOR_BLUE                  3

#define CMI_VOICE_COME_CLOSER                     4

#define CMI_VOICE_MOVE_BACK                       5

////////////////////////////////////////////////////////////////////////////////////////
// Color face Image Size - EMX/EMA/EMB only
////////////////////////////////////////////////////////////////////////////////////////

#define CMI_FACE_FULL_WIDTH                    720

#define CMI_FACE_FULL_HEIGHT                   960

////////////////////////////////////////////////////////////////////////////////////////
// Color face Image Type - EMX/EMA/EMB only
////////////////////////////////////////////////////////////////////////////////////////

#define CMI_FACE_MONO_FULL_RESOLUTION            1

#define CMI_FACE_MONO_HALF_RESOLUTION            2

#define CMI_FACE_BAYER_FULL_RESOLUTION          17

#endif // CMIGLOBAL_H
