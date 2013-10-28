CFLAGS+= `pkg-config --cflags dbus-glib-1`
LDFLAGS+= `pkg-config --libs dbus-glib-1`

all: sh_daemon

sh_daemon: src/sh_daemon.o
	$(CC) -g -gg $(LDFLAGS)  -o $@ $^

clean:
	find . -name "*.o" -print0 | xargs -0 rm -f
	rm -f sh_daemon
