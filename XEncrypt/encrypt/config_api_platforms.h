#pragma once

#include "config_platforms.h"
#if !defined(X_ENCRYPT_EXPORT)
#ifdef _MSC_VER
# include <malloc.h>
# define X_ENCRYPT_EXPORT __declspec(dllexport)
# define X_ENCRYPT_IMPORT __declspec(dllimport)
#elif X_ENCRYPT_TARGET_PSP2 || X_ENCRYPT_TARGET_PS4
# define X_ENCRYPT_EXPORT __declspec(dllexport)
# define X_ENCRYPT_IMPORT __declspec(dllimport)
#else
# define X_ENCRYPT_EXPORT __attribute__ ((visibility ("default")))
# define X_ENCRYPT_IMPORT
#endif
#endif
