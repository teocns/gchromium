#ifndef FINGERPRINTING_CORE_EXPORT_H_
#define FINGERPRINTING_CORE_EXPORT_H_

// Existing Class Export Logic
#if defined(WIN32)

#if defined(FINGERPRINTING_IMPLEMENTATION)
#define FINGERPRINTING_CORE_EXPORT __declspec(dllexport)
#else
#define FINGERPRINTING_CORE_EXPORT __declspec(dllimport)
#endif  // defined(FINGERPRINTING_IMPLEMENTATION)

#else  // defined(WIN32)
#if defined(FINGERPRINTING_IMPLEMENTATION)
#define FINGERPRINTING_CORE_EXPORT __attribute__((visibility("default")))
#else
#define FINGERPRINTING_CORE_EXPORT
#endif

#endif

#endif  // FINGERPRINTING_CORE_EXPORT_H_
