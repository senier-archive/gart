#ifdef __cplusplus
#include <atomic>

typedef std::atomic_bool atomic_bool;

using std::memory_order_relaxed;
using std::memory_order_acquire;
using std::memory_order_release;
using std::memory_order_acq_rel;
using std::memory_order_seq_cst;

using atomic_int_least8_t   = std::atomic<int_least8_t>;
using atomic_uint_least8_t  = std::atomic<uint_least8_t>;
using atomic_int_least16_t  = std::atomic<int_least16_t>;
using atomic_uint_least16_t = std::atomic<uint_least16_t>;
using atomic_int_least32_t  = std::atomic<int_least32_t>;
using atomic_uint_least32_t = std::atomic<uint_least32_t>;
using atomic_int_least64_t  = std::atomic<int_least64_t>;
using atomic_uint_least64_t = std::atomic<uint_least64_t>;

#else
#include_next <stdatomic.h>
#endif
