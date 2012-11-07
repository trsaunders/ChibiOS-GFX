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
 * @file    gdispRA8872/gdisp_lld_config.h
 * @brief   GDISP Graphic Driver subsystem low level driver header for the RA8872 display.
 *
 * @addtogroup GDISP
 * @{
 */

#ifndef _GDISP_LLD_CONFIG_H
#define _GDISP_LLD_CONFIG_H

#if HAL_USE_GDISP

/*===========================================================================*/
/* Driver hardware support.                                                  */
/*===========================================================================*/

#define GDISP_DRIVER_NAME				"RA8872"
#define GDISP_LLD(x)					gdisp_lld_##x##_RA8872

#define GDISP_HARDWARE_CLEARS			TRUE
#define GDISP_HARDWARE_FILLS			TRUE
#define GDISP_HARDWARE_BITFILLS			TRUE
#define GDISP_HARDWARE_SCROLL			TRUE
#define GDISP_HARDWARE_PIXELREAD		FALSE
#define GDISP_HARDWARE_CONTROL			TRUE

#if !defined(GDISP_PIXELFORMAT)
#define GDISP_PIXELFORMAT				GDISP_PIXELFORMAT_RGB332
#elif (GDISP_PIXELFORMAT != GDISP_PIXELFORMAT_RGB332) && \
	  (GDISP_PIXELFORMAT != GDISP_PIXELFORMAT_RGB444) && \
	  (GDISP_PIXELFORMAT != GDISP_PIXELFORMAT_RGB565)
#error "RA8872 only supports pixel formats 332, 444, 565"
#endif

#endif	/* HAL_USE_GDISP */

#endif	/* _GDISP_LLD_CONFIG_H */
/** @} */
