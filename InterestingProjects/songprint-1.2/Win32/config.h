/* Win32 Default config.h file.
 * 
 * These defaults are fine for compilation of the libraries under MS VC++ 6.0. 
 * Minor modifications might be required to get it to compile under your specific
 * compiler.
 */

#ifndef _CONFIG_H
#define _CONFIG_H

#undef ENABLE_NLS
#undef HAVE_CATGETS
#undef HAVE_GETTEXT
#undef HAVE_LC_MESSAGES
#undef HAVE_STPCPY
#undef HAVE_LIBSM
#undef PACKAGE_LOCALE_DIR
#undef PACKAGE_DATA_DIR
#undef PACKAGE_SOURCE_DIR
#define HAVE_OGGVORBIS
#define HAVE_MPG123
#define HAVE_WAV
#undef int64_t
#define HAVE_STRING_H

/* If you have WINSOCK2 header files/libraries, define both of these.  If you
 * only have WINSOCK, only define HAVE_WINSOCK
 */
#define HAVE_WINSOCK2
#define HAVE_WINSOCK

/* We need to define a DLL export function for building the songprint DLL */
#define NEED_DLLEXPORT

#endif