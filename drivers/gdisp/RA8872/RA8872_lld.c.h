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

#ifndef RA8872_H
#define RA8872_H

#define PWRR                    0x01
#define PWRR_LCD_ON             0x80
#define PWRR_LCD_OFF            0x00
#define PWRR_SLEEP_ON           0x02
#define PWRR_SLEEP_OFF          0x00
#define PWRR_RESET_ON           0x01
#define PWRR_RESET_OFF          0x00

#define SYSR                    0x10
#define SYSR_PARALLEL_DATA      0x00
#define SYSR_SERIAL_DATA        0x40
#define SYSR_8BPP               0x00
#define SYSR_12BPP              0x04
#define SYSR_16BPP              0x08

#define DRGB                    0x11
#define IOCR                    0x12
#define IODR                    0x13

#define HDWR                    0x14

#define HNDFTR                  0x15
#define HNDFTR_DE_ACT_HIGH      0x00
#define HNDFTR_DE_ACT_LOW       0x80

#define HNDR                    0x16
#define HSTR                    0x17
#define HPWR                    0x18

#define HSAW0                   0x30
#define HSAW1                   0x31
#define VSAW0                   0x32
#define VSAW1                   0x33
#define HEAW0                   0x34
#define HEAW1                   0x35
#define VEAW0                   0x36
#define VEAW1                   0x37
#define TFCR					0x42
#define TBCR					0x43


#define PLLC1                   0x88
#define PLLDIVM_BY_1            0x00
#define PLLDIVM_BY_2            0x80
#define PLLC2                   0x89
#define PLLC2_DIV_BY_8          0x03

#define MCLR					0x8E
#define MCLR_STOP				0x00
#define MCLR_START				0x80
#define MCLR_FULL_SCREEN		0x00
#define MCLR_ACTIVE_WINDOW		0x40
#define MCLR_BTE_BG_COLOR		0x00
#define MCLR_FONT_BG_COLOR		0x01



// I/O assignments
#define GDISP_BL_GPIO			GPIOB
#define GDISP_BL_PIN			8

/* all interfaces use RST via GPIO */
/* TODO: option to disable RST; assumes RST is tied high */
#define GDISP_RST_LOW		palClearPad(GDISP_RST_GPIO, GDISP_RST_PIN)
#define GDISP_RST_HIGH	palSetPad(GDISP_RST_GPIO, GDISP_RST_PIN)

#define RA8872_delay(n) 	halPolledDelay(MS2RTT(n));

#if defined(GDISP_USE_GPIO)

#elif defined(GDISP_USE_FSMC)
	#define GDISP_REG              (*((volatile uint8_t *) 0x60000000)) /* RS = 0 */
	#define GDISP_RAM              (*((volatile uint8_t *) 0x60020000)) /* RS = 1 */

	static __inline void lld_lcdWriteIndex(uint8_t index)			{ GDISP_REG = index; }
	static __inline void lld_lcdWriteData(uint8_t data)			{ GDISP_RAM = data; }
	static __inline void lld_lcdWriteReg(uint8_t lcdReg,uint8_t lcdRegValue) {
		GDISP_REG = lcdReg;
		GDISP_RAM = lcdRegValue;
	}
	static __inline uint8_t lld_lcdReadData(void)					{ return (GDISP_RAM); }
	static __inline uint8_t lld_lcdReadReg(uint16_t lcdReg) {
		GDISP_REG = lcdReg;
		return GDISP_RAM;
	}
	static __inline void lld_lcdWriteStreamStart(void)				{ GDISP_REG = 0x0022; }
	static __inline void lld_lcdWriteStreamStop(void)				{}
	static __inline void lld_lcdWriteStream(uint8_t *buffer, size_t size) {
		uint16_t i;
		for(i = 0; i < size; i++) GDISP_RAM = buffer[i];
	}
	static __inline void lld_lcdReadStreamStart(void)				{ GDISP_REG = 0x0022; }
	static __inline void lld_lcdReadStreamStop(void)				{}
	static __inline void lld_lcdReadStream(uint8_t *buffer, size_t size) {
		uint16_t i;
		volatile uint16_t dummy;

		/* throw away first value read */
		dummy = GDISP_RAM;
		for(i = 0; i < size; i++) buffer[i] = GDISP_RAM;
	}

#elif defined(GDISP_USE_SPI)
	#error "gdispRA8872: GDISP_USE_SPI not implemented yet"
#endif

static void lld_lcdSetCursor(coord_t x, coord_t y) {
	/* R20h - 8 bit
	 * R21h - 9 bit
	 */
	switch(GDISP.Orientation) {
		case GDISP_ROTATE_0:
			break;
		case GDISP_ROTATE_90:
		    /* Note X has already been mirrored, so we do it directly */
			break;
		case GDISP_ROTATE_180:
            break;
		case GDISP_ROTATE_270:
			break;
	}
}

static void lld_lcdActiveWindow(uint16_t x, uint16_t y, uint16_t cx, uint16_t cy) {
    /* x min */
    lld_lcdWriteReg(HSAW0, (uint8_t)(x && 0x00FF));
    lld_lcdWriteReg(HSAW1, (uint8_t)((x >> 8) && 0x00FF));
    /* x max */
    lld_lcdWriteReg(HEAW0, (uint8_t)(cx && 0x00FF));
    lld_lcdWriteReg(HEAW1, (uint8_t)((cx >> 8) && 0x00FF));

    /* y min */
    lld_lcdWriteReg(VSAW0, (uint8_t)(y && 0x00FF));
    lld_lcdWriteReg(VSAW1, (uint8_t)((y >> 8) && 0x00FF));
    /* y max */
    lld_lcdWriteReg(VEAW0, (uint8_t)(cy && 0x00FF));
    lld_lcdWriteReg(VEAW1, (uint8_t)((cy >> 8) && 0x00FF));
}

static void lld_lcdSetViewPort(uint16_t x, uint16_t y, uint16_t cx, uint16_t cy) {
    /* cx and cy specify the number of pixels, subtract one to get end pixel */
    cx--;
    cy--;

	switch(GDISP.Orientation) {
		case GDISP_ROTATE_0:
            lld_lcdActiveWindow(x, y, cx, cy);
			break;
		case GDISP_ROTATE_90:
            lld_lcdActiveWindow(y, x, cy, cx);
			break;
		case GDISP_ROTATE_180:
            lld_lcdActiveWindow(GDISP_SCREEN_WIDTH - 1 - x, GDISP_SCREEN_HEIGHT - (y + cy),
                                GDISP_SCREEN_WIDTH - (x+cx), GDISP_SCREEN_HEIGHT- 1 - y);
			break;
		case GDISP_ROTATE_270:
            lld_lcdActiveWindow(GDISP_SCREEN_HEIGHT - (y + cy), GDISP_SCREEN_WIDTH - 1 - x,
                                GDISP_SCREEN_HEIGHT- 1 - y, GDISP_SCREEN_WIDTH - (x+cx));
			break;
	}

	lld_lcdSetCursor(x, y);
}

static void lld_lcdResetViewPort(void) {
	switch(GDISP.Orientation) {
		case GDISP_ROTATE_0:
		case GDISP_ROTATE_180:
			lld_lcdSetViewPort(0, 0, GDISP_SCREEN_WIDTH, GDISP_SCREEN_HEIGHT);
			break;
		case GDISP_ROTATE_90:
		case GDISP_ROTATE_270:
			lld_lcdSetViewPort(0, 0, GDISP_SCREEN_HEIGHT, GDISP_SCREEN_WIDTH);
			break;
	}
}

#endif /* RA8872_H */
