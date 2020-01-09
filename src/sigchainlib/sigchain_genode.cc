#include <base/log.h>

#include "sigchain.h"

#if 0
#define NOT_IMPLEMENTED Genode::warning(__func__, ": not implemented")
#else
#define NOT_IMPLEMENTED
#endif

namespace art {

extern "C" void EnsureFrontOfChain(int signal) {
    NOT_IMPLEMENTED;
}

extern "C" void AddSpecialSignalHandlerFn(int signal, SigchainAction* sa) {
    NOT_IMPLEMENTED;
}

extern "C" void RemoveSpecialSignalHandlerFn(int signal,
                                             bool (*fn)(int, siginfo_t*, void*)) {
    NOT_IMPLEMENTED;
}

}
