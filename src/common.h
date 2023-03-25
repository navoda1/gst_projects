#ifndef __COMMON_H__
#define __COMMON_H__

#include <chrono>

typedef enum {
	success = 0,
	error = 1
} status_t;

typedef std::chrono::system_clock::time_point time_point_t;
typedef std::chrono::minutes minutes_t;
typedef std::chrono::seconds seconds_t;
typedef std::chrono::duration<uint8_t> duration_minutes_t;
typedef std::chrono::duration<uint8_t> duration_seconds_t;

#endif /* __COMMON_H__ */