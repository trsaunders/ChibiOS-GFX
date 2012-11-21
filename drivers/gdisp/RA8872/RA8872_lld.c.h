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


#define STSR_BTE_BUSY			0x40



#define PWRR                    0x01
#define PWRR_LCD_ON             0x80
#define PWRR_LCD_OFF            0x00
#define PWRR_SLEEP_ON           0x02
#define PWRR_SLEEP_OFF          0x00
#define PWRR_RESET_ON           0x01
#define PWRR_RESET_OFF          0x00

#define MRWC					0x02

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

#define DPCR					0x20
#define DPCR_ROT_0				0x00
#define DPCR_ROT_90				0x01
#define DPCR_ROT_180			0x02
#define DPCR_ROT_270			0x03

#define HOFS0					0x24
#define HOFS1					0x25
#define VOFS0					0x26
#define VOFS1					0x27

#define HSAW0                   0x30
#define HSAW1                   0x31
#define VSAW0                   0x32
#define VSAW1                   0x33
#define HEAW0                   0x34
#define HEAW1                   0x35
#define VEAW0                   0x36
#define VEAW1                   0x37
#define HSSW0					0x38
#define HSSW1					0x39
#define VSSW0					0x3A
#define VSSW1					0x3B
#define HESW0					0x3C
#define HESW1					0x3D
#define VESW0					0x3E
#define VESW1					0x3F

#define TFCR					0x42
#define TBCR					0x43
#define CURH0					0x46
#define CURH1					0x47
#define CURV0					0x48
#define CURV1					0x49
#define BECR0					0x50
#define BECR1					0x51
#define LTPR0					0x52
#define LTPR1					0x53
#define HSBE0					0x54
#define HSBE1					0x55
#define VSBE0					0x56
#define VSBE1					0x57
#define HDBE0					0x58
#define HDBE1					0x59
#define VDBE0					0x5A
#define VDBE1					0x5B
#define BEWR0					0x5C
#define BEWR1					0x5D
#define BEHR0					0x5E
#define BEHR1					0x5F

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

#define DCR 					0x90
#define DCR_DRAW_START			0x80
#define DCR_DRAW_CIRCLE_START	0x40
#define DCR_FILL_AREA			0x20
#define DCR_DRAW_LINE			0x00
#define DCR_DRAW_SQUARE			0x10



#define DLHSR0					0x91
#define DLHSR1					0x92
#define DLVSR0					0x93
#define DLVSR1					0x94
#define DLHER0					0x95
#define DLHER1					0x96
#define DLVER0					0x97
#define DLVER1					0x98


// I/O assignments
#define GDISP_BL_GPIO			GPIOB
#define GDISP_BL_PIN			8

/* all interfaces use RST via GPIO */
/* TODO: option to disable RST; assumes RST is tied high */
#define GDISP_RST_LOW		palClearPad(GDISP_RST_GPIO, GDISP_RST_PIN)
#define GDISP_RST_HIGH	palSetPad(GDISP_RST_GPIO, GDISP_RST_PIN)

#define RA8872_delay(n) 	halPolledDelay(MS2RTT(n));

#define RA8872_WRITE_COORD16(REG, x) \
    lld_lcdWriteReg(REG, (uint8_t)x); \
	lld_lcdWriteReg(REG+1, (uint8_t)(x >> 8) & 0x03);


#if defined(GDISP_USE_GPIO)

#elif defined(GDISP_USE_FSMC)
	#define GDISP_REG              (*((volatile uint8_t *) 0x60010000)) /* RS = 0 */
	#define GDISP_RAM              (*((volatile uint8_t *) 0x60000000)) /* RS = 1 */

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
	static __inline void lld_lcdWriteStreamStart(void)				{ GDISP_REG = MRWC; }
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

	static __inline uint8_t lld_lcdReadStatus(void) {
		return GDISP_REG;
	}

#endif

static void lld_lcdSetCursorXY(uint16_t x, uint16_t y) {
	lld_lcdWriteReg(CURH0, (uint8_t)x);
	lld_lcdWriteReg(CURH1, (uint8_t)(x >> 8));

	lld_lcdWriteReg(CURV0, (uint8_t)y);
	lld_lcdWriteReg(CURV1, (uint8_t)(y >> 8));
}

static void lld_lcdSetCursor(coord_t x, coord_t y) {
	/* R20h - 8 bit
	 * R21h - 9 bit
	 */
	switch(GDISP.Orientation) {
		case GDISP_ROTATE_0:
			lld_lcdSetCursorXY(x, y);
			break;
		case GDISP_ROTATE_90:
			lld_lcdSetCursorXY(y, x);
		    /* Note X has already been mirrored, so we do it directly */
			break;
		case GDISP_ROTATE_180:
            break;
		case GDISP_ROTATE_270:
			break;
	}
}

static void lld_lcdSetDrawCoords_(uint16_t x, uint16_t y, uint16_t cx, uint16_t cy) {
	lld_lcdWriteReg(DLHSR0, (uint8_t)x);
	lld_lcdWriteReg(DLHSR1, (uint8_t)(x >> 8));

	lld_lcdWriteReg(DLVSR0, (uint8_t)y);
	lld_lcdWriteReg(DLVSR1, (uint8_t)(y >> 8));

	lld_lcdWriteReg(DLHER0, (uint8_t)cx);
	lld_lcdWriteReg(DLHER1, (uint8_t)(cx >> 8));

	lld_lcdWriteReg(DLVER0, (uint8_t)cy);
	lld_lcdWriteReg(DLVER1, (uint8_t)(cy >> 8));
}

static void lld_lcdSetDrawCoords(uint16_t x, uint16_t y, uint16_t cx, uint16_t cy) {
	switch(GDISP.Orientation) {
		case GDISP_ROTATE_0:
            lld_lcdSetDrawCoords_(x, y, cx, cy);
			break;
		case GDISP_ROTATE_90:
            lld_lcdSetDrawCoords_(y, x, cy, cx);
			break;
		case GDISP_ROTATE_180:
            lld_lcdSetDrawCoords_(GDISP_SCREEN_WIDTH - 1 - x, GDISP_SCREEN_HEIGHT - (y + cy),
                                GDISP_SCREEN_WIDTH - (x+cx), GDISP_SCREEN_HEIGHT- 1 - y);
			break;
		case GDISP_ROTATE_270:
            lld_lcdSetDrawCoords_(GDISP_SCREEN_HEIGHT - (y + cy), GDISP_SCREEN_WIDTH - 1 - x,
                                GDISP_SCREEN_HEIGHT- 1 - y, GDISP_SCREEN_WIDTH - (x+cx));
			break;
	}	
}

static void lld_lcdActiveWindow(uint16_t x, uint16_t y, uint16_t cx, uint16_t cy) {
    /* x min */
    lld_lcdWriteReg(HSAW0, (uint8_t)(x & 0x00FF));
    lld_lcdWriteReg(HSAW1, (uint8_t)((x >> 8) & 0x00FF));
    /* x max */
    lld_lcdWriteReg(HEAW0, (uint8_t)(cx & 0x00FF));
    lld_lcdWriteReg(HEAW1, (uint8_t)((cx >> 8) & 0x00FF));

    /* y min */
    lld_lcdWriteReg(VSAW0, (uint8_t)(y & 0x00FF));
    lld_lcdWriteReg(VSAW1, (uint8_t)((y >> 8) & 0x00FF));
    /* y max */
    lld_lcdWriteReg(VEAW0, (uint8_t)(cy & 0x00FF));
    lld_lcdWriteReg(VEAW1, (uint8_t)((cy >> 8) & 0x00FF));
}

// lld_lcdSetViewPort(0, 0, 319, 239);
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
			lld_lcdSetViewPort(0, 0, GDISP_SCREEN_HEIGHT, GDISP_SCREEN_WIDTH);
			break;
		case GDISP_ROTATE_90:
		case GDISP_ROTATE_270:
			lld_lcdSetViewPort(0, 0, GDISP_SCREEN_WIDTH, GDISP_SCREEN_HEIGHT);
			break;
	}
}

static void lld_lcdSetScrollWindow_(uint16_t x, uint16_t y, uint16_t cx, uint16_t cy) {
	cx--;
	cy--;
	lld_lcdWriteReg(HSSW0, (uint8_t)x);
	lld_lcdWriteReg(HSSW1, (uint8_t)(x >> 8) & 0x03);

	lld_lcdWriteReg(HESW0, (uint8_t)cx);
	lld_lcdWriteReg(HESW1, (uint8_t)(cx >> 8) & 0x03);

	lld_lcdWriteReg(VSSW0, (uint8_t)y);
	lld_lcdWriteReg(VSSW1, (uint8_t)(y >> 8) & 0x03);

	lld_lcdWriteReg(VESW0, (uint8_t)cy);
	lld_lcdWriteReg(VESW1, (uint8_t)(cy >> 8) & 0x03);
}

static void lld_lcdSetScrollWindow(uint16_t x, uint16_t y, uint16_t cx, uint16_t cy) {
	switch(GDISP.Orientation) {
		case GDISP_ROTATE_0:
		case GDISP_ROTATE_180:
			lld_lcdSetScrollWindow_(x, y, cx, cy);
			break;
		case GDISP_ROTATE_90:
		case GDISP_ROTATE_270:
			lld_lcdSetScrollWindow_(y, x, cy, cx);
			break;
	}
}

/*
#define BECR0					0x50
#define BECR1					0x51
#define LTPR0					0x52
#define LTPR1					0x53
#define HSBE0					0x54
#define HSBE1					0x55
#define VSBE0					0x56
#define VSBE1					0x57
#define HDBE0					0x58
#define HDBE1					0x59
#define VDBE0					0x5A
#define VDBE1					0x5B
#define BEWR0					0x5C
#define BEWR1					0x5D
#define BEHR0					0x5E
#define BEHR1					0x5F
*/

static void lld_lcdCopyRegion(	uint16_t w, uint16_t h,
								uint16_t srcx, uint16_t srcy,
								uint16_t destx, uint16_t desty) {
	uint16_t sx, sy, dx, dy;

	lld_lcdResetViewPort();

	sx = srcx; sy = srcy;
	dx = destx; dy = desty;

	/* transform x,y to screen coordinates */
	#if 0
	switch(GDISP.Orientation) {
	 	case GDISP_ROTATE_0:
	 		sx = srcx; sy = srcy;
	 		dx = destx; dy = desty;
	 		break;
	 	case GDISP_ROTATE_90:
	 		sx = srcy; sy = srcx;
	 		dx = desty; dy = destx;
	 		break;
	 	case GDISP_ROTATE_180:
	 	case GDISP_ROTATE_270:
	 		/* TODO */
	 	default:
	 		break;
	}
	#endif

	/* get a linear index for source and destination */
	/* TODO: fix for orientation */
	unsigned int s = sy*GDISP_SCREEN_WIDTH + sx;
	unsigned int d = dy*GDISP_SCREEN_WIDTH + dx;

	sx = (d < s) ? sx : (sx + w);
	sy = (d < s) ? sy : (sy + h);
	dx = (d < s) ? dx : (dx + w);
	dy = (d < s) ? dy : (dy + h);

	uint8_t sl = 0;
	uint8_t dl = 0;

 	 	/* source address */
	lld_lcdWriteReg(HSBE0, (uint8_t)(sx & 0x00FF));
	lld_lcdWriteReg(HSBE1, ((uint8_t)((sx >> 8) & 0x0003)) | (sl) ? 0x80 : 0x00);

	lld_lcdWriteReg(VSBE0, (uint8_t)(sy & 0x00FF));
	lld_lcdWriteReg(VSBE1, ((uint8_t)((sy >> 8) & 0x0003)) | (sl) ? 0x80 : 0x00);

 	/* destination address */
 	lld_lcdWriteReg(HDBE0, (uint8_t)(dx & 0x00FF));
	lld_lcdWriteReg(HDBE1, ((uint8_t)((dx >> 8) & 0x0003)) | (dl) ? 0x80 : 0x00);

	lld_lcdWriteReg(VDBE0, (uint8_t)(dy & 0x00FF));
	lld_lcdWriteReg(VDBE1, ((uint8_t)((dy >> 8) & 0x0003)) | (dl) ? 0x80 : 0x00);

 	/* BTE width and help */
 	lld_lcdWriteReg(BEWR0, (uint8_t)(w & 0x00FF));
 	lld_lcdWriteReg(BEWR1, (uint8_t)((w >> 8) & 0x0003));

 	lld_lcdWriteReg(BEHR0, (uint8_t)(h & 0x00FF));
 	lld_lcdWriteReg(BEHR1, (uint8_t)((h >> 8) & 0x0003));

 	/* set BTE operation and ROP function, 
 	depending on negative or positive BTE direction 
 	Set ROP function as Source */
 	lld_lcdWriteReg(BECR1, ((d < s) ? 0x02 : 0x03) | (0b1100 << 4));

 	/* enable BTE function */
 	lld_lcdWriteReg(BECR0, 0x80);

 	while(lld_lcdReadReg(BECR0))

	 /* wait for complete */
	 while(lld_lcdReadStatus() & STSR_BTE_BUSY);
}

#endif /* RA8872_H */
