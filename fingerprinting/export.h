#ifndef FINGERPRINTING_EXPORT_H_
#define FINGERPRINTING_EXPORT_H_

// Existing Class Export Logic

#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(FINGERPRINTING_IMPLEMENTATION)
#define FINGERPRINTING_EXPORT __declspec(dllexport)
#else
#define FINGERPRINTING_EXPORT __declspec(dllimport)
#endif  // defined(FINGERPRINTING_IMPLEMENTATION)

#else  // defined(WIN32)
#if defined(FINGERPRINTING_IMPLEMENTATION)
#define FINGERPRINTING_EXPORT __attribute__((visibility("default")))
#else
#define FINGERPRINTING_EXPORT
#endif

#endif
#else  // defined(COMPONENT_BUILD)
#define FINGERPRINTING_EXPORT
#endif

#endif  // FINGERPRINTING_EXPORT_H_
