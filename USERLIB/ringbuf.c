#include <stdio.h>
#include <stdlib.h>
#include "ringbuf.h"
#include <string.h>

unsigned char  ringbufferInit(RINGBUFFER *info,unsigned char *buf,int size)
{
	if(buf==NULL){
		return 0;
	}
	info->data=buf;
	memset(info->data,0,size);
	info->getaddr=0;
	info->putaddr=0;
	info->getnum=0;
	info->putnum=0;
	info->num=0;
	info->size=size;
	printf("%s size =%d \r\n",__func__,info->size );
	return 1;
}

unsigned char  putdatatoBuffer(RINGBUFFER *info,unsigned char *buf,int len)
{
	if(len<=0){
		printf("putdatatoBuffer failed \r\n");
		return 0;
	}
	if(len>info->size-info->num){
		len=info->size-info->num;
		printf("putdatatoBuffer not enough\r\n");
	}
	if(len<=info->size-info->putaddr){
	   memcpy(info->data+info->putaddr,buf,len);
	   info->putaddr=(info->putaddr+len)%info->size;
	}else{
	   memcpy(info->data+info->putaddr,buf,info->size-info->putaddr);
	   memcpy(info->data,buf+info->size-info->putaddr,len-(info->size-info->putaddr));
	   info->putaddr=len-(info->size-info->putaddr);
	}
	//info->num+=len;
	info->putnum+=len;
	return 1;
}

unsigned char  flush(RINGBUFFER *dest,RINGBUFFER *src)
{
	dest->getnum=0;
	dest->putnum=0;
	if(src->putnum>0){
		dest->num+=src->putnum;
		dest->putaddr=src->putaddr;
	}else if(src->getnum>0){
		dest->num-=src->getnum;
		dest->getaddr=src->getaddr;
	}else{
		return 0;
	}
	return 1;
}

unsigned char  getdatafromBuffer(RINGBUFFER *info,unsigned char *buf,int len)
{
	if(len>info->num||len<=0){
		printf("getdatafromBuffer failed\r\n");
		return 0;
	}
	if(len>=info->size-info->getaddr){
		memcpy(buf,info->data+info->getaddr,info->size-info->getaddr);
		memcpy(buf+info->size-info->getaddr,info->data,len-(info->size-info->getaddr));
		info->getaddr=(info->getaddr+len)%info->size;
	}else{
		memcpy(buf,info->data+info->getaddr,len);
		info->getaddr+=len;
	}
	//info->num-=len;
	info->getnum+=len;	
	return 1;
}

