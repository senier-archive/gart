#ifdef __cplusplus
#include <atomic>

typedef std::atomic_bool atomic_bool;

#define memory_order_relaxed std::memory_order_relaxed
#define memory_order_acquire std::memory_order_acquire
#define memory_order_release std::memory_order_release
#define memory_order_acq_rel std::memory_order_acq_rel
#define memory_order_seq_cst std::memory_order_seq_cst

#else
#include_next <stdatomic.h>
#endif
