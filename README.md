# UWHook

Minimal native function hooking library for x86_64 on Windows.

## Features

 * **NOT THREAD SAFE**: Optionally record address of called function before triggering hook (retrieve using `UWHook::getTrampolineCaller()`)
 * Temporary hook release using `UWHookRelease` RAII class
 * Very very minimal
 * No global state
 * Easily portable to other platforms

## Limitations

 * Only works on Windows for x86_64 (64 bit x86) programs
 * Won't hook inlined function calls
 * Can't pass user data to hook function
 * Requires function padding of at least 12 bytes in hooked code if any hooked function is shorter than 12 bytes
 * Allows no more than one hook on a single function

## Use cases

 * Reverse engineering
 * Game modding (works with il2cpp!)
