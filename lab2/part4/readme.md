# Usage
Put the `custom_modules` folder under `linux/samples/rust` then add the following two lines in `linux/samples/rust/Makefile` and `linux/samples/rust/Kconfig`

```
# Makefile
obj-$(CONFIG_SAMPLES_RUST)				+= custom_modules/
```

```
# Kconfig, before `endif`
source "samples/rust/custom_modules/Kconfig"
```

Then `make menuconfig`, enable the `[*] CUSTOM_MODULES` and `<M> RUST_CAMERA`. Finally, in `custom_module`, `make camera` will generate the module.
