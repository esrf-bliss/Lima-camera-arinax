/***************************************************************************/
/*                                                                         */
/*                            VideoServer.h                                */
/*                                                                         */
/***************************************************************************/
/* Design MAATEL SAS (F38430 Moirans France)                               */
/*-------------------------------------------------------------------------*/
/*                                                                         */
/***************************************************************************/
/* Created on     : 4 juil. 2016  DLA                                      */
/***************************************************************************/

#ifndef __VIDEOSERVER_H__
#define __VIDEOSERVER_H__

#include <stdint.h>
#include <cstddef>
namespace arinax_ns {
#ifdef __cplusplus
extern "C" {
#endif
#ifndef _WIN32
    #define __stdcall
#endif


#define VIDEOSERVERDLL_EXPORTS
#if defined _WIN32 || defined __CYGWIN__ || defined __MINGW32__
    #ifdef VIDEOSERVERDLL_EXPORTS
        #ifdef __GNUC__
            #define VIDEOSERVERDLL_API __attribute__ ((dllexport))
        #else
            #define VIDEOSERVERDLL_API __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
        #endif
    #else
        #ifdef __GNUC__
            #define VIDEOSERVERDLL_API __attribute__ ((dllimport))
        #else
            #define VIDEOSERVERDLL_API __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
        #endif
    #endif
#else
    #if __GNUC__ >= 4
        #define VIDEOSERVERDLL_API __attribute__ ((visibility ("default")))
    #else
        #define VIDEOSERVERDLL_API 
    #endif
#endif

const int GENICAM_OK = 0;
const int GENICAM_BUFFER_OVERFLOW = 5;
const int GENICAM_ACCESS_DENIED = 4;
const int GENICAM_ERROR = -1;
//const int BUFFER_SIZE = 256;
const int ENUM_NORMAL_SIZE = 16;

/**
* Structure containing a frame.
* It defines : the pixel Format as per GenICam PFNC. (plus extra addition in case of JPEG image)
* Width and height of the image, in pixels.
* payloadSize, in bits of the underlying data, needed to safely retrieve the data from the void* pointer.
* void* pointer to the raw data.
*/
struct ImageRaw
{
    int64_t m_pixelFormat;
    uint32_t m_height;
    uint32_t m_width;
    uint32_t m_payloadSize;
    uint32_t m_maxSize;
    int64_t m_timestamp;
    void* m_Image;
    
    explicit ImageRaw():m_pixelFormat(0)
        , m_height(0)
        , m_width(0)
        , m_payloadSize(0)
    {
    };
    
    ImageRaw& operator =(const ImageRaw& b)
    {
        m_pixelFormat = b.m_pixelFormat;
        m_height = b.m_height;
        m_width = b.m_width;
        m_timestamp = b.m_timestamp;
        //if(b.m_payloadSize > m_maxSize)
        //{
        //    m_maxSize = b.m_maxSize;
        //    m_payloadSize = b.m_payloadSize;
        //    delete[] m_Image;
        //    m_Image = new unsigned char[m_maxSize];
        //}
        if(m_payloadSize == 0)
        {
            m_Image = b.m_Image;
            return *this;
        }
        if(b.m_payloadSize != m_payloadSize)
        {
            m_payloadSize = b.m_payloadSize;
        }
//        memcpy(m_Image, b.m_Image, m_payloadSize);
        return *this;
    }
};

/** init, load the various producers. Take camerasNamesCallback, used to retrieve all the available cameras names.
* param camerasNameCallback, this callback is called when updateCamerasNames is called. It takes the camera
*  index number and the associated name as arguments
* param infoCallback, this callback is called when updateCameraInfo is called. It takes the camera
*  index number and a description of the camera(id, name …) as arguments
*  return 1, if the init finished properly.
*/
VIDEOSERVERDLL_API int32_t init(void (__stdcall *camerasNameCallback)(int32_t, const char*), void (__stdcall *infoCallback)(int32_t, const char*));

//isInitialized, return true if the producers are loaded.
VIDEOSERVERDLL_API int32_t isInitialized();

/** openCamera, open a camera using its userName, as defined in the videoStreamer ini file.
* param callback, it sends the images when grabbed.
* param idx the camera Index, use it to refer to the camera then.
* note that for a multi camera zoom setup, this value will be -1.
* return 1 if the camera was opened.
*/
VIDEOSERVERDLL_API const int32_t openCameraNotGrab(const char* cameraName
    , int32_t* idx); //open a camera based on the deviceId as defined in strDeviceId in CameraInfo. Gives back the pointer to the camera object created.
/** openCamera, open a camera using its userName, as defined in the videoStreamer ini file.
* param callback, it sends the images when grabbed.
* param idx the camera Index, use it to refer to the camera then.
* note that for a multi camera zoom setup, this value will be -1.
* return 1 if the camera was opened.
*/
VIDEOSERVERDLL_API const int32_t openCamera(const char* cameraName
    , void (__stdcall *callback)(ImageRaw*)
    , int32_t* idx); //open a camera based on the deviceId as defined in strDeviceId in CameraInfo. Gives back the pointer to the camera object created.

/**closeCamera, properly close a camera. openCamera must be called again if you want to reuse this camera.
* param idx, index of the camera to close.
*/
VIDEOSERVERDLL_API int32_t closeCamera(int32_t idx);

/**
 * Register the callbacks to get parameter value. One for double, one for long, and one for string that is also used for enumeration.
 * These callback will be called after the updateParameter function is called.
 * Each callback take the parameter name and its associated value as parameter
 * param idx, identifier of the camera associated with the callbacks.
 */
VIDEOSERVERDLL_API void registerParameterCallBacks(int32_t idx
    , void (__stdcall *doubleCallback)(const double, const char*)
    , void (__stdcall *intCallback)(const char*, const int64_t)
    , void (__stdcall *stringCallback)(const char*, const char*));

/*Register the callbacks to get parameter range. One for double, one for long, and one for string that is also used for enumeration. And one for zoomLevel, zoom, exposure, gain, and whitebalance parameter.
 * These callback will be called after the updateParameterRange function is called.
 * Each callback take the parameter name, the number of values returned(it is usually 2 or 3 for min, max and stepsize, but for enumeration it is the number of values), 
 * and an array to the values.
 * The whitebalance callback has an extra argument for the color that is used. It can be 'Red', 'Blue', or 'Green'.
 * param idx, identifier of the camera associated with the callbacks.
 */
VIDEOSERVERDLL_API void registerParameterRangeCallBacks(int32_t idx
    , void (__stdcall *doubleCallback)(const char*, int32_t, double*)
    , void (__stdcall *intCallback)(const char*, int32_t, int64_t*)
    , void (__stdcall *stringCallback)(const char*, int32_t, const char**)
    , void (__stdcall *zoomLevelCallback)(int32_t, double*)
    , void (__stdcall *zoomCallback)(int32_t, int32_t*)
    , void (__stdcall *predefZoomLevelCallback)(int32_t, double*)
    , void (__stdcall *exposureCallback)(int32_t, double*)
    , void (__stdcall *gainCallback)(int32_t, double*)
    , void (__stdcall *wbCallback)(const char*, int32_t, double*));

/**
 * Retrieve the identifier of all the cameras used in the zoom setup.
 * the callback sends the number of cameras, and an array with the indexes as callback.
 */
VIDEOSERVERDLL_API int32_t getZoomCamerasIndexes(void (__stdcall *idxsCallback)(const int32_t, int32_t*));

/*isParameterAvailable, is this parameter defined for this camera.
 * param idx camera identifier.
 * param name, parameter name.
 * return true if this parameter exist on the camera.
 */
VIDEOSERVERDLL_API int32_t isParameterAvailable(int32_t idx, const char* name);

/*updateParameterRange, call to ask for parameter named name range and then call the callback to update the value.
 * Note that the registerParameterRangeCallbacks function must be called before using this one.
 * param idx camera identifier.
 * param name, parameter name.
 */
VIDEOSERVERDLL_API int32_t updateParameterRange(int32_t idx, const char* name);

/*updateParameter, call to ask for parameter named name value and then call the callback to update the value.
 * Note that the registerParameterCallbacks function must be called before using this one.
 * param idx camera identifier.
 * param name, parameter name.
 */
VIDEOSERVERDLL_API int32_t updateParameter(int32_t idx, const char* name);

/*writeParameter, set parameter name to its double value.
 */
VIDEOSERVERDLL_API int32_t writeParameter(int32_t idx, const char* name, double value);

/*writeParameter, set parameter name to its long value.
 */
VIDEOSERVERDLL_API int32_t writelParameter(int32_t idx, const char* name, int64_t value);

/*writeParameter, set parameter name to its string value, use this for enumeration also.
 */
VIDEOSERVERDLL_API int32_t writesParameter(int32_t idx, const char* name, const char* value);

/*setZoomLevel, set continuous zoom level.
 * as a double value between 0(min zoom level) and 1(max zoom level)
 */
VIDEOSERVERDLL_API int32_t setZoomLevel(int32_t idx, double val);

/*getZoomLevel, get continuous zoom level.
 * as a double value between 0(min zoom level) and 1(max zoom level)
 */
VIDEOSERVERDLL_API int32_t getZoomLevel(int32_t idx, double* val);

/*setZoomLevel, set discrete zoom level.
 * as an int, using the zoom levels defined in VideoStreamer.ini
 */
VIDEOSERVERDLL_API int32_t setZoom(int32_t idx, int32_t val);

/*getZoomLevel, get zoom level.
 * as an int, using the zoom levels defined in VideoStreamer.ini
 */
VIDEOSERVERDLL_API int32_t getZoom(int32_t idx, int32_t* val);

/* getZoomRange, get discrete zoom level range.
 * a call to this function will call the zoomCallback defined in the registerParameterRangeCallbacks
 */
VIDEOSERVERDLL_API int32_t getZoomRange(int32_t idx);

/*getPredefinedZoomLevel, get predefined zoom levels.
 * a call to this function will call the predifinedzoomCallback defined in the registerParameterRangeCallbacks
 * it retrieves the continuous zoom levels that correspond to specific setup (full camera ROI, or no image processing)
 */
VIDEOSERVERDLL_API int32_t getPredefinedZoomLevel(int32_t idx);

//setExposure, set exposure.
VIDEOSERVERDLL_API int32_t setExposure(int32_t idx, double val);

//getExposure, get exposure.
VIDEOSERVERDLL_API int32_t getExposure(int32_t idx, double* val);

//getExposureRange, get exposure range.
VIDEOSERVERDLL_API int32_t getExposureRange(int32_t idx);

//setGain, set gain.
VIDEOSERVERDLL_API int32_t setGain(int32_t idx, double val);

//getGain, get gain.
VIDEOSERVERDLL_API int32_t getGain(int32_t idx, double* val);

//getGainRange, get gain range.
VIDEOSERVERDLL_API int32_t getGainRange(int32_t idx);

/*setGain, set WhiteBalance.
 * param chan, can be 'Red', 'Blue', or 'Green', to select the color to set.
 */
VIDEOSERVERDLL_API int32_t setWhiteBalance(int32_t idx, double val, const char* chan);

/*getGain, get WhiteBalance.
 * param chan, can be 'Red', 'Blue', or 'Green', to select the color to get.
 */
VIDEOSERVERDLL_API int32_t getWhiteBalance(int32_t idx, double* val, const char* chan);

/*getGainRange, get WhiteBalance range.
 * param chan, can be 'Red', 'Blue', or 'Green', to select the color to get.
 */
VIDEOSERVERDLL_API int32_t getWhiteBalanceRange(int32_t idx, const char* chan);

//updateCameraInfo, call to ask for camera Info, the callback will be called to update the value.
VIDEOSERVERDLL_API int32_t updateCameraInfo(int32_t idx);

//getCamerasNumber, get the cameras number in the pointer pNumber.
VIDEOSERVERDLL_API int32_t getCamerasNumber(int32_t* pNumber);

//updateCamerasNames, call to get all the available cameras names, this method will call the callback to update the value.
VIDEOSERVERDLL_API int32_t updateCamerasNames();

//startGrabbing, start grabbing on the specified camera. The camera must first be opened via openCamera (otherwise you cannot have a good idx value anyway)
VIDEOSERVERDLL_API int32_t startGrabbing(int32_t idx);

//stoGrabbing, stop grabbing on the camera. Note that the camera is not stopped, only the frame grabbed are not treated anymore.
//to fully close the camera use closeCamera.
VIDEOSERVERDLL_API int32_t stopGrabbing(int32_t idx);

//enableCompositeImages, Combine the images from both cameras or not.
VIDEOSERVERDLL_API int32_t enableCompositeImages(int32_t enable);

//autoAdjustLightParameters, when using the hybrid zoom system, adjust white balance and exposure
//parameters, so that all the cameras render the same image as the highest magnification one.
//the size parameter define the area on which the reference will be measured.
//we will use (2*size)^2 pixels on the lowest magnification camera to run the calibration.
VIDEOSERVERDLL_API int32_t autoAdjustLightParameters(int32_t size);

//getScale, get Image scale.
VIDEOSERVERDLL_API int32_t getScale(int32_t idx, double* val);

//getScale, get Image scale.
VIDEOSERVERDLL_API int32_t getZoomScale(int32_t idx, double zoomVal, double* val);

//getScale, get Image scale.
VIDEOSERVERDLL_API int32_t getZoomLevelScale(int32_t idx, size_t zoomVal, double* val);

//close, cleanly close all the cameras producers and so on, use it before closing.
VIDEOSERVERDLL_API void terminateVideoServer();



/***********
 * Configuration functions, that should not be used.
 * ************/
VIDEOSERVERDLL_API int32_t setScale(int32_t idx, double value);
VIDEOSERVERDLL_API int32_t getHeightMagnificationRatio(int32_t idx, double* value);
VIDEOSERVERDLL_API int32_t setHeightMagnificationRatio(int32_t idx, double value);
VIDEOSERVERDLL_API int32_t getWidthMagnificationRatio(int32_t idx, double* value);
VIDEOSERVERDLL_API int32_t setWidthMagnificationRatio(int32_t idx, double value);
VIDEOSERVERDLL_API int32_t getHeightOpticCenterOffset(int32_t idx, int32_t* value);
VIDEOSERVERDLL_API int32_t setHeightOpticCenterOffset(int32_t idx, int32_t value);
VIDEOSERVERDLL_API int32_t getWidthOpticCenterOffset(int32_t idx, int32_t* value);
VIDEOSERVERDLL_API int32_t setWidthOpticCenterOffset(int32_t idx, int32_t value);

VIDEOSERVERDLL_API int32_t getImageInfo(int32_t idx, ImageRaw* info);
VIDEOSERVERDLL_API int32_t registerCallback(int32_t idx, 
    void (__stdcall *callback)(ImageRaw*));
#ifdef __cplusplus
}
#endif
}
#endif /*__VIDEOSERVER_H__*/
