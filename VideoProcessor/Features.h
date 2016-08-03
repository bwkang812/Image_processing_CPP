#pragma once
#include "MyImage.h"

#define M_PI		3.14159265358979323846

void DrawCross(CByteImage& image, int x, int y, BYTE color[3]);
void DrawLine(CByteImage& canvas, int x1, int y1, int x2, int y2, BYTE val);
void DrawLine(CByteImage& canvas, int x1, int y1, int x2, int y2, BYTE R, BYTE G, BYTE B);
void SobelEdge(const CByteImage& imageIn, CByteImage& imageOut);
void CannyEdge(const CByteImage& imageIn, CByteImage& imageOut, int nThresholdHi, int nThresholdLo);


int  HoughLines(const CByteImage& imageIn, int nTNum, int nTVal, double resTheta, int numLine, double* pRho, double* pTheta);
int  HarrisCorner(const CByteImage& imageIn,  double dThreshold, double paramK, int nMaxCorner, double* posX, double* posY);
