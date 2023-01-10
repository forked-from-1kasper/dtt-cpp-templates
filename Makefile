all:
	g++ -std=c++2a DTT.cpp -o DTT

bench:
	time g++ -std=c++2a N3000.cpp -o N3000 -ftemplate-depth=10000

cubical:
	g++ -std=c++2a Cubical.cpp -o Cubical

run: all
	./DTT

clean:
	rm -f DTT