#define UWHOOK_C_API

#include "uwhook_c.h"
#include "uwhook.hpp"

extern "C" {
UWHook *uw_hook_create(void *fnc, void *hook, bool useTrampoline) { return new UWHook(fnc, hook, useTrampoline); }

void uw_hook_destroy(UWHook *hook) { delete hook; }

bool uw_hook_release(UWHook *hook) { return hook->release(); }

bool uw_hook_restore(UWHook *hook) { return hook->restore(); }

void *uw_hook_get_function(UWHook *hook) { return hook->getFunction<void>(); }

void *uw_hook_get_addr(UWHook *hook) { return hook->getAddr(); }

bool uw_hook_is_active(UWHook *hook) { return hook->isActive(); }

void uw_hook_noop() { UWHook::noop(); }

void *uw_hook_get_trampoline_caller() { return UWHook::getTrampolineCaller(); }

UWHookRelease *uw_hook_release_create(UWHook *hook) { return new UWHookRelease(*hook); }

void uw_hook_release_destroy(UWHookRelease *release) { delete release; }

bool uw_hook_release_is_valid(UWHookRelease *release) { return release->isValid(); }
}
