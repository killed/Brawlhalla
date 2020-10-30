#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string>


#include "Debug Log.h"

void DebugPrint( const char* FileName, const char* Function, const char* Format, ... )
{
	va_list Arguments;
	va_start( Arguments, Format );

	char UserFormat[ 0x400 ];

	vsnprintf( UserFormat, sizeof( UserFormat ), Format, Arguments );

	auto Unix = time( NULL );

	tm DateTime;

	localtime_s( &DateTime, &Unix );

	std::string FileNameNoSlash = FileName;
	std::string FunctionNameNoLambda = Function;

	auto SlashPos = FileNameNoSlash.rfind( '\\' );

	if ( SlashPos != std::string::npos )
		FileNameNoSlash = FileNameNoSlash.substr( SlashPos + 1 );

	auto LambdaPos = FunctionNameNoLambda.rfind( "lambda" );

	if ( LambdaPos != std::string::npos )
		FunctionNameNoLambda = FunctionNameNoLambda.substr( 0, LambdaPos ) + "lambda>";

	printf( DEBUG_LOG_TIME_COLOR         "%02i:%02i:%02i" ANSI_RESET " | "
		DEBUG_LOG_FILENAME_COLOR     "%-40s"          ANSI_RESET " | "
		DEBUG_LOG_FUNCTIONNAME_COLOR "%-45s"          ANSI_RESET " | "
		"%s\n",
		DateTime.tm_hour, DateTime.tm_min, DateTime.tm_sec,
		FileNameNoSlash.c_str(),
		FunctionNameNoLambda.c_str(),
		UserFormat );

	va_end( Arguments );
}