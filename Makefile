all: libtags.a

libtags.a: tags.o harness.o
	ar rc libtags.a tags.o harness.o
	ranlib libtags.a

tags.o: tags.c tags.h
	gcc -c tags.c tags.h

harness.o: harness.h harness.c
	gcc -c harness.c harness.h

clean:
	rm -f ./*.h.gch
	rm -f ./*.o

rm: clean
	rm -f ./libtags.a

install: libtags.a tags.h harness.h get_tag.2.gz get_tag.3.gz retrieve_get_tag_params.3.gz interpret_get_tag_result.3.gz
	mkdir -p /usr/local/share/man/man2
	mkdir -p /usr/local/share/man/man3
	cp -f get_tag.2.gz /usr/local/share/man/man2/
	ln -f /usr/local/share/man/man2/get_tag.2.gz /usr/local/share/man/man2/set_tag.2.gz
	cp -f get_tag.3.gz /usr/local/share/man/man3/
	ln -f /usr/local/share/man/man3/get_tag.3.gz /usr/local/share/man/man3/set_tag.3.gz
	cp -f retrieve_get_tag_params.3.gz /usr/local/share/man/man3/
	ln -f /usr/local/share/man/man3/retrieve_get_tag_params.3.gz /usr/local/share/man/man3/retrieve_set_tag_params.3.gz
	cp -f interpret_get_tag_result.3.gz /usr/local/share/man/man3/
	ln -f /usr/local/share/man/man3/interpret_get_tag_result.3.gz /usr/local/share/man/man3/interpret_set_tag_result.3.gz
	mandb
	cp -f tags.h /usr/local/include/
	cp -f harness.h /usr/local/include/
	cp -f libtags.a /usr/local/lib/
