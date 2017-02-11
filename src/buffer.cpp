
#include "buffer.h"

void Push(char * &cp, uint8_t value){
	uint8_t * p = (uint8_t *)cp;
	*p = (uint8_t)value;
	cp += sizeof(uint8_t);
}
void Push(char * &cp, uint16_t value){
	uint16_t * p = (uint16_t *)cp;
	*p = (uint16_t)value;
	cp += sizeof(uint16_t);
}
void Push(char * &cp, uint32_t value){
	uint32_t * p = (uint32_t *)cp;
	*p = (uint32_t)value;
	cp += sizeof(uint32_t);
}
void Push(char * &cp, uint64_t value){
	uint64_t * p = (uint64_t *)cp;
	*p = value;
	cp += sizeof(uint64_t);
}
void Push(char * &dest, const char * src, uint32_t len){
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

void Pop(char * &cp, uint8_t &v){
	uint8_t * p = (uint8_t *)cp;
	v = *p;
	cp += sizeof(uint8_t);
}
void Pop(char * &cp, uint16_t &v){
	uint16_t * p = (uint16_t *)cp;
	v = *p;
	cp += sizeof(uint16_t);
}
void Pop(char * &cp, uint32_t &v){
	uint32_t * p = (uint32_t *)cp;
	v = *p;
	cp += sizeof(uint32_t);
}
void Pop(char * &cp, uint64_t &v){
	uint64_t * p = (uint64_t *)cp;
	v = *p;
	cp += sizeof(uint64_t);
}
void Pop(char * &src, char * dest, uint32_t len){
	memcpy(dest, src, len);
	src += len;
}
void Pop(char * &src, char * dest){
	uint32_t len = strlen(src);
	memcpy(dest, src, len);
	src += len + 1;
}
void Pop(char * &src, std::string & str){
	str = src;
	src += str.length() + 1;
}