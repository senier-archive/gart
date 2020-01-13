#include <base/exception.h>
#include <gart/env.h>

class GART_Env_Uninitialized : Genode::Exception { };

static Genode::Env *_env_ptr = nullptr;

extern "C"
void gart::init_genode_env(Genode::Env *env)
{
	_env_ptr = env;
}

Genode::Env &gart::genode_env()
{
    if (!_env_ptr)
    {
        Genode::error("gart::init_genode_env() not called");
        throw GART_Env_Uninitialized();
    }
    return *_env_ptr;
}

Genode::Env *gart::genode_env_ptr()
{
    return _env_ptr;
}
