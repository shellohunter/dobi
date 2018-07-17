
LDFLAGS+=-ljpeg -lv4l2

mysky:
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ v4l2grab.c yuv.c

clean:
	-rm -f *.o *.elf *.gbd mysky




