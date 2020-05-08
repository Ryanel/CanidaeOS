#pragma once

#ifdef DEBUG

void _assert(const char * expression, const char * file, const char * function, int line);
#define assert(cond) \
                    do { \
                        if(!(cond)) { \
                            _assert(#cond, __FILE__, __FUNCTION__, __LINE__); \
                        } \
                    } while(0)

#else 

#define assert(x) do { (void)sizeof(x); } while(0)  

#endif

