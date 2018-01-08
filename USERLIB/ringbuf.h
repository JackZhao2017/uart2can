#ifndef _RINGBUF_H_
#define _RINGBUF_H_


typedef struct{
	unsigned char    putaddr; 		//put first data index;
	unsigned char    getaddr; 		//get first data index;
	unsigned char    size;	 		//buffer size;
	unsigned char    num;     		//vaild data len; 
	unsigned char    putnum;
	unsigned char    getnum;
	unsigned char    *data;
}RINGBUFFER;

unsigned char  ringbufferInit(RINGBUFFER *info,unsigned char *buf,int size);
unsigned char  putdatatoBuffer(RINGBUFFER *info,unsigned char *buf,int len);
unsigned char  getdatafromBuffer(RINGBUFFER *info,unsigned char *buf,int len);
unsigned char  flush(RINGBUFFER *dest,RINGBUFFER *src);
#endif
