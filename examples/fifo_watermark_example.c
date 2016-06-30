/*						watermark test demo							*/
/*																	*/
/*	This demo tests the watermark feature of the sensor. It is  	*/
/*  Useful if your reading buffer is smaller than the fifo you are  */
/*  Using, but you do not wish to go through the process of emptying*/
/*  the fifo in multiple reads. You can then set a watermark smaller*/
/*  Than your buffer size, and the BHI will trigger an interrupt    */
/*  even it the latency is not reached yet							*/
/*	Terminal configuration : 115200, 8N1							*/

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

#include "asf.h"
#include "conf_board.h"
#include "led.h"

#include "bhy_uc_driver.h"
#include "BHIfw.h"

#include "string.h"
#include <math.h>

/** TWI Bus Clock 400kHz */
#define TWI_CLK     400000

#define ARRAYSIZE 250	//should be greater or equal to 69 bytes, page size (50) + maximum packet size(18) + 1
#define WATERMARK 200

#define EDBG_FLEXCOM		FLEXCOM7
#define EDBG_USART			USART7
#define EDBG_FLEXCOM_IRQ	FLEXCOM7_IRQn

const sam_usart_opt_t usart_console_settings = {
	115200,
	US_MR_CHRL_8_BIT,
	US_MR_PAR_NO,
	US_MR_NBSTOP_1_BIT,
	US_MR_CHMODE_NORMAL
};


void trigger_logic_analyzer(void);
void pre_initialize_i2c(void);
void twi_initialize(void);
void edbg_usart_enable(void);
void mdelay(u32 ul_dly_ticks);
void udelay(u32 ul_dly_ticks);
void device_specific_initialization(void);
void print_read_size(u16 bytes_read);

char outbuffer[60] = "";

/* active delay of ~1us */
void udelay(u32 ul_dly_ticks)
{
	volatile uint32_t dummy;
	
	for (uint32_t u=0; u<ul_dly_ticks; u++) {
		for (dummy=0; dummy < 0x06; dummy++) {
			if (dummy == 0xff) {
				dummy = 0x100;
			}		
		}
	}
}

/* active delay of ~1ms */
void mdelay(u32 ul_dly_ticks)
{	
	volatile uint32_t dummy;
	
	for (uint32_t u=0; u<ul_dly_ticks; u++) {
		for (dummy=0; dummy < 0x1BA0; dummy++) {
			if (dummy == 0xff) {
				dummy = 0x100;
			}		
		}
	}
}

/* this routine issues 9 clock cycles on the SCL line
   so that all devices release the SDA line if they are 
   holding it */
void pre_initialize_i2c(void) {
	ioport_set_pin_dir(EXT1_PIN_I2C_SCL, IOPORT_DIR_OUTPUT);
	for (int i=0; i<9;++i) {
		ioport_set_pin_level(EXT1_PIN_I2C_SCL, IOPORT_PIN_LEVEL_LOW);
		udelay(2);
		ioport_set_pin_level(EXT1_PIN_I2C_SCL, IOPORT_PIN_LEVEL_HIGH);
		udelay(1);
	}
}

void twi_initialize(void) {
	twi_options_t opt;
	opt.master_clk = sysclk_get_cpu_hz();
	opt.speed = TWI_CLK;
	/* Enable the peripheral and set TWI mode. */
	flexcom_enable(BOARD_FLEXCOM_TWI);
	flexcom_set_opmode(BOARD_FLEXCOM_TWI, FLEXCOM_TWI);

	
	if (twi_master_init(TWI4, &opt) != TWI_SUCCESS) 
		
		while (1) {
			/* Capture error */
		}
}


/* EDBG USART RX IRQ Handler */
/* just echo characters */
void FLEXCOM7_Handler ( void ) {
	uint32_t u32_char;
	while (usart_is_rx_ready(EDBG_USART)) {
		usart_getchar(EDBG_USART,&u32_char);
		usart_putchar(EDBG_USART,u32_char);
	}
}


void edbg_usart_enable(void) {
	
	flexcom_enable(EDBG_FLEXCOM);
	flexcom_set_opmode(EDBG_FLEXCOM, FLEXCOM_USART);
	
	usart_init_rs232(EDBG_USART, &usart_console_settings, sysclk_get_main_hz());
	usart_enable_tx(EDBG_USART);
	usart_enable_rx(EDBG_USART);
	
	usart_enable_interrupt(EDBG_USART, US_IER_RXRDY);
	NVIC_EnableIRQ(EDBG_FLEXCOM_IRQ);

}         

/* This function regroups all the initialization specific to SAM G55 */
void device_specific_initialization(void) {
	/* Initialize the SAM system */
	sysclk_init();

	/* execute this function before board_init */
	pre_initialize_i2c();
	
	/* Initialize the board */
	board_init();

	/* configure the i2c */
	twi_initialize();
	
	/* configures the serial port */
	edbg_usart_enable();

	/* configures the interrupt pin GPIO */
	ioport_set_pin_dir(EXT1_PIN_GPIO_1, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(EXT1_PIN_GPIO_1, IOPORT_MODE_PULLUP);
}

void print_read_size(u16 bytes_read) {
	static u16 min_value=0xFFFF, max_value=0;
	
	min_value = Min(min_value, bytes_read);
	max_value = Max(max_value, bytes_read);
	
	sprintf(outbuffer, " Min:%04d  Current:%04d  Max:%04d\r", min_value, bytes_read, max_value);
	
	usart_write_line(EDBG_USART, outbuffer);
}

int main(void)
{
	u8 array[ARRAYSIZE];
	u16 bytes_remaining, bytes_read;
	
	/* Initialize the SAM G55 system */
	device_specific_initialization();
	
	sprintf(outbuffer, "\n\rARRAYSIZE=%d, WATERMARK=%d\n\r", ARRAYSIZE, WATERMARK);
	usart_write_line(EDBG_USART, outbuffer);

	/* initializes the BHI160 and loads the RAM patch if */
	/* the ram patch does not output any debug			 */
	/* information to the fifo, this demo will not work	 */
	bhy_driver_init(_bhi_fw, _bhi_fw_len);
		
	//wait for the interrupt pin to go down then up again
	while (ioport_get_pin_level(EXT1_PIN_GPIO_1));
	while (!ioport_get_pin_level(EXT1_PIN_GPIO_1));
	
	/* empty the fifo for a first time. the interrupt does not contain */
	/* sensor data													   */
	bhy_read_fifo(array, ARRAYSIZE, &bytes_read, &bytes_remaining);
	
	/* Enables a streaming sensor. The goal here is to generate data */
	/* in the fifo, not to read the sensor values.								*/
	bhy_enable_virtual_sensor(VS_TYPE_ROTATION_VECTOR, VS_NON_WAKEUP, 100, 1000, VS_FLUSH_NONE, 0, 0);
	
	/* Sets the watermark level */
	bhy_set_fifo_water_mark(BHY_FIFO_WATER_MARK_NON_WAKEUP,	WATERMARK);
	
	/* continuously read and parse the fifo */
	while (true) {	
		/* wait until the interrupt fires */
		
		while (!ioport_get_pin_level(EXT1_PIN_GPIO_1) ) ;
		
		bhy_read_fifo(array, ARRAYSIZE, &bytes_read, &bytes_remaining);
		
		print_read_size(bytes_read);
		
		if (bytes_remaining) {
			/* The watermark level was set too high, it didn't manage to read */
			
			usart_write_line(EDBG_USART, "\n\rThe watermark level was too low, demo stopped.");
			
			/* stops the demo here */
			while(1);
		}
	}
}