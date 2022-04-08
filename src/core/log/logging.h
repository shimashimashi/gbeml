#ifndef GBEML_LOGGING_H_
#define GBEML_LOGGING_H_

#ifdef __EMSCRIPTEN__

#ifndef NDEBUG

// wasm && not debug
#define LOG(STATUS)        // do nothing
#define VLOG(STATUS)       // do nothing
#define DLOG(STATUS)       // do nothing
#define DVLOG(STATUS)      // do nothing
#define DCHECK(condition)  // do nothing

#else

// wasm && debug
#include <iostream>
#define LOG(STATUS) std::cout << STATUS << ": "
#define VLOG(STATUS) std::cout << STATUS << ": "
#define ERROR "error"
#define INFO "info"
#define WARNING "warning"
#define FATAL "fatal"
#define DLOG(STATUS) std::cout << STATUS << ": "
#define DVLOG(STATUS) std::cout << STATUS << ": "
// TODO: implement DCHECK
#define DCHECK(condition) (void)0

#endif

#else

// not wasm
#include <glog/logging.h>

#endif

#endif  // GBEML_LOGGING_H_
