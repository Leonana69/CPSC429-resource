# SPDX-License-Identifier: GPL-2.0

obj-$(CONFIG_RUST_CAMERA) += rust_camera_wrapper.o

rust_camera_wrapper-y := rust_camera.o ../../../rust/bindings.o

camera:
	make -C ../../.. SUBDIRS=./samples/rust modules -j17

insmod:
	sudo insmod rust_camera_wrapper.ko
	sudo dmesg

rmmod:
	sudo rmmod rust_camera_wrapper
	sudo dmesg
