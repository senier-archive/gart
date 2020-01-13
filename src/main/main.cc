/* Genode includes */
#include <libc/component.h>
#include <base/attached_rom_dataspace.h>
#include <util/xml_node.h>
#include <base/exception.h>

/* libc includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// GART includes
#include <gart/env.h>

enum { MAX_ARGS = 4096 };

class Env_Error : Genode::Exception { };

extern "C" int main (int argc, const char **argv);

#ifdef WAIT_FOR_DEBUGGER
extern "C" void wait_for_continue();
#define WAIT_FOR_CONTINUE Genode::log("WAITING FOR DEBUGGER..."); wait_for_continue()
#else
#define WAIT_FOR_CONTINUE
#endif

void Libc::Component::construct(Libc::Env &env)
{
    WAIT_FOR_CONTINUE;
    Genode::Attached_rom_dataspace config { env, "config" };
    gart::init_genode_env(&env);

	Libc::with_libc([&] {

       typedef Genode::String<2048> Value;
       int rv, argc = 0;
       const char *argv[MAX_ARGS];

       try {
         auto args = config.xml().sub_node("start");
         argv[argc++] = strdup(args.attribute_value("name", Value("/bin/unknown")).string());

         args.for_each_sub_node("arg", [&] (Genode::Xml_node arg_node) {
             argv[argc++] = strdup(arg_node.attribute_value("value", Value("ERROR")).string());
         });
         args.for_each_sub_node("env", [&] (Genode::Xml_node arg_node) {
             rv = setenv
                (arg_node.attribute_value("name", Value("=")).string(),
                 arg_node.attribute_value("value", Value("=")).string(),
                 0);
             if (rv != 0) {
                 throw Env_Error();
             }
         });
       } catch (Genode::Xml_node::Nonexistent_sub_node) {
         fprintf(stderr, "Invalid main() configuration, add <start> node\n");
         exit (-1);
       }

       argv[argc] = 0;

       setprogname (argv[0]);
       exit(main(argc, argv));
	});
}
