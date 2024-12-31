#pragma once

#include <array>
#include <cstdint>
#if !defined(_MSC_VER) || defined(__clang__)
#define UWHOOK_TRAMPOLINE_SUPPORT
#endif


extern void *uwHookCallerRip;

class UWHook {
    friend class UWHookRelease;

    void *fnc;
    void *hook;
    bool useTrampoline;

    static constexpr unsigned original_len = 14;
    std::array<uint8_t, original_len> original;
    bool released;

    bool release();
    bool restore();

public:
    static void noop();

    UWHook(void *fnc, void *hook, bool useTrampoline = false);
    ~UWHook() {release();}
    UWHook(const UWHook&) = delete;
    UWHook(UWHook&&) = delete;

    static void *getTrampolineCaller();

    template<typename fncT>
    fncT *getFunction() {
        return reinterpret_cast<fncT*>(fnc);
    }

    bool isActive() const {
        return !released;
    }
};

class UWHookRelease {
    UWHook& hook;
    bool active;

public:
    UWHookRelease(UWHook &hook);
    ~UWHookRelease();
    UWHookRelease(const UWHookRelease &) = delete;
    UWHookRelease(UWHookRelease &&) = delete;

    bool isValid() const {
        return active;
    }
};


#ifdef UWHOOK_TRAMPOLINE_SUPPORT
#define UWHOOK_TRAMPOLINE(hook) \
    __attribute__((naked)) \
    static void uwHookTrampoline_##hook () { \
        __asm pop r10 \
        __asm mov uwHookCallerRip, r10 \
        __asm jmp hook \
    }
#endif
