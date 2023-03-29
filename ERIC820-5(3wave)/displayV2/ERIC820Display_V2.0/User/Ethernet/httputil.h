#ifndef	__HTTPUTIL_H__
#define	__HTTPUTIL_H__

#include <stdio.h>
#include <string.h>
#include "w5500_config.h"
#include "w5500.h"
#include "utility.h"
#include "http_server.h"
#include "socket.h"


void proc_http(SOCKET s, u_char * buf);
void Https_Task(SOCKET s);
void cgi_ipconfig(st_http_request *http_request);
uint16 make_msg_response(uint8* buf,int8* msg);

void make_cgi_response(uint16 a,int8* b,int8* c);
void make_pwd_response(int8 isRight,uint16 delay,int8* cgi_response_content, int8 isTimeout);
#endif


