set build_components {
	core init timer
	test/libnativebridge-dummy
}

set config {
<config verbose="yes">
	<parent-provides>
		<service name="ROM"/>
		<service name="CAP"/>
		<service name="RM"/>
		<service name="PD"/>
		<service name="CPU"/>
		<service name="LOG"/>
		<service name="SIGNAL"/>
		<service name="IO_PORT"/>
		<service name="IRQ"/>
		<service name="IO_MEM"/>
	</parent-provides>
	<default-route>
		<any-service> <parent/> <any-child/> </any-service>
	</default-route>
	<default caps="200"/>

	<start name="timer">
		<resource name="RAM" quantum="1M"/>
		<provides> <service name="Timer"/> </provides>
	</start>
}

set boot_modules {
	core init ld.lib.so timer
	libc.lib.so libm.lib.so stdcxx.lib.so vfs.lib.so gtest.lib.so
	gart_liblog.lib.so gart_libbase.lib.so gart_libnativehelper.lib.so gart_libnativebridge.lib.so
	libnativebridge-dummy.lib.so
	libnativebridge2-dummy.lib.so
	libnativebridge3-dummy.lib.so
}

set timeout 10
