

#ifndef _CANCONFIG_H_
#define _CANCONFIG_H_

typedef enum{
	BandRate_10kbps,
	BandRate_125kbps,
	BandRate_250kbps,
	BandRate_500kbps,
	BandRate_1Mbps
}CanBandRate;


typedef struct{
    int   ID;
    int   BYTENUM;
    int   BITPOS;
    int   DATALEN;
    float DATACOEF;
}CANELE;

#endif