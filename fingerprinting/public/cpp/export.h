#ifndef FINGERPRINTING_PUBLIC_EXPORT_H_
#define FINGERPRINTING_PUBLIC_EXPORT_H_

// Existing Class Export Logic
#if defined(WIN32)

#if defined(FINGERPRINTING_PUBLIC_IMPL)
#define FINGERPRINTING_PUBLIC_EXPORT __declspec(dllexport)
#else
#define FINGERPRINTING_PUBLIC_EXPORT __declspec(dllimport)
#endif  // defined(FINGERPRINTING_PUBLIC_IMPL)

#else  // defined(WIN32)
#if defined(FINGERPRINTING_PUBLIC_IMPL)
#define FINGERPRINTING_PUBLIC_EXPORT __attribute__((visibility("default")))
#else
#define FINGERPRINTING_PUBLIC_EXPORT
#endif

#endif

#endif  // FINGERPRINTING_PUBLIC_EXPORT_H_
