#ifndef UW_HOOK_C_H
#define UW_HOOK_C_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Opaque pointer types for C interface
typedef struct UWHook UWHook;
typedef struct UWHookRelease UWHookRelease;

// Forward declarations
extern void *uwHookCallerRip;

// Function pointer types
typedef void (*UWHookNoopFunc)();
typedef void *(*UWHookTrampolineCallerFunc)();

// Create and destroy hooks
UWHook *uw_hook_create(void *fnc, void *hook, bool useTrampoline);
void uw_hook_destroy(UWHook *hook);

// Hook management
bool uw_hook_release(UWHook *hook);
bool uw_hook_restore(UWHook *hook);

// Information
void *uw_hook_get_function(UWHook *hook);
void *uw_hook_get_addr(UWHook *hook);
bool uw_hook_is_active(UWHook *hook);

// Static functions
void uw_hook_noop();
void *uw_hook_get_trampoline_caller();

// Hook release helper
UWHookRelease *uw_hook_release_create(UWHook *hook);
void uw_hook_release_destroy(UWHookRelease *release);
bool uw_hook_release_is_valid(UWHookRelease *release);

// Trampoline support
#ifdef UWHOOK_TRAMPOLINE_SUPPORT
#ifdef _WIN32
#define UWHOOK_TRAMPOLINE_C(hook)                                                                                                                              \
    __declspec(naked) static void uwHookTrampoline_##hook() { __asm pop r10 __asm mov uwHookCallerRip, r10 __asm jmp hook }
#else
#define UWHOOK_TRAMPOLINE_C(hook)                                                                                                                              \
    __attribute__((naked)) static void uwHookTrampoline_##hook() {                                                                                             \
        asm volatile("pop %%r10\n\t"                                                                                                                           \
                     "mov %%r10, %0\n\t"                                                                                                                       \
                     "jmp *%1\n\t"                                                                                                                             \
                     : "=m"(uwHookCallerRip)                                                                                                                   \
                     : "r"(hook)                                                                                                                               \
                     : "r10");                                                                                                                                 \
    }
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif // UW_HOOK_C_H
