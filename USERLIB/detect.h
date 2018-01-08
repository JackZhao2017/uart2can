#ifndef _DETECT_H_
#define _DETECT_H_
#include <stdint.h>
#include "ringbuf.h"

char detectvaildSync(RINGBUFFER *info,unsigned char sync);
char detectvaildMsgtype(RINGBUFFER *info,unsigned char *type);
char detectcrc8(unsigned char *msg,unsigned char  start, unsigned char  len);
unsigned char  addringaddr(RINGBUFFER *info,int inc);
#endif
