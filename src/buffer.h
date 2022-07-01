#ifndef BUFFER_H
#define BUFFER_H
#pragma once

#include	"typedefs.h"

void Push(char * &cp, uint8_t value);
void Push(char * &cp, uint16_t value);
void Push(char * &cp, uint32_t value);
void Push(char * &cp, uint64_t value);
void Push(char * &dest, const char * src, uint32_t len);
void Push(char * &dest, const char * src);
void Push(char * &dest, const std::string & src);

void Pop(char * &cp, uint8_t &v);
void Pop(char * &cp, uint16_t &v);
void Pop(char * &cp, uint32_t &v);
void Pop(char * &cp, uint64_t &v);
void Pop(char * &src, char * dest, uint32_t len);
void Pop(char * &src, char * dest);
void Pop(char * &src, std::string & str);

#endif //BUFFER_H