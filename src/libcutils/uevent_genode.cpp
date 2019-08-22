#include <base/log.h>
#include <cutils/uevent.h>
#include <unistd.h>

extern "C" {

ssize_t uevent_kernel_multicast_recv(int socket, void* buffer, size_t length) {
   Genode::log(__func__, " not implemented");
   return -1;
}

ssize_t uevent_kernel_multicast_uid_recv(int socket, void* buffer, size_t length, uid_t* uid) {
   Genode::log(__func__, " not implemented");
   return -1;
}

ssize_t uevent_kernel_recv(int socket, void* buffer, size_t length, bool require_group, uid_t* uid) {
   Genode::log(__func__, " not implemented");
   return -1;
}

int uevent_open_socket(int buf_sz, bool passcred) {
   Genode::log(__func__, " not implemented");
   return -1;
}

}
