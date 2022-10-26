# Usage
Put the `custom_modules` folder under `linux/samples/rust` then add the following two lines in `linux/samples/Makefile` and `linux/samples/rust/Kconfig`

```
# Makefile
obj-$(CONFIG_SAMPLES_RUST)				+= custom_modules/
```

```
# Kconfig, before `endif`
source "samples/rust/custom_modules/Kconfig"
```

Then `make menuconfig`, enable the `[*] CUSTOM_MODULES`, `<M> RUST_MYMEM` and `<M> RUST_MYMEM_BENCHMARK`. Finally, in `custom_module`, `make generate_lib` will generate the `.rmeta` library, `rust_mymem_wrapper.ko` with the exported symbols. `make generate_with_lib` will generate the `rust_mymem_benchmark.ko` which is a module use the exported functions from `rust_mymem`.