#include <base/log.h>
#include <cutils/sched_policy.h>
#include <errno.h>
#include <stddef.h>

static inline SchedPolicy _policy(SchedPolicy p)
{
   Genode::warning("Not implemented: ", __func__);
   return p == SP_DEFAULT ? SP_SYSTEM_DEFAULT : p;
}

static int getCGroupSubsys(int tid, const char* subsys, char* buf, size_t bufLen)
{
   Genode::warning("Not implemented: ", __func__);
    errno = ENOSYS;
    return -1;
}

int get_sched_policy(int, SchedPolicy *policy)
{
   Genode::warning("Not implemented: ", __func__);
   *policy = SP_SYSTEM_DEFAULT;
   return 0;
}

int set_cpuset_policy(int, SchedPolicy)
{
   Genode::warning("Not implemented: ", __func__);
   return 0;
}

int set_sched_policy(int tid, SchedPolicy)
{
   Genode::warning("Not implemented: ", __func__);
   return 0;
}

const char *get_sched_policy_name(SchedPolicy policy)
{
   Genode::warning("Not implemented: ", __func__);
   return "not implemented";
}
