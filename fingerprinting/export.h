// fingerprinting/export.h
#ifndef FINGERPRINTING_EXPORT_H_
#define FINGERPRINTING_EXPORT_H_

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

#endif  // FINGERPRINTING_EXPORT_H_
