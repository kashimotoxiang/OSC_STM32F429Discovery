/*********************************************************************
*          Portions COPYRIGHT 2013 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2013  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.22 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUIDRV_Template_ESP.c
Purpose     : Template driver, could be used as starting point for new
              simple display drivers supporting only one color depth.
---------------------------END-OF-HEADER------------------------------
*/

/**
  ******************************************************************************
  * @attention
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#include <stddef.h>
#include "LCD_MD050SD.h"
#include "LCD_Private.h"
#include "GUI_Private.h"
#include "LCD_ConfDefaults.h"
#include "LCD.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
/*********************************************************************
*
*       Macros for MIRROR_, SWAP_ and LUT_
*/
#if (!defined (LCD_LUT_COM) && !defined(LCD_LUT_SEG))
#if (!LCD_MIRROR_X && !LCD_MIRROR_Y && !LCD_SWAP_XY) 
#define LOG2PHYS_X(x, y) x
#define LOG2PHYS_Y(x, y) y
#elif (!LCD_MIRROR_X && !LCD_MIRROR_Y && LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) y
    #define LOG2PHYS_Y(x, y) x
#elif (!LCD_MIRROR_X && LCD_MIRROR_Y && !LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) x
    #define LOG2PHYS_Y(x, y) LCD_YSIZE - 1 - (y)
#elif (!LCD_MIRROR_X && LCD_MIRROR_Y && LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) y
    #define LOG2PHYS_Y(x, y) LCD_XSIZE - 1 - (x)
#elif ( LCD_MIRROR_X && !LCD_MIRROR_Y && !LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) LCD_XSIZE - 1 - (x)
    #define LOG2PHYS_Y(x, y) y
#elif ( LCD_MIRROR_X && !LCD_MIRROR_Y && LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) LCD_YSIZE - 1 - (y)
    #define LOG2PHYS_Y(x, y) x
#elif ( LCD_MIRROR_X && LCD_MIRROR_Y && !LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) LCD_XSIZE - 1 - (x)
    #define LOG2PHYS_Y(x, y) LCD_YSIZE - 1 - (y)
#elif ( LCD_MIRROR_X && LCD_MIRROR_Y && LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) LCD_YSIZE - 1 - (y)
    #define LOG2PHYS_Y(x, y) LCD_XSIZE - 1 - (x)
#endif
#else
#if ( defined (LCD_LUT_COM) && !defined(LCD_LUT_SEG))
    #define LOG2PHYS_X(x, y) x
    #define LOG2PHYS_Y(x, y) LCD__aLine2Com0[y]
#elif (!defined (LCD_LUT_COM) && defined(LCD_LUT_SEG))
    #define LOG2PHYS_X(x, y) LCD__aCol2Seg0[x]
    #define LOG2PHYS_Y(x, y) y
#elif ( defined (LCD_LUT_COM) && defined(LCD_LUT_SEG))
    #define LOG2PHYS_X(x, y) LCD__aCol2Seg0[x]
    #define LOG2PHYS_Y(x, y) LCD__aLine2Com0[y]
#endif
#endif

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct {
	U32 VRAMAddr;
	int xSize, ySize;
	int vxSize, vySize;
	int vxSizePhys;
	int BitsPerPixel;
} DRIVER_CONTEXT_TEMPLATE;

/*********************************************************************
*
*       Static functions
*
**********************************************************************
*/
/*********************************************************************
 打点函数
*/
static void ESP_SetPixelIndex (GUI_DEVICE* pDevice, int x, int y, int PixelIndex) {
	POINT_COLOR = PixelIndex;
	ESP_LCD_DrawPoint(x, y); //调用ILI93xx.c文件中的快速打点函数
}


/*********************************************************************
*
	读点函数
*/
static unsigned int ESP_GetPixelIndex (GUI_DEVICE* pDevice, int x, int y) {
	unsigned int PixelIndex;
#if (LCD_MIRROR_X == 1) || (LCD_MIRROR_Y == 1) || (LCD_SWAP_XY == 1)
      int xPhys, yPhys;

      xPhys = LOG2PHYS_X(x, y);
      yPhys = LOG2PHYS_Y(x, y);
#else
#define xPhys x
#define yPhys y
#endif
	GUI_USE_PARA(pDevice);
	GUI_USE_PARA(x);
	GUI_USE_PARA(y); {
		PixelIndex = ESP_LCD_ReadPoint(x, y);
	}
#if (LCD_MIRROR_X == 0) && (LCD_MIRROR_Y == 0) && (LCD_SWAP_XY == 0)
#undef xPhys
#undef yPhys
#endif
	return PixelIndex;

}

/*********************************************************************
*
*       ESP_XorPixel
*/
static void ESP_XorPixel (GUI_DEVICE* pDevice, int x, int y) {
	LCD_PIXELINDEX PixelIndex;
	LCD_PIXELINDEX IndexMask;

	PixelIndex = ESP_GetPixelIndex(pDevice, x, y);
	IndexMask = pDevice->pColorConvAPI->pfGetIndexMask();
	ESP_SetPixelIndex(pDevice, x, y, PixelIndex ^ IndexMask);
}

/*********************************************************************
*
*       ESP_FillRect
*/
static void ESP_FillRect (GUI_DEVICE* pDevice, int x0, int y0, int x1, int y1) {
	ESP_LCD_Fill(x0, y0, x1, y1,LCD_COLORINDEX);
}


/*********************************************************************
*
*       ESP_DrawHLine
*/
static void ESP_DrawHLine (GUI_DEVICE* pDevice, int x0, int y, int x1) {
	ESP_FillRect(pDevice, x0, y, x1, y);
}

/*********************************************************************
*
*       ESP_DrawVLine, not optimized
*/
static void ESP_DrawVLine (GUI_DEVICE* pDevice, int x, int y0, int y1) {
	ESP_FillRect(pDevice, x, y0, x, y1);
}

/*********************************************************************
*
*       Draw Bitmap 1 BPP
*/
static void ESP_DrawBitLine1BPP (GUI_DEVICE* pDevice, int x, int y, U8 const GUI_UNI_PTR * p, int Diff, int xsize, const LCD_PIXELINDEX* pTrans) {
	LCD_PIXELINDEX IndexMask, Index0, Index1, Pixel;

	Index0 = *(pTrans + 0);
	Index1 = *(pTrans + 1);
	x += Diff;
	switch (GUI_pContext->DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
		case 0:
			do {
				ESP_SetPixelIndex(pDevice, x++, y, (*p & (0x80 >> Diff)) ? Index1 : Index0);
				if (++Diff == 8) {
					Diff = 0;
					p++;
				}
			}
			while (--xsize);
			break;
		case LCD_DRAWMODE_TRANS:
			do {
				if (*p & (0x80 >> Diff))
					ESP_SetPixelIndex(pDevice, x, y, Index1);
				x++;
				if (++Diff == 8) {
					Diff = 0;
					p++;
				}
			}
			while (--xsize);
			break;
		case LCD_DRAWMODE_XOR | LCD_DRAWMODE_TRANS:
		case LCD_DRAWMODE_XOR:
			IndexMask = pDevice->pColorConvAPI->pfGetIndexMask();
			do {
				if (*p & (0x80 >> Diff)) {
					Pixel = ESP_GetPixelIndex(pDevice, x, y);
					ESP_SetPixelIndex(pDevice, x, y, Pixel ^ IndexMask);
				}
				x++;
				if (++Diff == 8) {
					Diff = 0;
					p++;
				}
			}
			while (--xsize);
			break;
	}
}

/*********************************************************************
*
*       Draw Bitmap 2 BPP
*/
static void ESP_DrawBitLine2BPP (GUI_DEVICE* pDevice, int x, int y, U8 const GUI_UNI_PTR * p, int Diff, int xsize, const LCD_PIXELINDEX* pTrans) {
	LCD_PIXELINDEX Pixels, PixelIndex;
	int CurrentPixel, Shift, Index;

	Pixels = *p;
	CurrentPixel = Diff;
	x += Diff;
	switch (GUI_pContext->DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
		case 0:
			if (pTrans) {
				do {
					Shift = (3 - CurrentPixel) << 1;
					Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
					PixelIndex = *(pTrans + Index);
					ESP_SetPixelIndex(pDevice, x++, y, PixelIndex);
					if (++CurrentPixel == 4) {
						CurrentPixel = 0;
						Pixels = *(++p);
					}
				}
				while (--xsize);
			}
			else {
				do {
					Shift = (3 - CurrentPixel) << 1;
					Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
					ESP_SetPixelIndex(pDevice, x++, y, Index);
					if (++CurrentPixel == 4) {
						CurrentPixel = 0;
						Pixels = *(++p);
					}
				}
				while (--xsize);
			}
			break;
		case LCD_DRAWMODE_TRANS:
			if (pTrans) {
				do {
					Shift = (3 - CurrentPixel) << 1;
					Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
					if (Index) {
						PixelIndex = *(pTrans + Index);
						ESP_SetPixelIndex(pDevice, x, y, PixelIndex);
					}
					x++;
					if (++CurrentPixel == 4) {
						CurrentPixel = 0;
						Pixels = *(++p);
					}
				}
				while (--xsize);
			}
			else {
				do {
					Shift = (3 - CurrentPixel) << 1;
					Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
					if (Index) {
						ESP_SetPixelIndex(pDevice, x, y, Index);
					}
					x++;
					if (++CurrentPixel == 4) {
						CurrentPixel = 0;
						Pixels = *(++p);
					}
				}
				while (--xsize);
			}
			break;
	}
}

/*********************************************************************
*
*       Draw Bitmap 4 BPP
*/
static void ESP_DrawBitLine4BPP (GUI_DEVICE* pDevice, int x, int y, U8 const GUI_UNI_PTR * p, int Diff, int xsize, const LCD_PIXELINDEX* pTrans) {
	LCD_PIXELINDEX Pixels, PixelIndex;
	int CurrentPixel, Shift, Index;

	Pixels = *p;
	CurrentPixel = Diff;
	x += Diff;
	switch (GUI_pContext->DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
		case 0:
			if (pTrans) {
				do {
					Shift = (1 - CurrentPixel) << 2;
					Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
					PixelIndex = *(pTrans + Index);
					ESP_SetPixelIndex(pDevice, x++, y, PixelIndex);
					if (++CurrentPixel == 2) {
						CurrentPixel = 0;
						Pixels = *(++p);
					}
				}
				while (--xsize);
			}
			else {
				do {
					Shift = (1 - CurrentPixel) << 2;
					Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
					ESP_SetPixelIndex(pDevice, x++, y, Index);
					if (++CurrentPixel == 2) {
						CurrentPixel = 0;
						Pixels = *(++p);
					}
				}
				while (--xsize);
			}
			break;
		case LCD_DRAWMODE_TRANS:
			if (pTrans) {
				do {
					Shift = (1 - CurrentPixel) << 2;
					Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
					if (Index) {
						PixelIndex = *(pTrans + Index);
						ESP_SetPixelIndex(pDevice, x, y, PixelIndex);
					}
					x++;
					if (++CurrentPixel == 2) {
						CurrentPixel = 0;
						Pixels = *(++p);
					}
				}
				while (--xsize);
			}
			else {
				do {
					Shift = (1 - CurrentPixel) << 2;
					Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
					if (Index) {
						ESP_SetPixelIndex(pDevice, x, y, Index);
					}
					x++;
					if (++CurrentPixel == 2) {
						CurrentPixel = 0;
						Pixels = *(++p);
					}
				}
				while (--xsize);
			}
			break;
	}
}

/*********************************************************************
*
*       Draw Bitmap 8 BPP
*/
static void ESP_DrawBitLine8BPP (GUI_DEVICE* pDevice, int x, int y, U8 const GUI_UNI_PTR * p, int xsize, const LCD_PIXELINDEX* pTrans) {
	LCD_PIXELINDEX Pixel;

	switch (GUI_pContext->DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
		case 0:
			if (pTrans) {
				for (; xsize > 0; xsize-- , x++ , p++) {
					Pixel = *p;
					ESP_SetPixelIndex(pDevice, x, y, *(pTrans + Pixel));
				}
			}
			else {
				for (; xsize > 0; xsize-- , x++ , p++) {
					ESP_SetPixelIndex(pDevice, x, y, *p);
				}
			}
			break;
		case LCD_DRAWMODE_TRANS:
			if (pTrans) {
				for (; xsize > 0; xsize-- , x++ , p++) {
					Pixel = *p;
					if (Pixel) {
						ESP_SetPixelIndex(pDevice, x, y, *(pTrans + Pixel));
					}
				}
			}
			else {
				for (; xsize > 0; xsize-- , x++ , p++) {
					Pixel = *p;
					if (Pixel) {
						ESP_SetPixelIndex(pDevice, x, y, Pixel);
					}
				}
			}
			break;
	}
}

/*********************************************************************
*
*       Draw Bitmap 16 BPP, not optimized
*
* Purpose:
*   Drawing of 16bpp high color bitmaps.
*   Only required for 16bpp color depth of target. Should be removed otherwise.
*/
static void ESP_DrawBitLine16BPP (GUI_DEVICE* pDevice, int x, int y, U16 const* p, int xsize) {
	LCD_SetCursor(x, y);
	*(__IO
	uint16_t * 
	)
	(LCD_CMD) = eLCDPar_wramcmd; //写入颜色值
	for (; xsize > 0; xsize-- , x++ , p++) {
		*(__IO
		uint16_t * 
		)
		(LCD_DATA) = (LCD_PIXELINDEX)*p;
	}
}

/*********************************************************************
*
*       Draw Bitmap 32 BPP, not optimized
*
* Purpose:
*   Drawing of 32bpp true color bitmaps.
*   Only required for 32bpp color depth of target. Should be removed otherwise.
*/
static void ESP_DrawBitLine32BPP (GUI_DEVICE* pDevice, int x, int y, U32 const GUI_UNI_PTR * p, int xsize) {
	for (; xsize > 0; xsize-- , x++ , p++) {
		ESP_SetPixelIndex(pDevice, x, y, *p);
	}
}

/*********************************************************************
*
*       ESP_DrawBitmap
*/
static void ESP_DrawBitmap (GUI_DEVICE* pDevice, int x0, int y0,
                            int xSize, int ySize,
                            int BitsPerPixel,
                            int BytesPerLine,
                            const U8 GUI_UNI_PTR * pData, int Diff,
                            const LCD_PIXELINDEX* pTrans) {
	int i;

	switch (BitsPerPixel) {
		case 1:
			for (i = 0; i < ySize; i++) {
				ESP_DrawBitLine1BPP(pDevice, x0, i + y0, pData, Diff, xSize, pTrans);
				pData += BytesPerLine;
			}
			break;
		case 2:
			for (i = 0; i < ySize; i++) {
				ESP_DrawBitLine2BPP(pDevice, x0, i + y0, pData, Diff, xSize, pTrans);
				pData += BytesPerLine;
			}
			break;
		case 4:
			for (i = 0; i < ySize; i++) {
				ESP_DrawBitLine4BPP(pDevice, x0, i + y0, pData, Diff, xSize, pTrans);
				pData += BytesPerLine;
			}
			break;
		case 8:
			for (i = 0; i < ySize; i++) {
				ESP_DrawBitLine8BPP(pDevice, x0, i + y0, pData, xSize, pTrans);
				pData += BytesPerLine;
			}
			break;
			//
			// Only required for 16bpp color depth of target. Should be removed otherwise.
			//
		case 16:
			for (i = 0; i < ySize; i++) {
				ESP_DrawBitLine16BPP(pDevice, x0, i + y0, (const U16 *)pData, xSize);
				pData += BytesPerLine;
			}
			break;
			//
			// Only required for 32bpp color depth of target. Should be removed otherwise.
			//
		case 32:
			for (i = 0; i < ySize; i++) {
				ESP_DrawBitLine32BPP(pDevice, x0, i + y0, (const U32 *)pData, xSize);
				pData += BytesPerLine;
			}
			break;
	}
}

/*********************************************************************
*
*       ESP_InitOnce
*
* Purpose:
*   Allocates a fixed block for the context of the driver
*
* Return value:
*   0 on success, 1 on error
*/
static int ESP_InitOnce (GUI_DEVICE* pDevice) {
	DRIVER_CONTEXT_TEMPLATE* pContext;

	if (pDevice->u.pContext == NULL) {
		pDevice->u.pContext = GUI_ALLOC_GetFixedBlock(sizeof(DRIVER_CONTEXT_TEMPLATE));
		pContext = (DRIVER_CONTEXT_TEMPLATE *)pDevice->u.pContext;
		pContext->BitsPerPixel = LCD__GetBPP(pDevice->pColorConvAPI->pfGetIndexMask());
	}
	return pDevice->u.pContext ? 0 : 1;
}

/*********************************************************************
*
*       ESP_GetDevProp
*/
static I32 ESP_GetDevProp (GUI_DEVICE* pDevice, int Index) {
	DRIVER_CONTEXT_TEMPLATE* pContext;

	pContext = (DRIVER_CONTEXT_TEMPLATE *)pDevice->u.pContext;
	switch (Index) {
		case LCD_DEVCAP_XSIZE:
			return pContext->xSize;
		case LCD_DEVCAP_YSIZE:
			return pContext->ySize;
		case LCD_DEVCAP_VXSIZE:
			return pContext->vxSize;
		case LCD_DEVCAP_VYSIZE:
			return pContext->vySize;
		case LCD_DEVCAP_BITSPERPIXEL:
			return pContext->BitsPerPixel;
		case LCD_DEVCAP_NUMCOLORS:
			return 0;
		case LCD_DEVCAP_XMAG:
			return 1;
		case LCD_DEVCAP_YMAG:
			return 1;
		case LCD_DEVCAP_MIRROR_X:
			return 0;
		case LCD_DEVCAP_MIRROR_Y:
			return 0;
		case LCD_DEVCAP_SWAP_XY:
			return 0;
	}
	return -1;
}

/*********************************************************************
*
*       ESP_GetDevData
*/
static void* ESP_GetDevData (GUI_DEVICE* pDevice, int Index) {
	GUI_USE_PARA(pDevice);
#if GUI_SUPPORT_MEMDEV 
	switch (Index) {
		case LCD_DEVDATA_MEMDEV:
			return (void *)&GUI_MEMDEV_DEVICE_16; // TBD: Has to be adapted to the right memory device depending on the used color depth!
	}
#else
	GUI_USE_PARA(Index);
#endif
	return NULL ;
}

/*********************************************************************
*
*       ESP_GetRect
*/
static void ESP_GetRect (GUI_DEVICE* pDevice, LCD_RECT* pRect) {
	DRIVER_CONTEXT_TEMPLATE* pContext;

	pContext = (DRIVER_CONTEXT_TEMPLATE *)pDevice->u.pContext;
	pRect->x0 = 0;
	pRect->y0 = 0;
	pRect->x1 = pContext->vxSize - 1;
	pRect->y1 = pContext->vySize - 1;
}

/*********************************************************************
*
*       ESP_SetOrg
*/
static void ESP_SetOrg (GUI_DEVICE* pDevice, int x, int y) {
	LCD_X_SETORG_INFO Data = {0};

	Data.xPos = x;
	Data.yPos = y;
	LCD_X_DisplayDriver(pDevice->LayerIndex, LCD_X_SETORG, (void *)&Data);
}

/*********************************************************************
*
*       Static code: Functions available by ESP_GetDevFunc()
*
**********************************************************************
*/
/*********************************************************************
*
*       ESP_SetVRAMAddr
*/
static void ESP_SetVRAMAddr (GUI_DEVICE* pDevice, void* pVRAM) {
	DRIVER_CONTEXT_TEMPLATE* pContext;
	LCD_X_SETVRAMADDR_INFO Data = {0};

	ESP_InitOnce(pDevice);
	if (pDevice->u.pContext) {
		pContext = (DRIVER_CONTEXT_TEMPLATE *)pDevice->u.pContext;
		pContext->VRAMAddr = (U32)pVRAM;
		Data.pVRAM = pVRAM;
		LCD_X_DisplayDriver(pDevice->LayerIndex, LCD_X_SETVRAMADDR, (void *)&Data);
	}
}

/*********************************************************************
*
*       ESP_SetVSize
*/
static void ESP_SetVSize (GUI_DEVICE* pDevice, int xSize, int ySize) {
	DRIVER_CONTEXT_TEMPLATE* pContext;

	ESP_InitOnce(pDevice);
	if (pDevice->u.pContext) {
		pContext = (DRIVER_CONTEXT_TEMPLATE *)pDevice->u.pContext;
		pContext->vxSize = xSize;
		pContext->vySize = ySize;
		pContext->vxSizePhys = xSize;
	}
}

/*********************************************************************
*
*       ESP_SetSize
*/
static void ESP_SetSize (GUI_DEVICE* pDevice, int xSize, int ySize) {
	DRIVER_CONTEXT_TEMPLATE* pContext;
	LCD_X_SETSIZE_INFO Data = {0};

	ESP_InitOnce(pDevice);
	if (pDevice->u.pContext) {
		pContext = (DRIVER_CONTEXT_TEMPLATE *)pDevice->u.pContext;
		pContext->vxSizePhys = (pContext->vxSizePhys == 0) ? xSize : pContext->vxSizePhys;
		pContext->xSize = xSize;
		pContext->ySize = ySize;
		Data.xSize = xSize;
		Data.ySize = ySize;
		LCD_X_DisplayDriver(pDevice->LayerIndex, LCD_X_SETSIZE, (void *)&Data);
	}
}

/*********************************************************************
*
*       ESP_Init
*/
static int ESP_Init (GUI_DEVICE* pDevice) {
	int r;

	r = ESP_InitOnce(pDevice);
	r |= LCD_X_DisplayDriver(pDevice->LayerIndex, LCD_X_INITCONTROLLER, NULL);
	return r;
}

/*********************************************************************
*
*       ESP_On
*/
static void ESP_On (GUI_DEVICE* pDevice) {
	LCD_X_DisplayDriver(pDevice->LayerIndex, LCD_X_ON, NULL);
}

/*********************************************************************
*
*       ESP_Off
*/
static void ESP_Off (GUI_DEVICE* pDevice) {
	LCD_X_DisplayDriver(pDevice->LayerIndex, LCD_X_OFF, NULL);
}

/*********************************************************************
*
*       ESP_SetLUTEntry
*/
static void ESP_SetLUTEntry (GUI_DEVICE* pDevice, U8 Pos, LCD_COLOR Color) {
	LCD_X_SETLUTENTRY_INFO Data = {0};

	Data.Pos = Pos;
	Data.Color = Color;
	LCD_X_DisplayDriver(pDevice->LayerIndex, LCD_X_SETLUTENTRY, (void *)&Data);
}

/*********************************************************************
*
*       ESP_GetDevFunc
*/
static void (* ESP_GetDevFunc (GUI_DEVICE** ppDevice, int Index)) (void) {
	GUI_USE_PARA(ppDevice);
	switch (Index) {
		case LCD_DEVFUNC_SET_VRAM_ADDR:
			return (void (*) (void))ESP_SetVRAMAddr;
		case LCD_DEVFUNC_SET_VSIZE:
			return (void (*) (void))ESP_SetVSize;
		case LCD_DEVFUNC_SET_SIZE:
			return (void (*) (void))ESP_SetSize;
		case LCD_DEVFUNC_INIT:
			return (void (*) (void))ESP_Init;
		case LCD_DEVFUNC_ON:
			return (void (*) (void))ESP_On;
		case LCD_DEVFUNC_OFF:
			return (void (*) (void))ESP_Off;
		case LCD_DEVFUNC_SETLUTENTRY:
			return (void (*) (void))ESP_SetLUTEntry;
	}
	return NULL ;
}

/*********************************************************************
*
*       Public data
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_DEVICE_API structure
*/
const GUI_DEVICE_API ESP_GUIDRV_Template_API = {
	//
	// Data
	//
	DEVICE_CLASS_DRIVER,
	//
	// Drawing functions
	//
	ESP_DrawBitmap,
	ESP_DrawHLine,
	ESP_DrawVLine,
	ESP_FillRect,
	ESP_GetPixelIndex,
	ESP_SetPixelIndex,
	ESP_XorPixel,
	//
	// Set origin
	//
	ESP_SetOrg,
	//
	// Request information
	//
	ESP_GetDevFunc,
	ESP_GetDevProp,
	ESP_GetDevData,
	ESP_GetRect,
};

/*************************** End of file ****************************/


