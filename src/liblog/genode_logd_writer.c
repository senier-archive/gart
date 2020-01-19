#include <errno.h>
#include <unistd.h>
#include <android/log.h>
#include <log/uio.h>
#include <liblog/config_write.h>
#include <liblog/logger.h>

static int genodeAvailable(log_id_t LogId);
static int genodeOpen();
static void genodeClose();
static int genodeWrite(log_id_t logId, struct timespec* ts, struct iovec* vec, size_t nr);

LIBLOG_HIDDEN struct android_log_transport_write logdLoggerWrite = {
  .node = { &logdLoggerWrite.node, &logdLoggerWrite.node },
  .context.sock = -EBADF,
  .name = "genode",
  .available = genodeAvailable,
  .open = genodeOpen,
  .close = genodeClose,
  .write = genodeWrite,
};

static int genodeOpen() {
   return 0;
}

static void genodeClose() {
   return;
}

static int genodeAvailable(log_id_t logId) {
   return 1;
}

static char *idstr(log_id_t logId) {
   switch (logId) {
      case LOG_ID_RADIO: return "RADIO";
      case LOG_ID_SYSTEM: return "SYSTEM";
      case LOG_ID_MAIN: return "MAIN";
      case LOG_ID_EVENTS: return "EVENTS";
      case LOG_ID_CRASH: return "CRASH";
      case LOG_ID_STATS: return "STATS";
      case LOG_ID_SECURITY: return "SECURITY";
      case LOG_ID_KERNEL: return "KERNEL";
      case LOG_ID_MAX: return "MAX";
   };
   return "INVALID";
}

static int genodeWrite(log_id_t logId, struct timespec* ts, struct iovec* vec, size_t nr) {
#if 0
   char buffer[1024];
   fprintf(stderr, "%lld.%.9ld [%s]: ", (long long)ts->tv_sec, ts->tv_nsec, idstr(logId));
   writev(2, vec, nr);
   fprintf(stderr, "\n");
#endif
   return nr;
}
