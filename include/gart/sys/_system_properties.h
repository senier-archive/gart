// FIXME: We mock this for now. However, this could be mapped e.g. to a <config> node on Genode.

#ifndef __GART_SYS__SYSTEM_PROPERTIES_H__
#define __GART_SYS__SYSTEM_PROPERTIES_H__

#include <sys/types.h>

#define PROP_VALUE_MAX 92

typedef struct prop_info { } prop_info;

static uint32_t __system_property_serial(const prop_info* __pi) { return 0; };
static const prop_info* __system_property_find(const char* __name) { return NULL; };
static int __system_property_read(const prop_info* __pi, char* __name, char* __value) { return 0; };
static uint32_t __system_property_area_serial(void) { return 0; };

#endif // !__GART_SYS__SYSTEM_PROPERTIES_H__
