CFLAGS+= `pkg-config --cflags dbus-glib-1`
LDFLAGS+= `pkg-config --libs dbus-glib-1`
CONF= "org.arthurlambert.daemon.conf"

all: sh_daemon conf/$(CONF)

sh_daemon: src/sh_daemon.o
	$(CC) -g -gg $(LDFLAGS)  -o $@ $^

conf/$(CONF):
	@-mkdir conf
	@echo "<!DOCTYPE busconfig PUBLIC" > conf/$(CONF)
	@echo " \"-//freedesktop//DTD D-BUS Bus Configuration 1.0//EN\"" >> conf/$(CONF)
	@echo "\"http://www.freedesktop.org/standards/dbus/1.0/busconfig.dtd\">" >> conf/$(CONF)
	@echo "<busconfig>" >> conf/$(CONF)
	@echo "    <policy user=\"`whoami`\">" >> conf/$(CONF)
	@echo "        <allow own=\"org.arthurlambert.daemon\"/>" >> conf/$(CONF)
	@echo "        <allow send_destination=\"org.arthurlambert.daemon\"/>" >> conf/$(CONF)
	@echo "        <allow send_interface=\"org.arthurlambert.daemon\"/>" >> conf/$(CONF)
	@echo "    </policy>" >> conf/$(CONF)
	@echo "</busconfig>" >> conf/$(CONF)
	@sudo cp conf/* /etc/dbus-1/system.d/.

clean:
	find . -name "*.o" -print0 | xargs -0 rm -f
	rm -f sh_daemon
	rm -f test/mybinary
	rm -rf conf

./test/mybinary:
	gcc -o ./test/mybinary ./test/mybinary.c

test_error: all ./test/mybinary
	@./sh_daemon -e "./test/mybinary"  -n "org.arthurlambert.daemon" -p "/org/arthurlambert/daemon" &
	@sleep 2
	@echo "- - - - - - - - - - - - - - - - - - - - - - - - "
	@echo "Test 1 : My binary"
	dbus-send --system --print-reply --dest='org.arthurlambert.daemon' /org/arthurlambert/daemon org.arthurlambert.daemon.exec array:string:"./test/mybinary"

test: all
	@./sh_daemon -e "/bin/ls"  -n "org.arthurlambert.daemon" -p "/org/arthurlambert/daemon" &
	@sleep 2
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
	-@killall -9 sh_daemon
