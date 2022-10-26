# SPDX-License-Identifier: GPL-2.0

obj-$(CONFIG_RUST_MYMEM) += rust_mymem_wrapper.o
obj-$(CONFIG_RUST_MYMEM_BENCHMARK) += rust_mymem_benchmark.o

always-$(CONFIG_RUST_MYMEM) += exports_custom_modules_generated.h

rust_mymem_wrapper-y := rust_mymem.o exports.o

# sed -i.bak '/init_module/d' ./$@
# 	-i.bak: backup the file
#	'/init_module/d': match /init_module, then /d means delete the matched line
#	./$@: input file name
quiet_cmd_exports = EXPORTS $@
      cmd_exports = \
	$(NM) -p --defined-only $< \
		| grep -E ' (T|R|D) ' | cut -d ' ' -f 3 \
		| xargs -Isymbol \
		echo 'EXPORT_SYMBOL_RUST_GPL(symbol);' > $@\
		&& sed -i.bak '/init_module/d' ./$@\
		&& sed -i.bak '/cleanup_module/d' ./$@

$(obj)/exports_custom_modules_generated.h: $(obj)/rust_mymem.o FORCE
	$(call if_changed,exports)

# do not include caller module when generating the lib
generate_lib:
	KRUSTFLAGS="--emit metadata" make -C ../../.. CONFIG_RUST_MYMEM_BENCHMARK=n SUBDIRS=./samples/rust/custom_modules modules -j16

generate_with_lib:
	KRUSTFLAGS="--extern rust_mymem=./samples/rust/custom_modules/librust_mymem.rmeta" \
	make -C ../../.. SUBDIRS=./samples/rust/custom_modules modules -j16
