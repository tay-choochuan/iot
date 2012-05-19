
#ifndef WOLFMQTT_VISIBILITY_H
#define WOLFMQTT_VISIBILITY_H

/* WOLFMQTT_API is used for the public API symbols.
        It either imports or exports (or does nothing for static builds)

   WOLFMQTT_LOCAL is used for non-API symbols (private).
*/

#if defined(BUILDING_WOLFMQTT)
    #if defined(HAVE_VISIBILITY) && HAVE_VISIBILITY
        #define WOLFMQTT_API   __attribute__ ((visibility("default")))
        #define WOLFMQTT_LOCAL __attribute__ ((visibility("hidden")))
    #elif defined(__SUNPRO_C) && (__SUNPRO_C >= 0x550)
        #define WOLFMQTT_API   __global
        #define WOLFMQTT_LOCAL __hidden
    #elif defined(_MSC_VER)
        #ifdef _WINDLL
            #define WOLFMQTT_API __declspec(dllexport)
        #else
            #define WOLFMQTT_API
        #endif
        #define WOLFMQTT_LOCAL
    #else
        #define WOLFMQTT_API
        #define WOLFMQTT_LOCAL
    #endif /* HAVE_VISIBILITY */
#else /* BUILDING_WOLFMQTT */
    #if defined(_MSC_VER)
        #define WOLFMQTT_API __declspec(dllimport)
        #define WOLFMQTT_LOCAL
    #else
        #define WOLFMQTT_API
        #define WOLFMQTT_LOCAL
    #endif
#endif /* BUILDING_WOLFMQTT */

#endif /* WOLFMQTT_VISIBILITY_H */
