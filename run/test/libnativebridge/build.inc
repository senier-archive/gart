# close config
append config {
</config>
}

#
# Build
#

source ${genode_dir}/repos/base/run/platform_drv.inc
append_platform_drv_build_components
build $build_components
create_boot_directory

#
# Generate config
#

install_config $config

#
# Boot image
#

# generic modules
append_platform_drv_boot_modules
build_boot_image $boot_modules

append qemu_args " -m 1024 -nographic "
