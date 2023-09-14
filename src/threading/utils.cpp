#include "utils.h"

#if defined(PLATFORM_WIN)

#pragma warning( push, 0 )
#include <windows.h>
#pragma warning( pop )

#elif defined(PLATFORM_MAC)

#include <pthread.h>

#endif

namespace threading
{
    void set_current_thread_name(const std::string& name)
    {
#if defined(PLATFORM_WIN)
        if (GetProcAddress(GetModuleHandle(L"kernel32.dll"), "SetThreadDescription"))
        {
            const std::wstring w_name = std::wstring(name.begin(), name.end());
            SetThreadDescription(GetCurrentThread(), w_name.c_str());
        }
#elif defined(PLATFORM_MAC)
        pthread_setname_np(name.c_str());
#endif
    }
}