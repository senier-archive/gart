#ifdef __cplusplus
#include <atomic>

typedef std::atomic_bool atomic_bool;

using std::memory_order_relaxed;
using std::memory_order_acquire;
using std::memory_order_release;
using std::memory_order_acq_rel;
using std::memory_order_seq_cst;

#else
#include_next <stdatomic.h>
#endif
