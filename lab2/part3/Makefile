all: user_camera_control.c utils.c
	cc -o user_camera_control -g user_camera_control.c utils.c
	@echo "### RUN ###"
	@sudo ./user_camera_control

clean:
	-rm -f *.o user_camera_control