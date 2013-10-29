CFLAGS+= `pkg-config --cflags dbus-glib-1`
LDFLAGS+= `pkg-config --libs dbus-glib-1`

all: sh_daemon

sh_daemon: src/sh_daemon.o
	$(CC) -g -gg $(LDFLAGS)  -o $@ $^

clean:
	find . -name "*.o" -print0 | xargs -0 rm -f
	rm -f sh_daemon

install: all
	@sudo cp conf/* /etc/dbus-1/system.d/.

test: install
	-@killall -9 sh_daemon
	@sleep 3
	@./sh_daemon -e "/bin/ls"  -n "org.arthurlambert.daemon" -p "/org/arthurlambert/daemon" &
	@sleep 3
	@echo "- - - - - - - - - - - - - - - - - - - - - - - - "
	@echo "Test 1 : ls src conf"
	dbus-send --system --print-reply --dest='org.arthurlambert.daemon' /org/arthurlambert/daemon org.arthurlambert.daemon.exec array:string:"/bin/ls","src","conf"
	@sleep 2
	@echo "- - - - - - - - - - - - - - - - - - - - - - - - "
	@echo "Test 2 : ls src fake_file"
	dbus-send --system --print-reply --dest='org.arthurlambert.daemon' /org/arthurlambert/daemon org.arthurlambert.daemon.exec array:string:"/bin/ls","src","fake file"
	@sleep 2
	@echo "- - - - - - - - - - - - - - - - - - - - - - - - "
	@echo "Test 3 : cat Makefile"
	dbus-send --system --print-reply --dest='org.arthurlambert.daemon' /org/arthurlambert/daemon org.arthurlambert.daemon.exec array:string:"/bin/cat","Makefile"
	@sleep 2
	@echo "- - - - - - - - - - - - - - - - - - - - - - - - "
	@echo "Test 4 : ls"
	dbus-send --system --print-reply --dest='org.arthurlambert.daemon' /org/arthurlambert/daemon org.arthurlambert.daemon.exec array:string:"/bin/ls",""

