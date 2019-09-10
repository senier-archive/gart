TARGET = vixl_test

ANDROID_PORT	     = vixl
ANDROID_DIR		     = vixl
ANDROID_BUILDFILES  = $(REP_DIR)/include/vixl/Android.bp Android.bp
ANDROID_SECTIONS    = /cc_test_host[@name=vixl-test-runner]
ANDROID_EXCLUDE_OPT = -Wredundant-decls -pedantic -DVIXL_DEBUG

LIBS += gart_main gart_libvixl-arm64 gart_libvixl-arm
CC_OPT += -Wno-unknown-pragmas
CC_OPT += -fno-var-tracking-assignments

include $(call select_from_repositories,lib/mk/android-prg.inc)

SRC_CC = \
	test/test-aborts.cc \
	test/test-api.cc \
	test/test-code-buffer.cc \
	test/test-code-generation-scopes.cc \
	test/test-invalset.cc \
	test/test-operands.cc \
	test/test-pool-manager.cc \
	test/test-runner.cc \
	test/test-use-scratch-register-scope.cc \
	test/test-utils.cc \
	test/aarch32/test-assembler-cond-dt-drt-drd-drn-drm-float-f32-only-a32.cc \
	test/aarch32/test-assembler-cond-dt-drt-drd-drn-drm-float-f32-only-t32.cc \
	test/aarch32/test-assembler-cond-dt-drt-drd-drn-drm-float-not-f16-a32.cc \
	test/aarch32/test-assembler-cond-dt-drt-drd-drn-drm-float-not-f16-t32.cc \
	test/aarch32/test-assembler-cond-rdlow-operand-imm8-in-it-block-t32.cc \
	test/aarch32/test-assembler-cond-rdlow-operand-imm8-t32.cc \
	test/aarch32/test-assembler-cond-rdlow-rnlow-operand-immediate-imm3-in-it-block-t32.cc \
	test/aarch32/test-assembler-cond-rdlow-rnlow-operand-immediate-imm3-t32.cc \
	test/aarch32/test-assembler-cond-rdlow-rnlow-operand-immediate-imm8-in-it-block-t32.cc \
	test/aarch32/test-assembler-cond-rdlow-rnlow-operand-immediate-imm8-t32.cc \
	test/aarch32/test-assembler-cond-rdlow-rnlow-operand-immediate-zero-in-it-block-t32.cc \
	test/aarch32/test-assembler-cond-rdlow-rnlow-operand-immediate-zero-t32.cc \
	test/aarch32/test-assembler-cond-rdlow-rnlow-rmlow-in-it-block-t32.cc \
	test/aarch32/test-assembler-cond-rdlow-rnlow-rmlow-t32.cc \
	test/aarch32/test-assembler-cond-rd-memop-immediate-512-a32.cc \
	test/aarch32/test-assembler-cond-rd-memop-immediate-8192-a32.cc \
	test/aarch32/test-assembler-cond-rd-memop-rs-a32.cc \
	test/aarch32/test-assembler-cond-rd-memop-rs-shift-amount-1to31-a32.cc \
	test/aarch32/test-assembler-cond-rd-memop-rs-shift-amount-1to32-a32.cc \
	test/aarch32/test-assembler-cond-rd-operand-const-cannot-use-pc-a32.cc \
	test/aarch32/test-assembler-cond-rd-operand-const-can-use-pc-a32.cc \
	test/aarch32/test-assembler-cond-rd-operand-const-t32.cc \
	test/aarch32/test-assembler-cond-rd-operand-imm16-t32.cc \
	test/aarch32/test-assembler-cond-rd-operand-rn-a32.cc \
	test/aarch32/test-assembler-cond-rd-operand-rn-identical-low-registers-in-it-block-t32.cc \
	test/aarch32/test-assembler-cond-rd-operand-rn-in-it-block-t32.cc \
	test/aarch32/test-assembler-cond-rd-operand-rn-low-registers-in-it-block-t32.cc \
	test/aarch32/test-assembler-cond-rd-operand-rn-ror-amount-a32.cc \
	test/aarch32/test-assembler-cond-rd-operand-rn-ror-amount-t32.cc \
	test/aarch32/test-assembler-cond-rd-operand-rn-shift-amount-1to31-a32.cc \
	test/aarch32/test-assembler-cond-rd-operand-rn-shift-amount-1to31-in-it-block-t32.cc \
	test/aarch32/test-assembler-cond-rd-operand-rn-shift-amount-1to31-t32.cc \
	test/aarch32/test-assembler-cond-rd-operand-rn-shift-amount-1to32-a32.cc \
	test/aarch32/test-assembler-cond-rd-operand-rn-shift-amount-1to32-in-it-block-t32.cc \
	test/aarch32/test-assembler-cond-rd-operand-rn-shift-amount-1to32-t32.cc \
	test/aarch32/test-assembler-cond-rd-operand-rn-shift-rs-a32.cc \
	test/aarch32/test-assembler-cond-rd-operand-rn-shift-rs-in-it-block-t32.cc \
	test/aarch32/test-assembler-cond-rd-operand-rn-shift-rs-narrow-out-it-block-t32.cc \
	test/aarch32/test-assembler-cond-rd-operand-rn-shift-rs-t32.cc \
	test/aarch32/test-assembler-cond-rd-operand-rn-t32.cc \
	test/aarch32/test-assembler-cond-rd-pc-operand-imm12-t32.cc \
	test/aarch32/test-assembler-cond-rd-pc-operand-imm8-t32.cc \
	test/aarch32/test-assembler-cond-rd-rn-a32.cc \
	test/aarch32/test-assembler-cond-rd-rn-operand-const-a32.cc \
	test/aarch32/test-assembler-cond-rd-rn-operand-const-t32.cc \
	test/aarch32/test-assembler-cond-rd-rn-operand-imm12-t32.cc \
	test/aarch32/test-assembler-cond-rd-rn-operand-rm-a32.cc \
	test/aarch32/test-assembler-cond-rd-rn-operand-rm-all-low-in-it-block-t32.cc \
	test/aarch32/test-assembler-cond-rd-rn-operand-rm-all-low-rd-is-rn-in-it-block-t32.cc \
	test/aarch32/test-assembler-cond-rd-rn-operand-rm-rd-is-rn-in-it-block-t32.cc \
	test/aarch32/test-assembler-cond-rd-rn-operand-rm-rd-is-rn-is-sp-in-it-block-t32.cc \
	test/aarch32/test-assembler-cond-rd-rn-operand-rm-rn-is-sp-in-it-block-t32.cc \
	test/aarch32/test-assembler-cond-rd-rn-operand-rm-ror-amount-a32.cc \
	test/aarch32/test-assembler-cond-rd-rn-operand-rm-ror-amount-t32.cc \
	test/aarch32/test-assembler-cond-rd-rn-operand-rm-shift-amount-1to31-a32.cc \
	test/aarch32/test-assembler-cond-rd-rn-operand-rm-shift-amount-1to31-t32.cc \
	test/aarch32/test-assembler-cond-rd-rn-operand-rm-shift-amount-1to32-a32.cc \
	test/aarch32/test-assembler-cond-rd-rn-operand-rm-shift-amount-1to32-t32.cc \
	test/aarch32/test-assembler-cond-rd-rn-operand-rm-shift-rs-a32.cc \
	test/aarch32/test-assembler-cond-rd-rn-operand-rm-t32.cc \
	test/aarch32/test-assembler-cond-rd-rn-rm-a32.cc \
	test/aarch32/test-assembler-cond-rd-rn-rm-t32.cc \
	test/aarch32/test-assembler-cond-rd-rn-t32.cc \
	test/aarch32/test-assembler-cond-rd-sp-operand-imm8-t32.cc \
	test/aarch32/test-assembler-cond-sp-sp-operand-imm7-t32.cc \
	test/aarch32/test-assembler-negative-cond-rd-rn-operand-rm-shift-rs-a32.cc \
	test/aarch32/test-assembler-rd-rn-rm-a32.cc \
	test/aarch32/test-assembler-rd-rn-rm-t32.cc \
	test/aarch32/test-disasm-a32.cc \
	test/aarch32/test-macro-assembler-cond-rd-rn-a32.cc \
	test/aarch32/test-macro-assembler-cond-rd-rn-pc-a32.cc \
	test/aarch32/test-macro-assembler-cond-rd-rn-t32.cc \
	test/aarch32/test-simulator-cond-dt-drt-drd-drn-drm-float-f64-a32.cc \
	test/aarch32/test-simulator-cond-dt-drt-drd-drn-drm-float-f64-t32.cc \
	test/aarch32/test-simulator-cond-rdlow-operand-imm8-t32.cc \
	test/aarch32/test-simulator-cond-rdlow-rnlow-operand-immediate-t32.cc \
	test/aarch32/test-simulator-cond-rdlow-rnlow-rmlow-t32.cc \
	test/aarch32/test-simulator-cond-rd-memop-immediate-512-a32.cc \
	test/aarch32/test-simulator-cond-rd-memop-immediate-8192-a32.cc \
	test/aarch32/test-simulator-cond-rd-memop-rs-a32.cc \
	test/aarch32/test-simulator-cond-rd-memop-rs-shift-amount-1to31-a32.cc \
	test/aarch32/test-simulator-cond-rd-memop-rs-shift-amount-1to32-a32.cc \
	test/aarch32/test-simulator-cond-rd-operand-const-a32.cc \
	test/aarch32/test-simulator-cond-rd-operand-const-t32.cc \
	test/aarch32/test-simulator-cond-rd-operand-imm16-t32.cc \
	test/aarch32/test-simulator-cond-rd-operand-rn-a32.cc \
	test/aarch32/test-simulator-cond-rd-operand-rn-ror-amount-a32.cc \
	test/aarch32/test-simulator-cond-rd-operand-rn-ror-amount-t32.cc \
	test/aarch32/test-simulator-cond-rd-operand-rn-shift-amount-1to31-a32.cc \
	test/aarch32/test-simulator-cond-rd-operand-rn-shift-amount-1to31-t32.cc \
	test/aarch32/test-simulator-cond-rd-operand-rn-shift-amount-1to32-a32.cc \
	test/aarch32/test-simulator-cond-rd-operand-rn-shift-amount-1to32-t32.cc \
	test/aarch32/test-simulator-cond-rd-operand-rn-shift-rs-a32.cc \
	test/aarch32/test-simulator-cond-rd-operand-rn-shift-rs-t32.cc \
	test/aarch32/test-simulator-cond-rd-operand-rn-t32.cc \
	test/aarch32/test-simulator-cond-rd-rn-a32.cc \
	test/aarch32/test-simulator-cond-rd-rn-operand-const-a32.cc \
	test/aarch32/test-simulator-cond-rd-rn-operand-const-t32.cc \
	test/aarch32/test-simulator-cond-rd-rn-operand-imm12-t32.cc \
	test/aarch32/test-simulator-cond-rd-rn-operand-rm-a32.cc \
	test/aarch32/test-simulator-cond-rd-rn-operand-rm-ror-amount-a32.cc \
	test/aarch32/test-simulator-cond-rd-rn-operand-rm-ror-amount-t32.cc \
	test/aarch32/test-simulator-cond-rd-rn-operand-rm-shift-amount-1to31-a32.cc \
	test/aarch32/test-simulator-cond-rd-rn-operand-rm-shift-amount-1to31-t32.cc \
	test/aarch32/test-simulator-cond-rd-rn-operand-rm-shift-amount-1to32-a32.cc \
	test/aarch32/test-simulator-cond-rd-rn-operand-rm-shift-amount-1to32-t32.cc \
	test/aarch32/test-simulator-cond-rd-rn-operand-rm-shift-rs-a32.cc \
	test/aarch32/test-simulator-cond-rd-rn-operand-rm-t32.cc \
	test/aarch32/test-simulator-cond-rd-rn-rm-a32.cc \
	test/aarch32/test-simulator-cond-rd-rn-rm-ge-a32.cc \
	test/aarch32/test-simulator-cond-rd-rn-rm-ge-t32.cc \
	test/aarch32/test-simulator-cond-rd-rn-rm-q-a32.cc \
	test/aarch32/test-simulator-cond-rd-rn-rm-q-t32.cc \
	test/aarch32/test-simulator-cond-rd-rn-rm-sel-a32.cc \
	test/aarch32/test-simulator-cond-rd-rn-rm-sel-t32.cc \
	test/aarch32/test-simulator-cond-rd-rn-rm-t32.cc \
	test/aarch32/test-simulator-cond-rd-rn-t32.cc \
	test/aarch32/test-simulator-rd-rn-rm-a32.cc \
	test/aarch32/test-simulator-rd-rn-rm-t32.cc \
	test/aarch32/test-utils-aarch32.cc \
	test/aarch64/test-abi.cc \
	test/aarch64/test-api-aarch64.cc \
	test/aarch64/test-assembler-aarch64.cc \
	test/aarch64/test-disasm-aarch64.cc \
	test/aarch64/test-fuzz-aarch64.cc \
	test/aarch64/test-simulator-aarch64.cc \
	test/aarch64/test-trace-aarch64.cc \
	test/aarch64/test-utils-aarch64.cc \
	test/aarch32/test-assembler-aarch32.cc \
