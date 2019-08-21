#include <cutils/iosched_policy.h>

int android_set_ioprio(int pid, IoSchedClass clazz, int ioprio) {
    return 0;
}

int android_get_ioprio(int pid, IoSchedClass *clazz, int *ioprio) {
    *clazz = IoSchedClass_NONE;
    *ioprio = 0;
    return 0;
}

