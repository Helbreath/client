#include "buffer.h"

void Push(char * &cp, uint8 value){
	uint8 * p = (uint8 *)cp;
	*p = (uint8)value;
	cp += sizeof(uint8);
}
void Push(char * &cp, uint16 value){
	uint16 * p = (uint16 *)cp;
	*p = (uint16)value;
	cp += sizeof(uint16);
}
void Push(char * &cp, uint32 value){
	uint32 * p = (uint32 *)cp;
	*p = (uint32)value;
	cp += sizeof(uint32);
}
void Push(char * &cp, uint64 value){
	uint64 * p = (uint64 *)cp;
	*p = value;
	cp += sizeof(uint64);
}
void Push(char * &dest, const char * src, uint32 len){
	memcpy(dest, src, len);
	dest += len;
}
void Push(char * &dest, const char * src){
	strcpy(dest, src);
	dest += strlen(src) + 1;
}
void Push(char * &dest, const std::string & str){
	strcpy(dest, str.c_str());
	dest += str.length() + 1;
}

void Pop(char * &cp, uint8 &v){
	uint8 * p = (uint8 *)cp;
	v = *p;
	cp += sizeof(uint8);
}
void Pop(char * &cp, uint16 &v){
	uint16 * p = (uint16 *)cp;
	v = *p;
	cp += sizeof(uint16);
}
void Pop(char * &cp, uint32 &v){
	uint32 * p = (uint32 *)cp;
	v = *p;
	cp += sizeof(uint32);
}
void Pop(char * &cp, uint64 &v){
	uint64 * p = (uint64 *)cp;
	v = *p;
	cp += sizeof(uint64);
}
void Pop(char * &src, char * dest, uint32 len){
	memcpy(dest, src, len);
	src += len;
}
void Pop(char * &src, char * dest){
	uint32 len = strlen(src);
	memcpy(dest, src, len);
	src += len + 1;
}
void Pop(char * &src, std::string & str){
	str = src;
	src += str.length() + 1;
}