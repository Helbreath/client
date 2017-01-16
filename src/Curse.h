//
// Copyright (c) 2020 Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

class CCurse  
{
public:
    CCurse();
    virtual ~CCurse();
    void LoadCurse(const char *filename);
    bool IsCurse(const char *str);
    char *ConvertString(char *str, int max_len);
    void filterChar(const char *src, char *dst, const char *chars);
    char* getField(const char *buf, int delim, char *field);
};

