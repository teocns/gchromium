#ifndef FINGERPRINTING_UTILITY_EXPORT_H_
#define FINGERPRINTING_UTILITY_EXPORT_H_

// Existing Class Export Logic
#if defined(WIN32)

#if defined(FINGERPRINTING_UTILITY_IMPLEMENTATION)
#define FINGERPRINTING_UTILITY_EXPORT __declspec(dllexport)
#else
#define FINGERPRINTING_UTILITY_EXPORT __declspec(dllimport)
#endif  // defined(FINGERPRINTING_UTILITY_IMPLEMENTATION)

#else  // defined(WIN32)
#if defined(FINGERPRINTING_UTILITY_IMPLEMENTATION)
#define FINGERPRINTING_UTILITY_EXPORT __attribute__((visibility("default")))
#else
#define FINGERPRINTING_UTILITY_EXPORT
#endif

#endif

#endif  // FINGERPRINTING_UTILITY_EXPORT_H_
