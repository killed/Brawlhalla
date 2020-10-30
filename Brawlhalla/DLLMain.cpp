#include <winsock2.h>
#include <Windows.h>
#include "Adobe AIRExports.h"
#include "Debug Log.h"
#include "../Dependencies/minhook/include/MinHook.h"
#pragma comment(lib, "ws2_32.lib")
void* SendStub;
void* RecvFromStub;

void PrintBytes( const BYTE* Bytes, SIZE_T Count, const char* Delimiter = " ", int Rows = 16 )
{
	for ( int i = 0; i < Count; ++i )
	{
		if ( Rows > 0 )
		{
			if ( i % Rows == 0 && i != 0 )
			{
				printf( "\n" );
			}
		}

		printf( "%02X", Bytes[ i ] );

		if ( Delimiter )
		{
			printf( "%s", Delimiter );
		}
	}

	printf( "\n" );
}

#include <intrin.h>

int WSAAPI RecvFromHook(
	SOCKET s,
	char * buf,
	 int len,
	int flags,
	struct sockaddr * from,
	int * fromlen
)
{
	auto Return = _ReturnAddress();

	auto _Original = (decltype( &RecvFromHook ))RecvFromStub;
	
	auto OriginalReturn = _Original( s, buf, len, flags, from, fromlen );

	if ( OriginalReturn > 1 )
	{
		LOG_INFO( "RecvFrom: Ret: %08X Port: %i Size: %i, Flags: %i Payload:", ntohs( (( sockaddr_in*)from)->sin_port ), Return, OriginalReturn, flags );

		PrintBytes( (BYTE*)buf, OriginalReturn );
	}

	return OriginalReturn;
}

int WSAAPI SendHook(
	SOCKET     s,
	const char* buf,
	int        len,
	int        flags
)
{
	auto _Original = (decltype( &SendHook ))SendStub;
	auto Return = _ReturnAddress();

	LOG_INFO( "Send: Ret: %08X Size: %i, Flags: %i Payload:", Return, len, flags );

	PrintBytes( (BYTE*)buf, len );

	return _Original( s, buf, len, flags );
}


DWORD WINAPI ProcessAttach(
	_In_ LPVOID Parameter
)
{
	if ( Parameter == NULL )
		return FALSE;
	
	CREATE_CONSOLE();

	LOG_INFO( "Loaded mofucker" );

	LOG_INFO( "Water Loaded: %08X", LoadLibraryW( L"C:\\Program Files (x86)\\Steam\\steamapps\\common\\Brawlhalla\\Adobe AIR\\Versions\\1.0\\AdobeWATER.dll" ) );

	CreateThread( NULL, 0, []( PVOID ) -> DWORD { 

		auto WS2_32Handle = GetModuleHandle( L"WS2_32.dll" );

		LOG_INFO( "WS2_32.DLL -> %08X", WS2_32Handle );

		auto SendAddr     = GetProcAddress( WS2_32Handle, "send" );
		auto RecvFromAddr = GetProcAddress( WS2_32Handle, "recvfrom" );

		LOG_INFO( "WS2_32:send -> %08X", WS2_32Handle );

		auto Status = MH_Initialize();

		if ( Status != MH_OK )
		{
			LOG_ERROR( "Failed To Init MinHook %i", Status, Status );
		}

		Status = MH_CreateHook( SendAddr, SendHook, &SendStub );

		if ( Status != MH_OK )
		{
			LOG_ERROR( "Failed To Create Hook: %i", Status );
			return false;
		}

		Status = MH_EnableHook( SendAddr );

		if ( Status != MH_OK )
		{
			LOG_ERROR( "Failed To Enable Hook: %i", Status );
			return false;
		}

		LOG_SUCCESS( "Attached Hook For Source: 0x%p", SendAddr );

		Status = MH_CreateHook( RecvFromAddr, RecvFromHook, &RecvFromStub );

		if ( Status != MH_OK )
		{
			LOG_ERROR( "Failed To Create Hook: %i", Status );
			return false;
		}

		Status = MH_EnableHook( RecvFromAddr );

		if ( Status != MH_OK )
		{
			LOG_ERROR( "Failed To Enable Hook: %i", Status );
			return false;
		}

		LOG_SUCCESS( "Attached Hook For Source: 0x%p", RecvFromAddr );
		//WS2_32.dll

		return 0;
	}, NULL, 0, NULL );

	return TRUE;
}
DWORD WINAPI ProcessDetach(
	_In_ LPVOID Parameter
)
{
	if ( Parameter == NULL )
		return FALSE;

	LOG_INFO( "Unloaded mofucker" );

	MH_DisableHook( MH_ALL_HOOKS );
	MH_Uninitialize();

	return TRUE;
}
BOOL APIENTRY DllMain( 
	_In_ HINSTANCE Instance,
	_In_ DWORD     Reason,
	_In_ LPVOID    Reserved 
)
{
	switch ( Reason )
	{
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls( Instance ); // Disable DLL_THREAD_ATTACH and DLL_THREAD_DETACH notifications
			return ProcessAttach( Instance );
		case DLL_PROCESS_DETACH:
			return ProcessDetach( Instance );
	}

	return TRUE;
}
