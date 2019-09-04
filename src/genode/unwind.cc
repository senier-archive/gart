#include <base/log.h>
#include <unwind.h>

#ifdef __cplusplus
extern "C" {
#endif

_Unwind_Reason_Code _Unwind_Backtrace (_Unwind_Trace_Fn, void *)
{
    Genode::warning(__func__, ": NOT IMPLEMENTED");
    return _URC_NO_REASON;
}

unsigned long _Unwind_GetIP (struct _Unwind_Context *)
{
    return 0;
}

#ifdef __cplusplus
}
#endif
