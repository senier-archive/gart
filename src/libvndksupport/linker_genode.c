#include <dlfcn.h>

#ifdef __cplusplus
extern "C" {
#endif

void* android_load_sphal_library(const char* name, int flag) {
   return dlopen(name, flag);
};

int android_unload_sphal_library(void* handle) {
   return dlclose(handle);
};

#ifdef __cplusplus
}
#endif
