DOXY_INSTPATH = /var/www/html/private/coht/doxy/vmeio

all:
	echo Make documentation for vmeio driver
	rm -f ,*.h
	rm -rf doc
	doxy/doxy.sh -n"Vmeio driver" -o "doc" ../vmeio.h

doxy_install: all
	mkdir -p $(DOXY_INSTPATH)
	cp -r doc/html/* $(DOXY_INSTPATH)
