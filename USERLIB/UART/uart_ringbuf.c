#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task_config.h"
#include "uart_ringbuf.h"

static 	 OS_EVENT * g_mutex_read;
#define  lock     OSMutexPend(g_mutex_read,0,&err)
#define  unlock   OSMutexPost(g_mutex_read)


unsigned char  Ringbuffer_Init(RINGBUFFER *info,unsigned char *buf,int size)
{
	INT8U err;
	if(buf==NULL){
		return 0;
	}
	info->data=buf;
	memset(info->data,0,size);
	info->getaddr=0;
	info->putaddr=0;
	info->num=0;
	info->size=size;
	g_mutex_read=OSMutexCreate(UART_MUTEX_PRIO,&err);
	printf("%s size =%d  %d \r\n",__func__,size,info->size);
	return 1;
}

unsigned char  Ringbuffer_PutDataToBuffer(RINGBUFFER *info,unsigned char *buf,int len)
{
	INT8U err;
	if(info==NULL){
		printf(" %s  info is NULL\r\n",__func__);
		return 0;
	}
	if(info->data==NULL||info->size<1){
		printf(" %s  data is NULL\r\n",__func__);
		return 0;
	}
	if(len<=0){
		printf(" %s  len is wrong \r\n",__func__);
		return 0;
	}


	lock;
	if(len>info->size - info->num){
		printf(" %s  len is big \r\n",__func__);
		len=info->size - info->num;
	}
	unlock;

	if(info->putaddr+len>info->size){
		memcpy(&info->data[info->putaddr],buf,info->size-info->putaddr);
		memcpy(&info->data[0],&buf[info->size-info->putaddr],info->putaddr+len-info->size);
	}else{
		memcpy(&info->data[info->putaddr],buf,len);
	}
	lock;
	info->num	  +=len;
	info->putaddr=(info->putaddr+len);
	if(info->putaddr>=info->size){
		 info->putaddr-=info->size;
	}	
	unlock;	
	//printf(" %s  num  %d  putaddr %d  getaddr %d\r\n",__func__,info->num,info->putaddr,info->getaddr);
	return 1;
}

unsigned char  Ringbuffer_GetbufferHeardata(RINGBUFFER *info,unsigned char *buf,int len)
{
	INT8U err;
	if(info==NULL){
		printf(" %s  info is NULL\r\n",__func__);
		return 0;
	}
	if(info->data==NULL){
		printf(" %s  info buf is NULL\r\n",__func__);
		return 0;
	}
	lock;
	if(len>info->num){
		len=info->num;
	}
	unlock;

	if(info->getaddr+len>=info->size){
		memcpy(buf,&info->data[info->getaddr],info->size-info->getaddr);
		memcpy(&buf[info->size-info->getaddr],&info->data[0],info->getaddr+len-info->size);
	}else{
		memcpy(buf,&info->data[info->getaddr],len);
	}
	//printf(" %s  num  %d  putaddr %d  getaddr %d\r\n",__func__,info->num,info->putaddr,info->getaddr);
	return len;
}
void Ringbuffer_AddBufferHearaddr(RINGBUFFER *info,int len)
{
	INT8U err;

	lock;
	info->num-=len;
	info->getaddr=(info->getaddr+len); 
	if(info->getaddr>=info->size){
		 info->getaddr-=info->size;
	}
	unlock;
}

void Ringbuffer_CleanBuffer(RINGBUFFER *info)
{
	info->getaddr=0;
	info->putaddr=0;
	info->num=0;	
}

