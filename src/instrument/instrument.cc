#include <base/log.h>

extern "C"
{
   void __cyg_profile_func_enter (void *func, void *caller)
   {
      Genode::log ("ENTER: func=", Genode::Hex ((unsigned long long)func),
         " caller=", Genode::Hex ((unsigned long long)caller));
   }

   void __cyg_profile_func_exit (void *func, void *caller)
   {
      Genode::log ("EXIT: func=", Genode::Hex ((unsigned long long)func),
         " caller=", Genode::Hex ((unsigned long long)caller));
   }
}
