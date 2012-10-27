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

static void lld_lcdSetViewPort(uint16_t x, uint16_t y, uint16_t cx, uint16_t cy) {
	/* HSA / HEA are 8 bit
	 * VSA / VEA are 9 bit
	 * use masks 0x00FF and 0x01FF to enforce this
	 */

	switch(GDISP.Orientation) {
		case GDISP_ROTATE_0:
			break;
		case GDISP_ROTATE_90:
			break;
		case GDISP_ROTATE_180:
			break;
		case GDISP_ROTATE_270:
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
