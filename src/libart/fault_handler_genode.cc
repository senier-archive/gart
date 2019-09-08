#include <base/log.h>
#include "fault_handler.h"

namespace art {

void FaultManager::GetMethodAndReturnPcAndSp(siginfo_t* siginfo ATTRIBUTE_UNUSED, void* context,
                                             ArtMethod** out_method,
                                             uintptr_t* out_return_pc, uintptr_t* out_sp) 
{
    Genode::warning(__func__, ": NOT IMPLEMENTED");
    *out_method = nullptr;
}

bool StackOverflowHandler::Action(int, siginfo_t* info, void* context) {
    Genode::warning(__func__, ": NOT IMPLEMENTED");
    return false;
}

bool NullPointerHandler::Action(int, siginfo_t* sig, void* context) {
    Genode::warning(__func__, ": NOT IMPLEMENTED");
    return false;
}

bool SuspensionHandler::Action(int, siginfo_t*, void* context) {
    Genode::warning(__func__, ": NOT IMPLEMENTED");
    return false;
}

}
