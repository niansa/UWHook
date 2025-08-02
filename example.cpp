#include <optional>
#include <cstdio>
#include <uwhook.hpp>

#ifdef _WIN32
#define noinline __declspec(noinline)
#else
#define noinline __attribute__((noinline))
#endif

noinline // Will not hook inlined functions
    void
    actual_test0(int a) {
    printf("actual_test0(%d) called\n", a);
    fflush(stdout);
}
noinline void actual_test1(int a) {
    printf("actual_test1(%d) called\n", a);
    fflush(stdout);
}

std::optional<UWHook> hook0, hook1;

void test_hook0(int a) {
    printf("test_hook0(%d) called\n", a);
    fflush(stdout);
}
void test_hook1(int a) {
    printf("test_hook1(%d) called\n", a);
    fflush(stdout);
    UWHookRelease HR(hook0.value());
    auto actual_fnc = hook0->getFunction<void(int)>();
    actual_fnc(a * 2);
}
#ifdef UWHOOK_TRAMPOLINE_SUPPORT
void test_hook2(int a) {
    printf("test_hook2(%d) called instead of function at 0x%p\n", a, UWHook::getTrampolineCaller());
    fflush(stdout);
    UWHookRelease HR(hook0.value());
    hook0->getFunction<void(int)>()(a);
}
UWHOOK_TRAMPOLINE(test_hook2)
#endif

int main() {
    printf("actual_test0 is at 0x%p\n", actual_test0);
    printf("actual_test1 is at 0x%p\n", actual_test1);
    printf("\n");
    actual_test0(1);
    actual_test1(1);
    printf("\n");
    hook0.emplace(reinterpret_cast<void *>(actual_test0), reinterpret_cast<void *>(test_hook0));
    actual_test0(2);
    printf("\n");
    hook0.emplace(reinterpret_cast<void *>(actual_test0), reinterpret_cast<void *>(test_hook1));
    actual_test0(3);
    printf("\n");
#ifdef UWHOOK_TRAMPOLINE_SUPPORT
    hook0.emplace(reinterpret_cast<void *>(actual_test0), reinterpret_cast<void *>(uwHookTrampoline_test_hook2), true);
    hook1.emplace(reinterpret_cast<void *>(actual_test1), reinterpret_cast<void *>(uwHookTrampoline_test_hook2), true);
    actual_test0(4);
    printf("\n");
    actual_test1(4);
    printf("\n");
    actual_test1(5);
    printf("\n");
#endif
    puts("End");
}
