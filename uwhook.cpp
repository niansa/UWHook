#include "uwhook.hpp"
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#include <memoryapi.h>
#include <processthreadsapi.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#ifdef __clang__
__attribute__((used))
#endif
void *uwHookCallerRip;

#ifdef _WIN32
static bool write_memory(void *dest, const void *src, size_t len) {
    SIZE_T writ = 0;
    return WriteProcessMemory(GetCurrentProcess(), dest, src, len, &writ) && writ == len;
}
#else
static bool write_memory(void *dest, const void *src, size_t len) {
    static int mem = open("/proc/self/mem", O_WRONLY);

    if (mem < 0)
        return false;

    lseek(mem, reinterpret_cast<off_t>(dest), SEEK_SET);
    if (write(mem, src, len) != len)
        return false;

    __builtin___clear_cache(reinterpret_cast<char *>(dest), reinterpret_cast<char *>(dest) + len);

    return true;
}
#endif

UWHook::UWHook(void *fnc, void *hook, bool useTrampoline) : fnc(fnc), hook(hook), useTrampoline(useTrampoline) {
    memcpy(original.data(), fnc, original_len);
    restore();
}

void *UWHook::getTrampolineCaller() { return reinterpret_cast<uint8_t *>(uwHookCallerRip) - 6; }

bool UWHook::release() {
    const bool success = write_memory(fnc, original.data(), original_len);
    if (success)
        released = true; // Hook is now inactive
    return success;
}

bool UWHook::restore() {
    uint8_t test[] = {0xff, static_cast<uint8_t>(useTrampoline ? 0x15 : 0x25), 0x00, 0x00, 0x00, 0x00, 0xef, 0xbe, 0xad, 0xde, 0xef, 0xbe, 0xad, 0xde};
    static_assert(sizeof(test) == original_len);
    memcpy(test + 6, &hook, 8);

    const bool success = write_memory(fnc, test, sizeof(test));
    if (success)
        released = false; // Hook is now active
    return success;
}

UWHookRelease::UWHookRelease(UWHook& hook) : hook(hook) { active = hook.isActive() && hook.release(); }

UWHookRelease::~UWHookRelease() {
    if (active)
        hook.restore();
}

void UWHook::noop() {}
