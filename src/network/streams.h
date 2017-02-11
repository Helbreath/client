/*
* File:   Streams.h
* Author: zero
*
* Created on June 23, 2012, 7:25 AM
*/

#ifndef STREAMS_H
#define	STREAMS_H

#include <iostream>
#include <fstream>

using namespace std;

/**
** Exceptions:
** -191 = string too long
** -192 = data variable invalid
** -193 = Stream read out of bounds
**/

class FileWrite
{
public:
	FileWrite(ofstream & input)
		: file(input)
	{
	};
	~FileWrite()
	{
		file.close();
	}
	ofstream & file;

	void WriteByte(char value)
	{
		file.write(&value, 1);
	};

	void WriteBytes(char * value, int sz)
	{
		file.write(value, sz);
	};

	void WriteShort(int16_t value)
	{
		char sc[2];
		int16_t * sp = (int16_t*)sc;
		*sp = value;
		file.write(sc, 2);
	};

	void WriteInt(int32_t value)
	{
		char ic[4];
		int * ip = (int*)ic;
		*ip = value;
		file.write(ic, 4);
	};

	void WriteInt64(int64_t value)
	{
		char ic[8];
		int64_t * ip = (int64_t*)ic;
		*ip = value;
		file.write(ic, 8);
	};

	void WriteFloat(float value)
	{
		char ic[4];
		memcpy(ic, (char*)&value, 4);
		file.write(ic, 4);
	};

	void WriteString(string value)
	{
		WriteInt(uint32_t(value.length()));
		file.write(value.c_str(), value.length());
	};
};

#define VerifyData if (data == 0) throw -192
#define VerifySize(a) if (position+a > size) { uint64_t oldsize = size; if ((uint64_t)(float(size)*1.15) < oldsize+a) { size = (uint64_t)(float(size)*1.15)+a; } else { size = (uint64_t)(float(size)*1.15); } char * temp = new char[size]; memset(temp, 0, size); memcpy(temp, data, oldsize); delete[] data; data = temp; }
#define VerifyLength(a) if (position+a > size) { throw -193; }
class StreamWrite
{
public:
	StreamWrite(uint32_t initsize = 100)
	{
		size = initsize;
		position = 0;
		data = new char[size];
		memset(data, 0, size);
	};
	~StreamWrite()
	{
		if (data)
			delete[] data;
	}
	char * data;
	uint64_t position;
	uint64_t size;

	void Clear()
	{
		if (data)
			delete[] data;
		size = 100;
		data = new char[size];
		position = 0;
	}

	uint64_t WriteSize()
	{
		VerifyData;
		memcpy(data, &position, 2);
		return position;
	};

	void WriteBytes(const char * value, int sz)
	{
		VerifyData;
		VerifySize(sz);
		memcpy(data + position, value, sz);
		position += sz;
	};

	void WriteArray(const char * value, int sz)
	{
		VerifyData;
		WriteByte(sz);
		VerifySize(sz);
		memcpy(data + position, value, sz);
		position += sz;
	};

	void WriteByte(const char value)
	{
		VerifyData;
		VerifySize(1);
		memcpy(data + position, &value, 1);
		position++;
	};

	void WriteShort(const uint16_t value)
	{
		VerifyData;
		VerifySize(2);
		memcpy(data + position, &value, 2);
		position += 2;
	};

	void WriteInt(const uint32_t value)
	{
		VerifyData;
		VerifySize(4);
		memcpy(data + position, &value, 4);
		position += 4;
	};

	void WriteInt64(const uint64_t value)
	{
		VerifyData;
		VerifySize(8);
		memcpy(data + position, &value, 8);
		position += 8;
	};

	void WriteFloat(const float value)
	{
		VerifyData;
		VerifySize(4);
		memcpy(data + position, &value, 4);
		position += 4;
	};

	void WriteString(const string & value)
	{
		VerifyData;
		if (value.length() >= 255) throw - 191;
		WriteByte(char(value.length()));
		VerifySize(value.length());
		memcpy(data + position, value.c_str(), value.length());
		position += value.length();
	};

	void WriteString(const string & value, uint64_t length)
	{
		VerifyData;
		VerifySize(length);
		if (value.length() > length)
			memcpy(data + position, value.c_str(), length);
		else
			memcpy(data + position, value.c_str(), value.length());
		position += length;
	};
};

class FileRead
{
public:
	FileRead(char * input, uint32_t in)
		: position(0)
		, data(input)
		, size(in)
	{
	};
	~FileRead()
	{
	}

	char * data;
	uint64_t position;
	uint64_t size;

	char ReadByte()
	{
		VerifyData;
		VerifyLength(1);
		char * cp = (char*)(data + position);
		position++;
		return *cp;
	};

	void ReadBytes(char * p, uint16_t sz)
	{
		VerifyData;
		VerifyLength(sz);
		memcpy(p, (char*)(data + position), sz);
		position += sz;
	}

	uint16_t ReadShort()
	{
		VerifyData;
		VerifyLength(2);
		uint16_t * sp = (uint16_t*)(data + position);
		position += 2;
		return *sp;
	};

	int ReadInt()
	{
		VerifyData;
		VerifyLength(4);
		uint32_t * ip = (uint32_t*)(data + position);
		position += 4;
		return *ip;
	};

	int64_t ReadInt64()
	{
		VerifyData;
		VerifyLength(8);
		int64_t * ip = (int64_t*)(data + position);
		position += 8;
		return *ip;
	};

	float ReadFloat()
	{
		VerifyData;
		VerifyLength(4);
		float * fp = (float*)(data + position);
		position += 4;
		return *fp;
	};

	string ReadString()
	{
		uint32_t sz = ReadInt();
		if (sz == 0)
		{
			return "";
		}
		VerifyData;
		VerifyLength(sz);
		char * c = new char[sz + 1];
		memset(c, 0, sz + 1);
		memcpy(c, data + position, sz);
		position += sz;
		string str;
		str = c;
		delete[] c;
		return str;
	};
};
class StreamRead
{
public:
	StreamRead(char * input, uint32_t in)
		: position(0)
		, data(input)
		, size(in)
	{
		//size += 4;
	};
	~StreamRead()
	{
	}

	char * data;
	uint64_t position;
	uint64_t size;


	uint16_t ReadSize()
	{
		VerifyData;
		return *(uint16_t*)(data);
	};

	char * ReadArray()
	{
		VerifyData;
		int sz = ReadInt();
		VerifyLength(sz);
		char * temp = new char[sz];
		memcpy(temp, (char*)(data + position), sz);
		position += sz;
		return temp;
	};

	void ReadBytes(char * p, uint16_t sz)
	{
		VerifyData;
		VerifyLength(sz);
		memcpy(p, (char*)(data + position), sz);
		position += sz;
	}

	char ReadByte()
	{
		VerifyData;
		VerifyLength(1);
		char cp = (char)*(data + position);
		position++;
		return cp;
	};

	uint16_t ReadShort()
	{
		VerifyData;
		VerifyLength(2);
		uint16_t * sp = (uint16_t*)(data + position);
		position += 2;
		return *sp;
	};

	int ReadInt()
	{
		VerifyData;
		VerifyLength(4);
		uint32_t * ip = (uint32_t*)(data + position);
		position += 4;
		return *ip;
	};

	int64_t ReadInt64()
	{
		VerifyData;
		VerifyLength(8);
		int64_t * ip = (int64_t*)(data + position);
		position += 8;
		return *ip;
	};

	float ReadFloat()
	{
		VerifyData;
		VerifyLength(4);
		float * fp = (float*)(data + position);
		position += 4;
		return *fp;
	};

	string ReadString()
	{
		uint32_t sz = ReadByte();
		if (sz == 0)
		{
			return "";
		}
		VerifyData;
		VerifyLength(sz);
		char * c = new char[sz + 1];
		memset(c, 0, sz + 1);
		memcpy(c, data + position, sz);
		position += sz;
		string str;
		str = c;
		delete[] c;
		return str;
	};
	string ReadString(uint32_t sz)
	{
		VerifyData;
		VerifyLength(sz);
		char * c = new char[sz + 1];
		memset(c, 0, sz + 1);
		memcpy(c, data + position, sz);
		position += sz;
		string str;
		str = c;
		delete[] c;
		return str;
	};
};

#endif	/* STREAMS_H */

