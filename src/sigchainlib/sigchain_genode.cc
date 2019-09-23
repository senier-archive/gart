#include <base/log.h>

#include "sigchain.h"

namespace art {

extern "C" void EnsureFrontOfChain(int signal) {
  Genode::warning(__func__, ": not implemented");
}

extern "C" void AddSpecialSignalHandlerFn(int signal, SigchainAction* sa) {
  Genode::warning(__func__, ": not implemented");
}

extern "C" void RemoveSpecialSignalHandlerFn(int signal,
                                             bool (*fn)(int, siginfo_t*, void*)) {
  Genode::warning(__func__, ": not implemented");
}

}
