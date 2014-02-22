#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#define NULL 0

typedef unsigned long long uint64;
typedef signed long long int64;
typedef unsigned long uint32;
typedef signed long int32;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned char uint8;
typedef signed char int8;

#include <string>
 //typedef std::string string;
 typedef std::stringstream sstream;
 
 typedef std::ostream ostream;

#include <list>
#ifdef __MINGW32__
#include <tr1\unordered_map>
#define HashMap std::tr1::unordered_map
#else
#include <unordered_map>
#define HashMap std::unordered_map
#endif
#include <map>
#define Map std::map
#include <vector>
#define Vector std::vector
#define AutoPtr std::auto_ptr

typedef uint16 ItemID;
typedef uint64 ItemUID;
typedef uint32 NpcID;

//-------------------------------------------MACROS----------------------------------------------------------
#define foreach(x,y)		for(decltype(( y ).begin()) x = ( y ).begin(); x != ( y ).end(); ++x)
#define foreach_a(x,y)	for(decltype(&( y )[0]) x = &( y )[0]; x < &( y )[0] + (sizeof( y )/sizeof(( y )[0])); x++)

//--------------------------------------------TIME-----------------------------------------------------------

struct Date
{
	uint16 year;
	uint8 month;
	uint8 day;
};

//#define SECONDS(x)			(x * CLOCKS_PER_SEC)
//#define MINUTES(x)			(x * SECONDS(60))
//#define HOURS(x)			(x * MINUTES(60))
//#define DAYS(x)				(x * HOURS(24))

// Time
#define _ms					_s / 1000
#define _s					* CLOCKS_PER_SEC
#define _m					_s * 60
#define _h					_m * 60
#define _d					_h * 24

// time_t
#define MINUTE(x)		x * 60
#define HOUR(x)	 	x * MINUTE(60)
#define DAY(x)		 	x * HOUR(24)

enum DaysOfWeek
{
	SUNDAY,
	MONDAY,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY
};

#endif // TYPEDEFS_H
