#include <base/log.h>
#include <base/shared_object.h>

extern "C"
{
   void __cyg_profile_func_enter (void *f, void *c)
   {
	  try {
		  Genode::Address_info function(reinterpret_cast<Genode::addr_t>(f));
		  Genode::Address_info caller(reinterpret_cast<Genode::addr_t>(c));

    	  Genode::log (
    	     "<call addr=\"", Genode::Hex (function.addr), "\" path=\"", function.path, "\" name=\"", function.name,"\">"
    	     	,"<caller addr=\"", Genode::Hex (caller.addr), "\" path=\"", caller.path, "\" name=\"", caller.name, "\">"
			);
		} catch (Genode::Address_info::Invalid_address) {
    	  Genode::log (
    	     "<call addr=\"", Genode::Hex (reinterpret_cast<unsigned long long>(f)), "\">"
    	     	, "<caller addr=\"", Genode::Hex (reinterpret_cast<unsigned long long>(c)) ,"\"/>"
			);
		}
   }

   void __cyg_profile_func_exit (void *func __attribute__((unused)), void *caller __attribute__((unused)))
   {
      //Genode::log ("EXIT: func=", Genode::Hex ((unsigned long long)func),
      //   " caller=", Genode::Hex ((unsigned long long)caller));
      Genode::log ("</call>");
   }
}
