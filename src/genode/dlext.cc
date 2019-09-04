#include <base/log.h>
#include <dlfcn.h>
#include <android/dlext.h>

void* android_dlopen_ext(const char* __filename, int __flags, const android_dlextinfo* __info) {
    return dlopen(__filename, __flags);
}

#define NOT_IMPLEMENTED Genode::warning(__func__, ": NOT IMPLEMENTED")

#ifdef __cplusplus
extern "C" {
#endif

bool android_init_anonymous_namespace(const char* shared_libs_sonames,
                                      const char* library_search_path) {
    NOT_IMPLEMENTED;
    return false;
}

android_namespace_t* android_get_exported_namespace(const char* name)
{
    NOT_IMPLEMENTED;
    return nullptr;
}

bool android_link_namespaces(android_namespace_t* from,
                             android_namespace_t* to,
                             const char* shared_libs_sonames)
{
    NOT_IMPLEMENTED;
    return false;
}

struct android_namespace_t* android_create_namespace(const char* name,
                                                     const char* ld_library_path,
                                                     const char* default_library_path,
                                                     uint64_t type,
                                                     const char* permitted_when_isolated_path,
                                                     android_namespace_t* parent)
{
    NOT_IMPLEMENTED;
    return nullptr;
}

#ifdef __cplusplus
}
#endif
