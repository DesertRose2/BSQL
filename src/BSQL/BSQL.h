﻿#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#define BYOND_FUNC __declspec(dllexport) const char* _cdecl
#else
#define BYOND_FUNC __attribute__((visibility("default"))) const char*
#endif

#include <mysql.h>

#include <atomic>
#include <chrono>
#include <deque>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <stack>
#include <string>
#include <thread>

class Library;

#include "Operation.h"
#include "Query.h"
#include "Connection.h"

#include "MySqlConnection.h"
#include "MySqlConnectOperation.h"
#include "MySqlQueryOperation.h"

#include "Library.h"
