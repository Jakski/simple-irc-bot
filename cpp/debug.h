#ifndef HAVE_DEBUG
#define HAVE_DEBUG

#include <stdio.h>

#define ERROR(...) \
    (fprintf(stderr, "ERROR in %s:%d : ", __FILE__,  __LINE__), \
     fprintf(stderr, __VA_ARGS__), \
     fprintf(stderr, "\n"), \
     exit(EXIT_FAILURE))

#define WARN(...) \
    (fprintf(stderr, "WARN in %s:%d : ", __FILE__,  __LINE__), \
     fprintf(stderr, __VA_ARGS__), \
     fprintf(stderr, "\n"))
    
#define STUB(...) \
    (fprintf(stdout, "STUB in %s:%d : ", __FILE__,  __LINE__), \
     fprintf(stdout, __VA_ARGS__), \
     fprintf(stdout, "\n"))

#define INFO(...) \
    (fprintf(stdout, "INFO in %s:%d : ", __FILE__,  __LINE__), \
     fprintf(stdout, __VA_ARGS__), \
     fprintf(stdout, "\n"))

#endif
