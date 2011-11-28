#ifndef RENDER_H
#define RENDER_H


#ifdef __cplusplus
 extern "C" {
#endif


void RenderGDIBitmap(unsigned char* x0, unsigned int y0,
		const unsigned char* src, unsigned int width, unsigned int rows);

void DrawHLine(unsigned int x0, unsigned int x1, unsigned int y, unsigned char* buf);
void DrawVLine(unsigned int y0, unsigned int y1, unsigned int x, unsigned char* buf);
void InvertVLine(unsigned int y0, unsigned int y1, unsigned int x, unsigned char* buf);
void InvertHLine(unsigned int x0, unsigned int x1, unsigned int y, unsigned char* buf);

#ifdef __cplusplus
}
#endif

#endif
