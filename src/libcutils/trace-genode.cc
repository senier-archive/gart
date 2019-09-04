#include <base/log.h>
#include <cutils/trace.h>

atomic_bool atrace_is_ready = ATOMIC_VAR_INIT(true);
int atrace_marker_fd = -1;
uint64_t atrace_enabled_tags = 0;

#define NOT_IMPLEMENTED Genode::warning(__func__, ": not implemented")

void atrace_set_debuggable(bool /*debuggable*/)
{
    NOT_IMPLEMENTED;
}

void atrace_set_tracing_enabled(bool /*enabled*/)
{
    NOT_IMPLEMENTED;
}

void atrace_update_tags()
{
    NOT_IMPLEMENTED;
}

void atrace_setup()
{
    NOT_IMPLEMENTED;
}

extern "C" {

void atrace_begin_body(const char* /*name*/)
{
    NOT_IMPLEMENTED;
}

void atrace_end_body()
{
    NOT_IMPLEMENTED;
}

void atrace_async_begin_body(const char* /*name*/, int32_t /*cookie*/)
{
    NOT_IMPLEMENTED;
}

void atrace_async_end_body(const char* /*name*/, int32_t /*cookie*/)
{
    NOT_IMPLEMENTED;
}

void atrace_int_body(const char* /*name*/, int32_t /*value*/)
{
    NOT_IMPLEMENTED;
}

void atrace_int64_body(const char* /*name*/, int64_t /*value*/)
{
    NOT_IMPLEMENTED;
}

}
