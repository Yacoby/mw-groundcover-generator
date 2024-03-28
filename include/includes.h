#ifndef __INCLUDES_H_
#define __INCLUDES_H_


#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <list>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#endif

#include <boost/filesystem.hpp>

#include <ctime>
#include <cstdlib>

#include "ESBase.h"

extern uint32_t gNumRecords;
extern long gNumRecordPos;

#endif
