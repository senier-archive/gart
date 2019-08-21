/* Genode includes */
#include <libc/component.h>
#include <base/attached_rom_dataspace.h>

/* Googletest includes */
#include <gtest/gtest.h>

/* libc includes */
#include <stdio.h>
#include <stdlib.h>

enum { MAX_ARGS = 4096 };

void Libc::Component::construct(Libc::Env &env)
{
   Genode::Attached_rom_dataspace config { env, "config" };

	Libc::with_libc([&] {

       typedef Genode::String<512> Value;
       int argc = 0;
		 const char *argv[MAX_ARGS];

       auto args = config.xml().sub_node("start");
       argv[argc++] = strdup(args.attribute_value("name", Value("/bin/unknown")).string());

       args.for_each_sub_node("arg", [&] (Genode::Xml_node arg_node) {
           argv[argc++] = strdup(arg_node.attribute_value("value", Value("ERROR")).string());
       });
       argv[argc] = 0;

		 setprogname (argv[0]);
		 testing::InitGoogleTest(&argc, (char **)argv);
		 exit(RUN_ALL_TESTS());
	});
}
