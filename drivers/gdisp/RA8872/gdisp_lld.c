/*
    ChibiOS/RT - Copyright (C) 2012
                 Joel Bodenmann aka Tectu <joel@unormal.org>

    This file is part of ChibiOS/GFX.

    ChibiOS/GFX is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/GFX is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    gdispRA8872/gdisp_lld.c
 * @brief   GDISP Graphics Driver subsystem low level driver source for the RA8872 display.
 *
 * @addtogroup GDISP
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "gdisp.h"

#if HAL_USE_GDISP || defined(__DOXYGEN__)

/* Include the emulation code for things we don't support */
#include "gdisp_emulation.c"

#include "RA8872_lld.c.h"

#define SEQ_WRT_REG (1 << 11)
#define SEQ_GPIO (1 << 10)
#define SEQ_SPI_CMD (1 << 8)
#define SEQ_DLY (1 << 9)
#define GPIO(x) ((x & 0x00FF) | SEQ_GPIO)
#define SPI_CMD(x) ((x & 0x00FF) | SEQ_SPI_CMD)
#define SPI_DAT(x) (x & 0x00FF)
#define DAT(x) (x & 0x00FF)
#define DLY(x) ((x & 0x00FF) | SEQ_DLY)
#define WRT_REG(x, y) ((x & 0x00FF) | SEQ_WRT_REG), (y & 0x00FF)

 
 uint16_t ili9340_cfg[] = {
 #if 1
 	WRT_REG(SYSR, 0x08),
 	WRT_REG(0x04, 0x00),
 	WRT_REG(0x21, 0x10),
 	DLY(100),
 	WRT_REG(0x11, 0x00),
 	WRT_REG(0x12, 0x00),
 	WRT_REG(0x13, 0x00),
 	/* horizontal set */
 	WRT_REG(0x14, 0x27),
 	WRT_REG(0x15, 0x04),
 	WRT_REG(0x16, 0x00),
 	WRT_REG(0x17, 0x04),
 	WRT_REG(0x18, 0x00),
 	WRT_REG(0x19, 0xEF),
 	WRT_REG(0x1A, 0x00),
 	WRT_REG(0x1B, 0x14),
 	WRT_REG(0x1C, 0x00),
 	WRT_REG(0x1D, 0x0A),
 	WRT_REG(0x1E, 0x00),
 	WRT_REG(0x1F, 0x00),
 	WRT_REG(0x88, 0x09),
 	WRT_REG(0x8A, 0x81),
 	WRT_REG(0x8B, 0xFF),
 	WRT_REG(0x8C, 0x81),
 	WRT_REG(0x8D, 0xF4),
#endif
 	/* reset LCD driver */
 	GPIO(0x0F),	/* 1 1 1 1 */
 	GPIO(0x07), /* 0 1 1 1 */
 	DLY(200),
 	GPIO(0x0F), /* 1 1 1 1 */
 	DLY(100),
 	/* start initial sequence */
 	SPI_CMD(0xEF), SPI_DAT(0x03), SPI_DAT(0x80), SPI_DAT(0x02),
 	SPI_CMD(0xB1), SPI_DAT(0x00), SPI_DAT(0x1A),
 	/* --- */
 	SPI_CMD(0xCF), SPI_DAT(0x00), SPI_DAT(0xAA), SPI_DAT(0xB0),
 	SPI_CMD(0xED), SPI_DAT(0x67), SPI_DAT(0x03), SPI_DAT(0x12), SPI_DAT(0x81),
 	SPI_CMD(0xE8), SPI_DAT(0x85), SPI_DAT(0x01), SPI_DAT(0x78),
 	SPI_CMD(0xCB), SPI_DAT(0x39), SPI_DAT(0x2C), SPI_DAT(0x00), SPI_DAT(0x34), SPI_DAT(0x02),
 	SPI_CMD(0xF7), SPI_DAT(0x20),
 	SPI_CMD(0xEA), SPI_DAT(0x00), SPI_DAT(0x00),
 	/* power control 1 */
 	SPI_CMD(0xC0), SPI_DAT(0x25),
 	/* power control 1 */
 	SPI_CMD(0xC1), SPI_DAT(0x11),
 	/* Vcomh & coml control */
 	SPI_CMD(0xC5), SPI_DAT(0x5C), SPI_DAT(0x4C),
 	/* vcom adjust control */
 	SPI_CMD(0xC7), SPI_DAT(0x80),
 	/* Memory Access Control  */
	SPI_CMD(0x36), SPI_DAT(0x48),
	SPI_CMD(0xF2), SPI_DAT(0x00),
	/* column select */
	SPI_CMD(0x2A), SPI_DAT(0x00), SPI_DAT(0x00), SPI_DAT(0x00), SPI_DAT(0xEF),
	/* page select */
	SPI_CMD(0x2B), SPI_DAT(0x00), SPI_DAT(0x00), SPI_DAT(0x01), SPI_DAT(0x3F),
	/* Gamma select G2.2 */
	SPI_CMD(0x26), SPI_DAT(0x01),
	/* Positive  gamma */
	SPI_CMD(0xE0), SPI_DAT(0x0F), SPI_DAT(0x27), SPI_DAT(0x23), SPI_DAT(0x0b), SPI_DAT(0x0f), 
			   SPI_DAT(0x05), SPI_DAT(0x54), SPI_DAT(0x74), SPI_DAT(0x45), SPI_DAT(0x0a), 
			   SPI_DAT(0x17), SPI_DAT(0x0a), SPI_DAT(0x1c), SPI_DAT(0x0e), SPI_DAT(0x08),
	/* negative  gamma */
	SPI_CMD(0xE1), SPI_DAT(0x08), SPI_DAT(0x1a), SPI_DAT(0x1e), SPI_DAT(0x03), SPI_DAT(0x0f),
			   SPI_DAT(0x05), SPI_DAT(0x2e), SPI_DAT(0x25), SPI_DAT(0x3b), SPI_DAT(0x01), 
			   SPI_DAT(0x06), SPI_DAT(0x05), SPI_DAT(0x25), SPI_DAT(0x33), SPI_DAT(0x0F),
	/* RGB & CPU 18 0X66 /16 0X55 bit select */
	SPI_CMD(0x3A), SPI_DAT(0x66),
	/* INTERFACE control */
	SPI_CMD(0xf6), SPI_DAT(0x01), SPI_DAT(0x01), SPI_DAT(0x06),
	SPI_CMD(0xB0), SPI_DAT(0xc0), SPI_DAT(0x02), SPI_DAT(0x02), SPI_DAT(0x0a), SPI_DAT(0x14),
	/* display function control */
	SPI_CMD(0xB6), SPI_DAT(0x0A), SPI_DAT(0xA2),
	SPI_CMD(0x11),
	DLY(120),
	/* display on */
	SPI_CMD(0x29)
 };

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/* ---- Required Routines ---- */
/*
	The following 2 routines are required.
	All other routines are optional.
*/


inline void spi9341_8bit(uint8_t data) {
	uint8_t i, mask=0x80;
	for(i=0; i < 8; i++) {
		if(mask & data) {
			lld_lcdWriteReg(0x13, 0x2B); /* 1 0 1 1 */
			lld_lcdWriteReg(0x13, 0x2A); /* 1 0 1 0 */
			lld_lcdWriteReg(0x13, 0x2B); /* 1 0 1 1 */
		} else {
			lld_lcdWriteReg(0x13, 0x29); /* 1 0 0 1 */
			lld_lcdWriteReg(0x13, 0x28); /* 1 0 0 0 */
			lld_lcdWriteReg(0x13, 0x29); /* 1 0 0 1 */
		}
	}
}

inline void spi9341_cmd(uint8_t cmd) {
	lld_lcdWriteReg(0x13, 0x09); /* 1 0 0 1 */
	lld_lcdWriteReg(0x13, 0x08); /* 1 0 0 0 */
	lld_lcdWriteReg(0x13, 0x09); /* 1 0 0 1 */
	spi9341_8bit(cmd);
}

inline void spi9341_data(uint8_t data) {
	lld_lcdWriteReg(0x13, 0x0B); /* 1 0 1 1 */
	lld_lcdWriteReg(0x13, 0x0A); /* 1 0 1 0 */
	lld_lcdWriteReg(0x13, 0x0B); /* 1 0 1 1 */
	spi9341_8bit(data);
}


/**
 * @brief   Low level GDISP driver initialization.
 *
 * @notapi
 */
bool_t GDISP_LLD(init)(void) {
	chThdSleepMilliseconds(1000);
	/* ILI9340 init */
	uint16_t i;
	for(i = 0; i < sizeof(ili9340_cfg); i++) {
		if(ili9340_cfg[i] & SEQ_SPI_CMD)
			spi9341_cmd(DAT(ili9340_cfg[i]));
		else if(ili9340_cfg[i] & SEQ_DLY)
			chThdSleepMilliseconds(DAT(ili9340_cfg[i]));
		else if(ili9340_cfg[i] & SEQ_GPIO)
			lld_lcdWriteReg(0x13, DAT(ili9340_cfg[i]));
		else if(ili9340_cfg[i] & SEQ_WRT_REG)
			lld_lcdWriteReg(DAT(ili9340_cfg[i]), DAT(ili9340_cfg[++i]));
		else
			spi9341_data(DAT(ili9340_cfg[i]));
	}
#if 0
	palSetPadMode(GDISP_RST_GPIO, GDISP_RST_PIN, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
	// A Good idea to reset the module before using
	GDISP_RST_LOW;
	chThdSleepMilliseconds(200);
	GDISP_RST_HIGH;         // Hardware Reset
	chThdSleepMilliseconds(700);
	chThdSleepMilliseconds(2000);
#endif
	#ifdef GDISP_USE_GPIO
		#error "GPIO not yet implemented for this device"
	#elif defined(GDISP_USE_FSMC)
		#if defined(STM32F1XX)
			/* FSMC setup. TODO: this only works for STM32F1 */
			rccEnableAHB(RCC_AHBENR_FSMCEN, 0);

			/* TODO: pin setup */
		#elif defined(STM32F4XX)
			/* STM32F4 FSMC init */
			rccEnableAHB3(RCC_AHB3ENR_FSMCEN, 0);

			/* set pins to FSMC mode */
			IOBus busD = {GPIOD, (1 << 0) | (1 << 1) | (1 << 4) | (1 << 5) | 
					 (1 << 7) | (1 << 11) | (1 << 14) | (1 << 15), 0};

			IOBus busE = {GPIOE, (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10), 0};

			palSetBusMode(&busD, PAL_MODE_ALTERNATE(12));
			palSetBusMode(&busE, PAL_MODE_ALTERNATE(12));
		#else
			#error "FSMC not implemented for this device"
		#endif

		int FSMC_Bank = 0;
		/* FSMC timing */
		FSMC_Bank1->BTCR[FSMC_Bank+1] = (14) | (14 << 8) | (14 << 16);

		/* Bank1 NOR/SRAM control register configuration */
		FSMC_Bank1->BTCR[FSMC_Bank] =   FSMC_BCR1_WREN | FSMC_BCR1_MBKEN;
	#endif
	/*uint8_t r = 0;
	for(r = 0; r < 128; r++) {
		volatile uint8_t reg = lld_lcdReadReg(r);
	}*/
#if 1
    /* PLL pre-driver divided by 1, PLLDIVN = 7 */
    lld_lcdWriteReg(PLLC1, PLLDIVM_BY_1 | 0x07);
    /* PLLDIVK = 3 -> PLL output divided by 8*/
    lld_lcdWriteReg(PLLC2, PLLC2_DIV_BY_8);
    chThdSleepMilliseconds(5);

    /* perform software reset */
    lld_lcdWriteReg(PWRR, PWRR_RESET_ON);
    chThdSleepMilliseconds(10);
    lld_lcdWriteReg(PWRR, PWRR_RESET_OFF);
	chThdSleepMilliseconds(100);

    /* GPIOs as output */
    lld_lcdWriteReg(IOCR, 0x00);

    /* GPIOs off */
    lld_lcdWriteReg(IODR, 0x00);

    /* Horizontal display width(pixels) = (HDWR + 1)*8 => width = 320 pixels */
    lld_lcdWriteReg(HDWR, 39);
    /* Horizontal Non-Display Period fine tune */
    lld_lcdWriteReg(HNDFTR, HNDFTR_DE_ACT_HIGH | 0x02);
    /* Horizontal Non-Display Period */
    lld_lcdWriteReg(HNDR, 0x03);
    /* HSYNC Start Position Register */
    lld_lcdWriteReg(HSTR, 0x01);
    /* HSYNC Pulse Width Register */
    lld_lcdWriteReg(HPWR, 0x03);

    /* vertical set */
    lld_lcdWriteReg(0x19, 0xEF);
    lld_lcdWriteReg(0x1A, 0x00);
    lld_lcdWriteReg(0x1B, 0x0F);
    lld_lcdWriteReg(0x1C, 0x00);
    lld_lcdWriteReg(0x1D, 0x0E);
    lld_lcdWriteReg(0x1E, 0x06);
    lld_lcdWriteReg(0x1F, 0x01);
    lld_lcdWriteReg(0x28, 0x02);
#endif

	/*SYSR bit[4:3] = 00 256 color , bit[2:1]= 00 8bit MCU */
    //lld_lcdWriteReg(SYSR, 0x30 | SYSR_PARALLEL_DATA | SYSR_8BPP);

	/* Now initialise the GDISP structure */
	GDISP.Width = GDISP_SCREEN_WIDTH;
	GDISP.Height = GDISP_SCREEN_HEIGHT;
	GDISP.Orientation = GDISP_ROTATE_0;
	GDISP.Powermode = powerOn;
	GDISP.Backlight = 100;
	GDISP.Contrast = 50;
	#if GDISP_NEED_VALIDATION || GDISP_NEED_CLIP
		GDISP.clipx0 = 0;
		GDISP.clipy0 = 0;
		GDISP.clipx1 = GDISP.Width;
		GDISP.clipy1 = GDISP.Height;
	#endif

	lld_lcdWriteReg(SYSR, SYSR_PARALLEL_DATA | SYSR_8BPP);

	lld_lcdWriteReg(PWRR, PWRR_LCD_ON);

    //lld_lcdResetViewPort();
    //lld_lcdSetViewPort(0, 0, 320, 240);

	return TRUE;
}

/**
 * @brief   Draws a pixel on the display.
 *
 * @param[in] x        X location of the pixel
 * @param[in] y        Y location of the pixel
 * @param[in] color    The color of the pixel
 *
 * @notapi
 */
void GDISP_LLD(drawpixel)(coord_t x, coord_t y, color_t color) {
	#if GDISP_NEED_VALIDATION || GDISP_NEED_CLIP
		if (x < GDISP.clipx0 || y < GDISP.clipy0 || x >= GDISP.clipx1 || y >= GDISP.clipy1) return;
	#endif
	lld_lcdSetCursor(x, y);
	lld_lcdWriteReg(0x0022, color);
}

/* ---- Optional Routines ---- */

#if GDISP_HARDWARE_CLEARS || defined(__DOXYGEN__)
	/**
	 * @brief   Clear the display.
	 * @note    Optional - The high level driver can emulate using software.
	 *
	 * @param[in] color    The color of the pixel
	 *
	 * @notapi
	 */
	void GDISP_LLD(clear)(color_t color) {
		/* set font background colour to color */
		//lld_lcdWriteReg(TBCR, 0x3E);
		//lld_lcdWriteReg(MCLR, MCLR_START | MCLR_FULL_SCREEN | MCLR_FONT_BG_COLOR);

		    //Memory_Clear_with_Font_BgColor
	lld_lcdWriteReg(0x8E, lld_lcdReadReg(0x8E) | 0x01);

	//Text_Background_Color(0xfc); 
	lld_lcdWriteReg(0x43, color);

	//Memory_Clear(); 
	lld_lcdWriteReg(0x8E, lld_lcdReadReg(0x8E) | 0x80);

	// chk_busy()
	while(lld_lcdReadStatus() & 0x80);
	}
#endif

#if GDISP_HARDWARE_FILLS || defined(__DOXYGEN__)
	/**
	 * @brief   Fill an area with a color.
	 * @note    Optional - The high level driver can emulate using software.
	 *
	 * @param[in] x, y     The start filled area
	 * @param[in] cx, cy   The width and height to be filled
	 * @param[in] color    The color of the fill
	 *
	 * @notapi
	 */
	void GDISP_LLD(fillarea)(coord_t x, coord_t y, coord_t cx, coord_t cy, color_t color) {
		unsigned i, area;

		#if GDISP_NEED_VALIDATION || GDISP_NEED_CLIP
			if (x < GDISP.clipx0) { cx -= GDISP.clipx0 - x; x = GDISP.clipx0; }
			if (y < GDISP.clipy0) { cy -= GDISP.clipy0 - y; y = GDISP.clipy0; }
			if (cx <= 0 || cy <= 0 || x >= GDISP.clipx1 || y >= GDISP.clipy1) return;
			if (x+cx > GDISP.clipx1)	cx = GDISP.clipx1 - x;
			if (y+cy > GDISP.clipy1)	cy = GDISP.clipy1 - y;
		#endif

		area = cx*cy;
		lld_lcdSetViewPort(x, y, cx, cy);
		lld_lcdWriteStreamStart();
		for(i = 0; i < area; i++)
			lld_lcdWriteData(color);
		lld_lcdWriteStreamStop();
		lld_lcdResetViewPort();
	}
#endif

#if GDISP_HARDWARE_BITFILLS || defined(__DOXYGEN__)
	/**
	 * @brief   Fill an area with a bitmap.
	 * @note    Optional - The high level driver can emulate using software.
	 *
	 * @param[in] x, y     The start filled area
	 * @param[in] cx, cy   The width and height to be filled
	 * @param[in] srcx, srcy   The bitmap position to start the fill from
	 * @param[in] srccx    The width of a line in the bitmap.
	 * @param[in] buffer   The pixels to use to fill the area.
	 *
	 * @notapi
	 */
	void GDISP_LLD(blitareaex)(coord_t x, coord_t y, coord_t cx, coord_t cy, coord_t srcx, coord_t srcy, coord_t srccx, const pixel_t *buffer) {
		coord_t endx, endy;
		unsigned lg;

		#if GDISP_NEED_VALIDATION || GDISP_NEED_CLIP
			if (x < GDISP.clipx0) { cx -= GDISP.clipx0 - x; srcx += GDISP.clipx0 - x; x = GDISP.clipx0; }
			if (y < GDISP.clipy0) { cy -= GDISP.clipy0 - y; srcy += GDISP.clipy0 - y; y = GDISP.clipy0; }
			if (srcx+cx > srccx)		cx = srccx - srcx;
			if (cx <= 0 || cy <= 0 || x >= GDISP.clipx1 || y >= GDISP.clipy1) return;
			if (x+cx > GDISP.clipx1)	cx = GDISP.clipx1 - x;
			if (y+cy > GDISP.clipy1)	cy = GDISP.clipy1 - y;
		#endif

		lld_lcdSetViewPort(x, y, cx, cy);
		lld_lcdWriteStreamStart();

		endx = srcx + cx;
		endy = y + cy;
		lg = srccx - cx;
		buffer += srcx + srcy * srccx;
		for(; y < endy; y++, buffer += lg)
			for(x=srcx; x < endx; x++)
				lld_lcdWriteData(*buffer++);
		lld_lcdWriteStreamStop();
		lld_lcdResetViewPort();
	}
#endif

#if (GDISP_NEED_PIXELREAD && GDISP_HARDWARE_PIXELREAD) || defined(__DOXYGEN__)
	/**
	 * @brief   Get the color of a particular pixel.
	 * @note    Optional.
	 * @note    If x,y is off the screen, the result is undefined.
	 *
	 * @param[in] x, y     The start of the text
	 *
	 * @notapi
	 */
	color_t GDISP_LLD(getpixelcolor)(coord_t x, coord_t y) {
		/* This routine is marked "DO NOT USE" in the original
		 *  GLCD driver. We just keep our GDISP_HARDWARE_READPIXEL
		 *  turned off for now.
		 */
		color_t color;

		#if GDISP_NEED_VALIDATION || GDISP_NEED_CLIP
			if (x < 0 || x >= GDISP.Width || y < 0 || y >= GDISP.Height) return 0;
		#endif

		lld_lcdSetCursor(x, y);
		lld_lcdWriteStreamStart();

		color = lld_lcdReadData();
		color = lld_lcdReadData();

		lld_lcdWriteStreamStop();

		return color;
	}
#endif

#if (GDISP_NEED_SCROLL && GDISP_HARDWARE_SCROLL) || defined(__DOXYGEN__)
	/**
	 * @brief   Scroll vertically a section of the screen.
	 * @note    Optional.
	 * @note    If x,y + cx,cy is off the screen, the result is undefined.
	 * @note    If lines is >= cy, it is equivelent to a area fill with bgcolor.
	 *
	 * @param[in] x, y     The start of the area to be scrolled
	 * @param[in] cx, cy   The size of the area to be scrolled
	 * @param[in] lines    The number of lines to scroll (Can be positive or negative)
	 * @param[in] bgcolor  The color to fill the newly exposed area.
	 *
	 * @notapi
	 */
	void GDISP_LLD(verticalscroll)(coord_t x, coord_t y, coord_t cx, coord_t cy, int lines, color_t bgcolor) {
		/* This is marked as "TODO: Test this" in the original GLCD driver.
		 * For now we just leave the GDISP_HARDWARE_SCROLL off.
		 */
		static color_t buf[((GDISP_SCREEN_HEIGHT > GDISP_SCREEN_WIDTH ) ? GDISP_SCREEN_HEIGHT : GDISP_SCREEN_WIDTH)];
		coord_t row0, row1;
		unsigned i, gap, abslines;

		#if GDISP_NEED_VALIDATION || GDISP_NEED_CLIP
			if (x < GDISP.clipx0) { cx -= GDISP.clipx0 - x; x = GDISP.clipx0; }
			if (y < GDISP.clipy0) { cy -= GDISP.clipy0 - y; y = GDISP.clipy0; }
			if (!lines || cx <= 0 || cy <= 0 || x >= GDISP.clipx1 || y >= GDISP.clipy1) return;
			if (x+cx > GDISP.clipx1)	cx = GDISP.clipx1 - x;
			if (y+cy > GDISP.clipy1)	cy = GDISP.clipy1 - y;
		#endif

		abslines = lines < 0 ? -lines : lines;
		if (abslines >= cy) {
			abslines = cy;
			gap = 0;
		} else {
			gap = cy - abslines;
			for(i = 0; i < gap; i++) {
				if(lines > 0) {
					row0 = y + i + lines;
					row1 = y + i;
				} else {
					row0 = (y - i - 1) + lines;
					row1 = (y - i - 1);
				}

				/* read row0 into the buffer and then write at row1*/
				lld_lcdSetViewPort(x, row0, cx, 1);
				lld_lcdReadStreamStart();
				lld_lcdReadStream(buf, cx);
				lld_lcdReadStreamStop();

				lld_lcdSetViewPort(x, row1, cx, 1);
				lld_lcdWriteStreamStart();
				lld_lcdWriteStream(buf, cx);
				lld_lcdWriteStreamStop();
			}
		}

		/* fill the remaining gap */
		lld_lcdSetViewPort(x, lines > 0 ? (y+gap) : y, cx, abslines);
		lld_lcdWriteStreamStart();
		gap = cx*abslines;
		for(i = 0; i < gap; i++) lld_lcdWriteData(bgcolor);
		lld_lcdWriteStreamStop();
		lld_lcdResetViewPort();
	}
#endif

#if GDISP_HARDWARE_CONTROL || defined(__DOXYGEN__)
	/**
	 * @brief   Driver Control
	 * @detail	Unsupported control codes are ignored.
	 * @note	The value parameter should always be typecast to (void *).
	 * @note	There are some predefined and some specific to the low level driver.
	 * @note	GDISP_CONTROL_POWER			- Takes a gdisp_powermode_t
	 * 			GDISP_CONTROL_ORIENTATION	- Takes a gdisp_orientation_t
	 * 			GDISP_CONTROL_BACKLIGHT -	 Takes an int from 0 to 100. For a driver
	 * 											that only supports off/on anything other
	 * 											than zero is on.
	 * 			GDISP_CONTROL_CONTRAST		- Takes an int from 0 to 100.
	 * 			GDISP_CONTROL_LLD			- Low level driver control constants start at
	 * 											this value.
	 *
	 * @param[in] what		What to do.
	 * @param[in] value		The value to use (always cast to a void *).
	 *
	 * @notapi
	 */
	void GDISP_LLD(control)(unsigned what, void *value) {
		switch(what) {
		case GDISP_CONTROL_POWER:
			if (GDISP.Powermode == (gdisp_powermode_t)value)
				return;
			switch((gdisp_powermode_t)value) {
				case powerOff:
					/* 	Code here */
					/* break; */
				case powerOn:
					/* 	Code here */
					/* You may need this ---
						if (GDISP.Powermode != powerSleep)
							GDISP_LLD(init();
					*/
					/* break; */
				case powerSleep:
					/* 	Code here */
					/* break; */
				default:
					return;
			}
			GDISP.Powermode = (gdisp_powermode_t)value;
			return;
		case GDISP_CONTROL_ORIENTATION:
			if (GDISP.Orientation == (gdisp_orientation_t)value)
				return;
			switch((gdisp_orientation_t)value) {
			case GDISP_ROTATE_0:
				lld_lcdWriteReg(0x0001,0x0127);
				lld_lcdWriteReg(0x03, 0b0011);
				GDISP.Height = GDISP_SCREEN_HEIGHT;
				GDISP.Width = GDISP_SCREEN_WIDTH;
				break;
			case GDISP_ROTATE_90:
				lld_lcdWriteReg(0x0001,0x0027);
				lld_lcdWriteReg(0x0003, 0b1011);
				GDISP.Height = GDISP_SCREEN_WIDTH;
				GDISP.Width = GDISP_SCREEN_HEIGHT;
				break;
			case GDISP_ROTATE_180:
				lld_lcdWriteReg(0x0001,0x0127);
				lld_lcdWriteReg(0x0003, 0b0000);
				GDISP.Height = GDISP_SCREEN_HEIGHT;
				GDISP.Width = GDISP_SCREEN_WIDTH;
				break;
			case GDISP_ROTATE_270:
				lld_lcdWriteReg(0x0001,0x0027);
				lld_lcdWriteReg(0x0003, 0b1000);
				GDISP.Height = GDISP_SCREEN_WIDTH;
				GDISP.Width = GDISP_SCREEN_HEIGHT;
				break;
			default:
				return;
			}
			#if GDISP_NEED_CLIP || GDISP_NEED_VALIDATION
				GDISP.clipx0 = 0;
				GDISP.clipy0 = 0;
				GDISP.clipx1 = GDISP.Width;
				GDISP.clipy1 = GDISP.Height;
			#endif
			GDISP.Orientation = (gdisp_orientation_t)value;
			return;
/*
		case GDISP_CONTROL_BACKLIGHT:
		case GDISP_CONTROL_CONTRAST:
*/
		}
	}
#endif

#endif /* HAL_USE_GDISP */
/** @} */
