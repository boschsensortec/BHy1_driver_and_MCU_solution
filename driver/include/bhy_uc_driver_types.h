/*
 * bhy_uc_driver_types.h
 *
 * Created: 8/12/2015 13:33:40
 *  Author: Marc-Andre Harvey
 */ 

 /*  Disclaimer
  *
  * Common: Bosch Sensortec products are developed for the consumer goods
  * industry. They may only be used within the parameters of the respective valid
  * product data sheet.  Bosch Sensortec products are provided with the express
  * understanding that there is no warranty of fitness for a particular purpose.
  * They are not fit for use in life-sustaining, safety or security sensitive
  * systems or any system or device that may lead to bodily harm or property
  * damage if the system or device malfunctions. In addition, Bosch Sensortec
  * products are not fit for use in products which interact with motor vehicle
  * systems.  The resale and/or use of products are at the purchaser's own risk
  * and his own responsibility. The examination of fitness for the intended use
  * is the sole responsibility of the Purchaser.
  *
  * The purchaser shall indemnify Bosch Sensortec from all third party claims,
  * including any claims for incidental, or consequential damages, arising from
  * any product use not covered by the parameters of the respective valid product
  * data sheet or not approved by Bosch Sensortec and reimburse Bosch Sensortec
  * for all costs in connection with such claims.
  *
  * The purchaser must monitor the market for the purchased products,
  * particularly with regard to product safety and inform Bosch Sensortec without
  * delay of all security relevant incidents.
  *
  * Engineering Samples are marked with an asterisk (*) or (e). Samples may vary
  * from the valid technical specifications of the product series. They are
  * therefore not intended or fit for resale to third parties or for use in end
  * products. Their sole purpose is internal client testing. The testing of an
  * engineering sample may in no way replace the testing of a product series.
  * Bosch Sensortec assumes no liability for the use of engineering samples. By
  * accepting the engineering samples, the Purchaser agrees to indemnify Bosch
  * Sensortec from all claims arising from the use of engineering samples.
  *
  * Special: This software module (hereinafter called "Software") and any
  * information on application-sheets (hereinafter called "Information") is
  * provided free of charge for the sole purpose to support your application
  * work. The Software and Information is subject to the following terms and
  * conditions:
  *
  * The Software is specifically designed for the exclusive use for Bosch
  * Sensortec products by personnel who have special experience and training. Do
  * not use this Software if you do not have the proper experience or training.
  *
  * This Software package is provided `` as is `` and without any expressed or
  * implied warranties, including without limitation, the implied warranties of
  * merchantability and fitness for a particular purpose.
  *
  * Bosch Sensortec and their representatives and agents deny any liability for
  * the functional impairment of this Software in terms of fitness, performance
  * and safety. Bosch Sensortec and their representatives and agents shall not be
  * liable for any direct or indirect damages or injury, except as otherwise
  * stipulated in mandatory applicable law.
  *
  * The Information provided is believed to be accurate and reliable. Bosch
  * Sensortec assumes no responsibility for the consequences of use of such
  * Information nor for any infringement of patents or other rights of third
  * parties which may result from its use. No license is granted by implication
  * or otherwise under any patent or patent rights of Bosch. Specifications
  * mentioned in the Information are subject to change without notice.
  *
  */


#ifndef BHY_UC_DRIVER_TYPES_H_
#define BHY_UC_DRIVER_TYPES_H_

#include "bhy_uc_driver_constants.h"

/****************************************************************************/
/*						Data types definitions								*/
/****************************************************************************/

/* follows section 9.4 table 14 of the BHI160 datasheet */
typedef enum {
	VS_TYPE_ACCELEROMETER				= VS_ID_ACCELEROMETER,
	VS_TYPE_GEOMAGNETIC_FIELD			= VS_ID_MAGNETOMETER,
	VS_TYPE_ORIENTATION					= VS_ID_ORIENTATION,
	VS_TYPE_GYROSCOPE					= VS_ID_GYROSCOPE,
	VS_TYPE_LIGHT						= VS_ID_LIGHT,
	VS_TYPE_PRESSURE					= VS_ID_BAROMETER,
	VS_TYPE_TEMPERATURE					= VS_ID_TEMPERATURE,
	VS_TYPE_PROXIMITY					= VS_ID_PROXIMITY,
	VS_TYPE_GRAVITY						= VS_ID_GRAVITY,
	VS_TYPE_LINEAR_ACCELERATION			= VS_ID_LINEAR_ACCELERATION,
	VS_TYPE_ROTATION_VECTOR				= VS_ID_ROTATION_VECTOR,
	VS_TYPE_RELATIVE_HUMIDITY			= VS_ID_HUMIDITY,
	VS_TYPE_AMBIENT_TEMPERATURE			= VS_ID_AMBIENT_TEMPERATURE,
	VS_TYPE_MAGNETIC_FIELD_UNCALIBRATED = VS_ID_UNCALIBRATED_MAGNETOMETER,
	VS_TYPE_GAME_ROTATION_VECTOR		= VS_ID_GAME_ROTATION_VECTOR,
	VS_TYPE_GYROSCOPE_UNCALIBRATED		= VS_ID_UNCALIBRATED_GYROSCOPE,
	VS_TYPE_SIGNIFICANT_MOTION			= VS_ID_SIGNIFICANT_MOTION,
	VS_TYPE_STEP_DETECTOR				= VS_ID_STEP_DETECTOR,
	VS_TYPE_STEP_COUNTER				= VS_ID_STEP_COUNTER,
	VS_TYPE_GEOMAGNETIC_ROTATION_VECTOR	= VS_ID_GEOMAGNETIC_ROTATION_VECTOR,
	VS_TYPE_HEART_RATE					= VS_ID_HEART_RATE,
	VS_TYPE_TILT						= VS_ID_TILT_DETECTOR,
	VS_TYPE_WAKEUP						= VS_ID_WAKE_GESTURE,
	VS_TYPE_GLANCE						= VS_ID_GLANCE_GESTURE,
	VS_TYPE_PICKUP						= VS_ID_PICKUP_GESTURE,
	VS_TYPE_ACTIVITY_RECOGNITION		= VS_ID_ACTIVITY
} bhy_virtual_sensor_t;

typedef enum {
	BHY_META_EVENT_TYPE_NOT_USED 				= 0,
	BHY_META_EVENT_TYPE_FLUSH_COMPLETE			= 1,
	BHY_META_EVENT_TYPE_SAMPLE_RATE_CHANGED		= 2,
	BHY_META_EVENT_TYPE_POWER_MODE_CHANGED 		= 3,
	BHY_META_EVENT_TYPE_ERROR 					= 4,
	/* IDs 5-10 are reserved */
	BHY_META_EVENT_TYPE_SENSOR_ERROR 			= 11,
	BHY_META_EVENT_TYPE_FIFO_OVERFLOW 			= 12,
	BHY_META_EVENT_TYPE_DYNAMIC_RANGE_CHANGED	= 13,
	BHY_META_EVENT_TYPE_FIFO_WATERMARK 			= 14,
	BHY_META_EVENT_TYPE_SELF_TEST_RESULTS 		= 15,
	BHY_META_EVENT_TYPE_INITIALIZED 			= 16,

} bhy_meta_event_type_t;


/* definition of structures of all the data types */
typedef struct {
	u8 sensor_id;
} bhy_data_padding_t;

typedef struct {
	u8  sensor_id;
	s16 x;
	s16 y;
	s16 z;
	s16 w;
	s16 estimated_accuracy;
} bhy_data_quaternion_t;

typedef struct {
	u8  sensor_id;
	s16 x;
	s16 y;
	s16 z;
	u8  status;
} bhy_data_vector_t;

typedef struct {
	u8 sensor_id;
	u8 data;
} bhy_data_scalar_u8_t;

typedef struct {
	u8  sensor_id;
	u16 data;
} bhy_data_scalar_u16_t;

typedef struct {
	u8  sensor_id;
	s16 data;
} bhy_data_scalar_s16_t;

typedef struct {
	u8  sensor_id;
	u32 data;
} bhy_data_scalar_u24_t;

typedef struct {
	u8 sensor_id;
} bhy_data_sensor_event_t;

typedef struct {
	u8  sensor_id;
	s16 x;
	s16 y;
	s16 z;
	s16 x_bias;
	s16 y_bias;
	s16 z_bias;
	u8  status;
} bhy_data_uncalib_vector_t;

typedef struct {
	u8 meta_event_id;
	bhy_meta_event_type_t event_number;
	u8 sensor_type;
	u8 event_specific;
} bhy_data_meta_event_t;

typedef struct {
	u8 sensor_id;
	s32 x;
	s32 y;
	s32 z;
	u32 timestamp;
} bhy_data_bsx_t;

#if BHY_DEBUG
typedef struct {
	u8 sensor_id;
	u8 data[13];
} bhy_data_debug_t;
#endif

/* definition of a generic structure that can contain any data type	it		*/
/* occupies in RAM the size of the largest data structure, which is 18 bytes*/
/* as of 08/04/2015															*/
typedef union {
	bhy_data_padding_t 			data_padding;
	bhy_data_quaternion_t 		data_quaternion;
	bhy_data_vector_t			data_vector;
	bhy_data_scalar_u8_t		data_scalar_u8;
	bhy_data_scalar_u16_t		data_scalar_u16;
	bhy_data_scalar_s16_t		data_scalar_s16;
	bhy_data_scalar_u24_t		data_scalar_u24;
	bhy_data_sensor_event_t		data_sensor_event;
	bhy_data_uncalib_vector_t	data_uncalib_vector;
	bhy_data_meta_event_t		data_meta_event;
	bhy_data_bsx_t				data_bsx;
	#if BHY_DEBUG
	bhy_data_debug_t			data_debug;
	#endif
} bhy_data_generic_t;

/* follows section 15 of the BHI160 datasheet the order of this enumeration */
/* is important, do not change it											*/
typedef enum {
	BHY_DATA_TYPE_PADDING				= 0,
	BHY_DATA_TYPE_QUATERNION			= 1,
	BHY_DATA_TYPE_VECTOR				= 2,
	BHY_DATA_TYPE_SCALAR_U8				= 3,
	BHY_DATA_TYPE_SCALAR_U16			= 4,
	BHY_DATA_TYPE_SCALAR_S16			= 5,
	BHY_DATA_TYPE_SCALAR_U24			= 6,
	BHY_DATA_TYPE_SENSOR_EVENT			= 7,
	BHY_DATA_TYPE_UNCALIB_VECTOR		= 8,
	BHY_DATA_TYPE_META_EVENT			= 9,
	BHY_DATA_TYPE_BSX					= 10
	#if BHY_DEBUG
	,	BHY_DATA_TYPE_DEBUG					= 11
	#endif
} bhy_data_type_t;


#define VS_NON_WAKEUP	0
#define VS_WAKEUP		32

#define VS_FLUSH_NONE	0x00
#define VS_FLUSH_ALL	0xFF
#define VS_FLUSH_SINGLE 0x01



#endif /* BHY_UC_DRIVER_TYPES_H_ */