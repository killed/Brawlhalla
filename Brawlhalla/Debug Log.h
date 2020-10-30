#pragma once

#include <Windows.h>
#include <string>

#define NOMINMAX
#include <Windows.h>
#define MACRO_STRINGIFY(X) #X
#define MACRO_TO_STRING(X) MACRO_STRINGIFY(X)
#define COMPILETIME constexpr

#define IDA_BASE 0x00007FF61B940000i64
#define ADDR_TO_IDA(Address, Base) ( ( (UINT_PTR)Address - (UINT_PTR)Base ) + IDA_BASE )

#define ANSI_FOREGROUND_256(ColorCode)  "\033[38;5;" MACRO_TO_STRING(ColorCode) "m"
#define ANSI_RESET                      "\033[0m"
#define ANSI_BOLD                       "\033[1m" 
#define ANSI_ITALIC                     "\033[3m" 
#define ANSI_UNDERLINE                  "\033[4m" 

#define ANSI_BLACK                      ANSI_FOREGROUND_256( 0 )     
#define ANSI_RED                        ANSI_FOREGROUND_256( 1 )     
#define ANSI_GREEN                      ANSI_FOREGROUND_256( 2 )     
#define ANSI_YELLOW                     ANSI_FOREGROUND_256( 3 )     
#define ANSI_BLUE                       ANSI_FOREGROUND_256( 4 )     
#define ANSI_MAGENTA                    ANSI_FOREGROUND_256( 5 )     
#define ANSI_CYAN                       ANSI_FOREGROUND_256( 6 )     
#define ANSI_WHITE                      ANSI_FOREGROUND_256( 7 )     
#define ANSI_LIGHTGREY                  ANSI_FOREGROUND_256( 8 ) 
#define ANSI_BRIGHTRED                  ANSI_FOREGROUND_256( 9 ) 

#define DEBUG_LOG_CONSOLE_TITLE         "Endeavour V4 Debug Console"
//#define DEBUG_LOG_DISABLE_COLOR

#ifdef DEBUG_LOG_DISABLE_COLOR

#define DEBUG_LOG_TIME_COLOR     
#define DEBUG_LOG_FILENAME_COLOR
#define DEBUG_LOG_FILENAME_LINE_COLOR
#define DEBUG_LOG_FUNCTIONNAME_COLOR
#define DEBUG_LOG_SUCCESS_COLOR
#define DEBUG_LOG_INFO_COLOR
#define DEBUG_LOG_ERROR_COLOR
#define DEBUG_LOG_WARNING_COLOR

#else

#define DEBUG_LOG_TIME_COLOR            ANSI_FOREGROUND_256( 220 )
#define DEBUG_LOG_FILENAME_COLOR        ANSI_FOREGROUND_256( 209 )
#define DEBUG_LOG_FILENAME_LINE_COLOR   ANSI_FOREGROUND_256( 6 )
#define DEBUG_LOG_FUNCTIONNAME_COLOR    
#define DEBUG_LOG_SUCCESS_COLOR         ANSI_GREEN
#define DEBUG_LOG_INFO_COLOR            ANSI_CYAN
#define DEBUG_LOG_ERROR_COLOR           ANSI_RED
#define DEBUG_LOG_WARNING_COLOR         ANSI_YELLOW

#endif

#if defined(_DEBUG) || defined(_DEBUG_OPTIMISED)

void DebugPrint( const char* FileName, const char* Function, const char* Format, ... );

#define DEBUG_STR(X) X
#define LOG(...) DebugPrint( __FILE__ ANSI_RESET ":" DEBUG_LOG_FILENAME_LINE_COLOR MACRO_TO_STRING( __LINE__ ) ANSI_RESET, __FUNCTION__, __VA_ARGS__)
#define CREATE_CONSOLE() \
	[]( ) { \
		if( !AttachConsole( GetCurrentProcessId() ) )  \
		{ \
			AllocConsole(); \
		} \
		FILE* StdOutDescriptor = nullptr; \
		freopen_s(&StdOutDescriptor, "CONOUT$", "w", stdout); \
		SetConsoleTitleA( DEBUG_LOG_CONSOLE_TITLE ); \
		DWORD CurrentMode; \
		GetConsoleMode( GetStdHandle( STD_OUTPUT_HANDLE ), &CurrentMode ); \
		SetConsoleMode( GetStdHandle(STD_OUTPUT_HANDLE), CurrentMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING ); \
	}( )

#else

#define DEBUG_STR(X) ""
#define LOG(...)
#define CREATE_CONSOLE()

#endif

#define LOG_SUCCESS(...) LOG("[" DEBUG_LOG_SUCCESS_COLOR  "SUCESS"  ANSI_RESET "] " __VA_ARGS__)
#define LOG_INFO(...)    LOG("[" DEBUG_LOG_INFO_COLOR     "INFO"    ANSI_RESET "] " __VA_ARGS__)
#define LOG_ERROR(...)   LOG("[" DEBUG_LOG_ERROR_COLOR    "ERROR"   ANSI_RESET "] " __VA_ARGS__)
#define LOG_WARNING(...) LOG("[" DEBUG_LOG_WARNING_COLOR  "WARNING" ANSI_RESET "] " __VA_ARGS__)