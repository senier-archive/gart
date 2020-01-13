#include <base/env.h>

namespace gart {
    extern "C" void init_genode_env(Genode::Env *env);
    Genode::Env &genode_env();
    Genode::Env *genode_env_ptr();
}
