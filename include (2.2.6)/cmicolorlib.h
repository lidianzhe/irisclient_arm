/////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2011~2013 CMITECH Co., Ltd.
// All rights reserved.
//
// Licensees holding valid CMITECH's CMIRIS SDK licenses may use this file in
// accordance with the CMIRIS SDK License Agreement provided with the
// Software or, alternatively, in accordance with the terms contained in
// an agreement between you and CMITECH.
////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
// cmicolorlib.h
//
// Version 0.9.0   Nov. 4, 2013
//                 Initial beta Release
//
// Description: cmicolorlib.h provide the color conversion utility of the face image
//              captured by DMX-30, EMX-30, EMB-30, and EMA-30 developed by CMITECH.
//
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CMICOLORLIB_H
#define CMICOLORLIB_H

#ifdef WIN32
  #include <windows.h>
  #if defined(CMICOLORLIB_LIBRARY)
    #define CMICOLORLIBSHARED_EXPORT __declspec(dllexport) __stdcall
  #else
    #define CMICOLORLIBSHARED_EXPORT __declspec(dllimport) __stdcall
  #endif
#else
  #define CMICOLORLIBSHARED_EXPORT
#endif

#if defined(__linux__)
#include <stdint.h>
typedef uint32_t DWORD;
#endif

#define _CMIC_EXPORT CMICOLORLIBSHARED_EXPORT

#define CMICOLORLIB_MAJOR_VERSION         0
#define CMICOLORLIB_MINOR_VERSION         9
#define CMICOLORLIB_REVISION              0

typedef void *CMIC_HANDLE;

////////////////////////////////////////////////////////////////////////////////////////
// Return values
////////////////////////////////////////////////////////////////////////////////////////

#define CMIC_SUCCESS                              0

#define CMIC_ERROR_INVALID_HANDLE                -1  // Invalid CMI_HANDLE in argument

#define CMIC_ERROR_CANNOT_ALLOC_MEMORY           -2


#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

    void _CMIC_EXPORT  cmic_getCMIColorVersion(DWORD *major, DWORD *minor, DWORD *revision);
	// Get cmirislib version and revision number for compatibility.
	// Application should check if major version number is consistent.
	//
	// major[out]
	// minor[out]
	// revision[out]

    int _CMIC_EXPORT cmic_createCMIColor(CMIC_HANDLE *phandle);
    // Create CMIColor.
	//
    // phandle[out] - CMIC_HANDLE value if successful. Otherwise, NULL.
    // Return value - CMIC_ERROR_CANNOT_ALLOC_MEMORY
    //                CMIC_SUCCESS

    int _CMIC_EXPORT cmic_destroyCMIColor(CMIC_HANDLE handle);
	// Destroy CMIris.
	//
    // handle[in] - CMIC_HANDLE value
	//
    // Return value - CMIC_ERROR_INVALID_HANDLE
    //                CMIC_SUCCES

    int _CMIC_EXPORT cmic_colorConversion(CMIC_HANDLE handle, unsigned char *bayerImage,
                                       int width, int height, unsigned char *colorImage);
    // Color conversion of face Bayer image captured by EMX-30 series to color image
    //
    // handle[in] - CMIC_HANDLE value
    // bayerImage[in] - 8bit Bayer image of raw format
    // width[in] - width of the Bayer image
    // height[in] - height of the Bayer image
    // colorImage[out] - 3 byte(R, G, B) color image of raw format with size of width/2 by height/2
    //      Need to allocate memory of 3*width/2*height/2 before call this function
    //
    // Return value - CMIC_ERROR_INVALID_HANDLE
    //                CMIC_SUCCES

	//////////////////////////////////////////////////////////////////////////////////////
	// Getters and Setters
    //
    // Common Return values - CMIC_ERROR_INVALID_HANDLE
    //                        CMIC_SUCCESS
	//
	//////////////////////////////////////////////////////////////////////////////////////

    int _CMIC_EXPORT cmic_getGamma(CMIC_HANDLE handle, float *gamma);
    int _CMIC_EXPORT cmic_setGamma(CMIC_HANDLE handle, float gamma);
    // Get/Set gamma correction value option
	//
    // handle[in] - CMIC_HANDLE value
    // gamma[in/out] - default is 1.4 and can be set to 2.2 depending on the display
	//

    int _CMIC_EXPORT cmic_getFaceRedGreenRatioMin(CMIC_HANDLE handle, float *faceRedGreenRatioMin);
    int _CMIC_EXPORT cmic_setFaceRedGreenRatioMin(CMIC_HANDLE handle, float faceRedGreenRatioMin);
    int _CMIC_EXPORT cmic_getFaceRedGreenRatioMax(CMIC_HANDLE handle, float *faceRedGreenRatioMax);
    int _CMIC_EXPORT cmic_setFaceRedGreenRatioMax(CMIC_HANDLE handle, float faceRedGreenRatioMax);

    int _CMIC_EXPORT cmic_getFaceBlueGreenRatioMin(CMIC_HANDLE handle, float *faceBlueGreenRatioMin);
    int _CMIC_EXPORT cmic_setFaceBlueGreenRatioMin(CMIC_HANDLE handle, float faceBlueGreenRatioMin);
    int _CMIC_EXPORT cmic_getFaceBlueGreenRatioMax(CMIC_HANDLE handle, float *faceBlueGreenRatioMax);
    int _CMIC_EXPORT cmic_setFaceBlueGreenRatioMax(CMIC_HANDLE handle, float faceBlueGreenRatioMax);
    // Get/Set Face Red/Green and Blue/Green Min and Max values for white balance
    //
    // handle[in] - CMIC_HANDLE value
    // faceRedGreenRatioMin[in/out] - default is 1.5
    // faceRedGreenRatioMax[in/out] - default is 1.8
    // faceBlueGreenRatioMin[in/out] - default is 0.6
    // faceBlueGreenRatioMax[in/out] - default is 0.9
    //

    int _CMIC_EXPORT cmic_getColorMatrixID(CMIC_HANDLE handle, float *colorMatrixID);
    int _CMIC_EXPORT cmic_setColorMatrixID(CMIC_HANDLE handle, float colorMatrixID);
    // Get/Set Face color matrix ambient light index for white balance.
    // -1: auto white balance
    // 0: incandescent light
    // 0 ~ 2: between incandescent and sun light
    // 2: sun
    //
    // handle[in] - CMIC_HANDLE value
    // colorMatrixID[in/out] - default is -1
    //

#ifdef __cplusplus
} // extern "C"
#endif

#endif // CMICOLORLIB_H
