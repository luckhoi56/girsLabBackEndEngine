
#include "redcas_util_imgexport_ImageToClipBoardCopier.h"
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_redcas_util_imgexport_ImageToClipBoardCopier_copyToClipBoardN
  (JNIEnv * jni, jclass t, jint w, jint h, jintArray bytes ) 
{ 
	BOOL bOk  = false; 
	HWND hWnd = GetDesktopWindow(); 
	HDC  dtdc = GetDC(hWnd);
	HDC  dc   = CreateCompatibleDC(dtdc);
	HBITMAP hBmp = CreateCompatibleBitmap( dtdc,w,h );
	SelectObject(dc,hBmp);


	if ( hBmp ) 
	{ 

		jint  len = jni->GetArrayLength( bytes ); 
		jint * bits = jni->GetIntArrayElements( bytes, NULL);
		for (int j = 0 ; j < h ; j++) {
			for (int i = 0 ; i < w ; i++) {
				int x = i;
				int y = j;
				int pixel = bits[j * w + i];
				BYTE red   = (pixel >> 16) & 0xff;
				BYTE green = (pixel >>  8) & 0xff;
				BYTE blue  = (pixel      ) & 0xff;

				COLORREF col = RGB(red,green,blue);

				SetPixelV(dc,x,y,col);
			}
		}
		jni->ReleaseIntArrayElements(bytes,bits,0);
		bOk = true; 
	} 

	if ( bOk ) 
	{ 
		bOk = false; 
		if (OpenClipboard(NULL) ) 
		{ 
			EmptyClipboard(); 

			if ( SetClipboardData( CF_BITMAP,hBmp ) ) 
			{ 
				bOk= true; 
			} 
			CloseClipboard(); 
		} 
	} 

	DeleteDC(dc);
	ReleaseDC( hWnd,dtdc );
	dtdc = NULL;
}



#ifdef __cplusplus
}
#endif
