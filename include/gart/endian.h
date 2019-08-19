#ifndef __GART_ENDIAN_H__
#define __GART_ENDIAN_H__

#include <sys/endian.h>
#include <netinet/in.h>

#define htonq(x) htobe64(x)
#define ntohq(x) be64toh(x)

#define __LITTLE_ENDIAN _LITTLE_ENDIAN
#define __BIG_ENDIAN _BIG_ENDIAN
#define __BYTE_ORDER _BYTE_ORDER

#define betoh16(x) be16toh(x)
#define betoh32(x) be32toh(x)
#define betoh64(x) be64toh(x)

#define letoh16(x) (x)
#define letoh32(x) (x)
#define letoh64(x) (x)

#endif // !__GART_ENDIAN_H__
