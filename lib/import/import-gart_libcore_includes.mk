INC_DIR := $(INC_DIR) \
	$(call select_from_ports,gart_core)/system/core/include \
	$(call select_from_ports,gart_core)/system/core/base/include
