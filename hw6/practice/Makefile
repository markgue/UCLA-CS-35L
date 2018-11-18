CC=gcc



target: 
	cp a.txt b.txt

copy: 
	cp b.txt c.txt

copy2: 
	cp b.txt c.txt

a.txt:
	touch $@
	# $@: target name	

b.txt: a.txt c.txt
	cp $< $@
	cp $(word 2,$^) d.txt

c.txt:
	touch $@

# % allows you to simplify stuff with the same endings
%.txt:
	touch $@

compile: 
	gcc -o a a.c

clean: 
	rm b.txt | rm c.txt

