#ifndef _UARTRINGBUF_H_
#define _UARTRINGBUF_H_


typedef struct{
	int     putaddr; 		//put first data index;
	int     getaddr; 		//get first data index;
	int     size;	 		  //buffer size;
	int     num;     		//vaild data len; 
	unsigned char    *data;
}RINGBUFFER;

unsigned char  Ringbuffer_Init(RINGBUFFER *info,unsigned char *buf,int size);
unsigned char  Ringbuffer_PutDataToBuffer(RINGBUFFER *info,unsigned char *buf,int len);
unsigned char  Ringbuffer_GetbufferHeardata(RINGBUFFER *info,unsigned char *buf,int len);
void 		   		 Ringbuffer_AddBufferHearaddr(RINGBUFFER *info,int len);
void  		     Ringbuffer_CleanBuffer(RINGBUFFER *info);
#endif
