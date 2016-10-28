
/*******************************************************************************
 *
 *            Copyright(c) 2015 L-3 Communication Corporation,
 *                 Warrior Systems Division, All Rights Reserved
 *
 *                THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF
 *           L-3 COMMUNICATIONS CORPORATION, WARRIOR SYSTEMS DIVISION
 *
 *    The copyright notice above does not evidence any actual or intended
 *    publication of such source code. This source code may contain
 *    technical data that is subject to the control of the international
 *    traffic in arms regulations and/or the export administration
 *    regulations.
 *
 *    L-3 Communications Corporation, Warrior Systems Division is currently
 *    determining the proper export jurisdiction of all of our components.
 *    You should treat all attached information as being Technical Data
 *    controlled under the International Traffic in Arms Regulations (ITAR)
 *    Parts 120-130, unless indicated to the contrary.  U.S. Government
 *    authorization is required to transfer Technical Data to a non U.S.
 *    person.  If you wish to export this information, contact Warrior
 *    Systems at 866-509-2040 (Export Compliance) for proper jurisdiction.
 *
 *****************************************************************************/
/**
 *  @file      rpmsg.h
 *  @date      Nov 11, 2015
 *
 *             Name           Initials
 *  @author    withee_j        <initials>
 *
 ******************************************************************************
 *  @brief
 *  <brief description goes here>
 *
 ******************************************************************************/

#ifndef RPMSG_H_
#define RPMSG_H_


/*----------------------------------------------------------------------------*/
/* Includes                                                                   */
/*----------------------------------------------------------------------------*/
#include <stdint.h>

#if defined (__cplusplus)
extern "C" {
#endif


/*----------------------------------------------------------------------------*/
/* Public Defines and Macros                                                  */
/*----------------------------------------------------------------------------*/
#define POI_MAX_CALL_SIGN  (24)

/*----------------------------------------------------------------------------*/
/* Public Typedefs                                                            */
/*----------------------------------------------------------------------------*/
typedef struct
{
   uint32_t type;
   uint32_t repeat;
} IpcAhrsDebug_t;

typedef struct
{
   uint32_t id;
} IpcId_t;

typedef struct
{
   uint32_t id;
   float az;
   float range;
} IpcPoiDir_t;

typedef struct
{
   uint32_t    entries;   /*< number of icons to send back */
   uint32_t    total;     /*< Total number of icons to be shown */
   uint32_t    activeId;  /*< Id of the active icon in the FOV */
   float       heading;   /*< Our current heading */
   IpcPoiDir_t poi[0];    /*< List of our pois */
} IpcPolarPlot_t;

typedef struct
{
   uint32_t    len;          /*< number of icons to send back */
   uint32_t    totalLen;     /*< Total number of icons to be shown */
   uint32_t    frag;         /*< Fragment # of the current packet */
   uint8_t     data[0];      /*< data */
} IpcMsg_t;

typedef struct
{
  float scaleX[4];      // Third order scale factor over temp - X axis.
  float scaleY[4];      // Third order scale factor over temp - Y axis.
  float scaleZ[4];      // Third order scale factor over temp - Z axis.
  float biasX [4];      // Third order bias over temp - X axis.
  float biasY [4];      // Third order bias over temp - Y axis.
  float biasZ [4];      // Third order bias over temp - Z axis.
  float noXY  [3];      // 2nd order non-orthogonality - XY
  float noXZ  [3];      // 2nd order non-orthogonality - XZ
  float noYX  [3];      // 2nd order non-orthogonality - YX
  float noYZ  [3];      // 2nd order non-orthogonality - YZ
  float noZX  [3];      // 2nd order non-orthogonality - ZX
  float noZY  [3];      // 2nd order non-orthogonality - ZY
  float rotation[3][3]; // Alignment rotation of the unit.
} AhrsCalCoefSensor;

typedef enum GPS_FIX_QUAL
{
  GPS_FIX_QUAL_INVALID= 0,
  GPS_FIX_QUAL_GPS,
  GPS_FIX_QUAL_DGPS
} GpsFixQual;

#define GPS_TIME_STR_MAX         11

typedef struct
{
  char       time[GPS_TIME_STR_MAX]; // Zulu Time: HHMMSS.sss
  float      lat;                    // Current GPS Latitude
  float      lon;                    // Current GPS Longitude
  GpsFixQual fixQuality;             // GPS Fix Quality /* FIXME: do not send enumerations over interface - unknown */
  uint32_t   numSats;                // Number of Satellites
  float      HDOP;                   // Horizontal Dilution of Precision (HDOP)
  float      hmsl;                   // Height above mean sea level
  float      hae;                    // Current GPS height above ellipsoid (WGS84)
} GpsData;

struct AhrsOwnInfo
{
  float heading;        // Heading in radians (-pi to pi).
  float elevation;      // Elevation in radians (-pi/2 to pi/2).
  float lat;            // Latitude in WGS-84 degrees.
  float lon;            // Longitude in WGS-84 degrees.
  float hae;            // Height Above WGS-84 Ellipsoid in meters.
  float roll;           // Roll in radians (-pi/2 to pi/2).
};

typedef enum AHRS_IMU_TYPE
{
  AHRS_IMU_TYPE_ENVG = 0,
  AHRS_IMU_TYPE_IMU,
  AHRS_IMU_TYPE_EVE,
  AHRS_IMU_TYPE_CRATOS,
} AhrsImuType;

typedef enum AHRS_SENSOR_ID
{
  AHRS_SENSOR_ID_START   = 0,
  AHRS_SENSOR_ID_GYRO    = AHRS_SENSOR_ID_START,
  AHRS_SENSOR_ID_ACCEL   = 1,
  AHRS_SENSOR_ID_MAG_INT = 2,
  AHRS_SENSOR_ID_MAG_EXT = 3,
  AHRS_SENSOR_ID_MAX
} AhrsSensorId;

typedef struct
{
  uint32_t cols;
  uint32_t rows;
  uint32_t centerColPixel; // X.
  uint32_t centerRowPixel; // Y.
  float    ifov;           // IFOV (radians/pixel).
} AhrsCamera;

typedef struct
{
   uint32_t id;
   uint32_t type;
   float    lat;
   float    lon;
   float    hae;
   uint8_t  callSign[ POI_MAX_CALL_SIGN ];
} IpcAhrsPoiAdd_t;

typedef struct
{
  float    lat;       // Latitude in WGS-84 degrees.
  float    lon;       // Longitude in WGS-84 degrees.
  float    hae;       // Height Above WGS-84 Ellipsoid in meters.
} IpcAhrsLoc_t;

typedef struct
{
   uint32_t          type;
   uint32_t          sensor;
   AhrsCalCoefSensor cal;
} IpcImuCal_t;

typedef struct
{
   uint32_t          type;
   AhrsCamera        cfg;
} IpcCameraCfg_t;

/*!
 * IPC Message Identifiers
 */
typedef enum
{
   eIpcMsg_AddEntry = 0,
   eIpcMsg_RemoveEntry,
   eIpcMsg_RemoveAll,
   eIpcMsg_OwnLocation,
   eIpcMsg_RpLocation,
   eIpcMsg_Rp,
   eIpcMsg_Pin,
   eIpcMsg_Compass,
   eIpcMsg_PolarPlot,
   eIpcMsg_CameraCfg,
   eIpcMsg_DisplayCfg,
   eIpcMsg_ImuCal1,
   eIpcMsg_ImuCal2,
   eIpcMsg_DisplayFilter,
   eIpcMsg_DisplayRadarFilter,
   eIpcMsg_DetailText,
   eIpcMsg_MagCal,
   eIpcMsg_Config,
   eIpcMsg_GetOwnLoc,
   eIpcMsg_GetAhrsStatus,
   eIpcMsg_FrameSynch,
   eIpcMsg_SetPoiTrans,
   eIpcMsg_FirmwareVersion,
   eIpcMsg_PosSourceEnable,
   eIpcMsg_RpDetailText,
   eIpcMsg_DebugData,
   eIpcMsg_Range,
   eIpcMsg_RadaPlotStype,
   eIpcMsg_PinUpdateRequest,
   eIpcMsg_PinSetActive,
   eIpcMsg_StartService,
   eIpcMsg_PoiTextEnable,
   eIpcMsg_PreRp,
   eIpcMsg_IckfUpdate,
   eIpcMsg_SetLandmarkConf,
   eIpcMsg_SetLandmarkPercent,
   eIpcMsg_CalGyro,
   eIpcMsg_CalAccel,
   eIpcMsg_CalMagsInt,
   eIpcMsg_CalMagsExt,
   eIpcMsg_GetCalGyro,
   eIpcMsg_GetCalAccel,
   eIpcMsg_GetCalMagsInt,
   eIpcMsg_GetCalMagsExt,
   eIpcMsg_StaticLocation,
   eIpcMsg_SetMagDec,
   eIpcMsg_SetImuCal,
   eIpcMsg_SetReticleMode,
   eIpcMsg_SetGpsLoc,
   eIpcMsg_HeadingRequest,

   eIpcMsg_MaxEnum,
} IpcMsg_e;

/*!
 * IPC POI Types
 */
typedef enum
{
   ePoiType_Friend = 0,
   ePoiType_Hostile,
   ePoiType_Neutral,
   ePoiType_Unknown,
   ePoiType_Pin,
   ePoiType_Waypoint,
   ePoiType_PinRange,
   ePoiType_Rta = 16,
   ePoiType_Ar,
   ePoiType_MaxEnum
} PoiType_t;

/*!
 * IPC Retcile Modes
 */
typedef enum
{
   eIpcRetcile_Off,
   eIpcReticle_On,
   eIpcReticle_Auto,

   eIpcReticle_MaxEnum,
} IpcRetcile_e;
/*----------------------------------------------------------------------------*/
/* Public (global extern) Variables                                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Function Declarations                                               */
/*----------------------------------------------------------------------------*/

int Rpmsg_OpenDevice   ( const char* dev );
int Rpmsg_CloseDevice  ( int fd );
int Rpmsg_RecvMsg      ( int fd, void * data, size_t len );
int Rpmsg_SendMsg      ( int fd, IpcMsg_e command, void * data, size_t len );
int Rpmsg_SendMsgNoAck ( int fd, IpcMsg_e command, void * data, size_t len );

#if defined (__cplusplus)
}
#endif

#endif /* RPMSG_H_ */
