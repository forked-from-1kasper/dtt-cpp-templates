all:
	g++ -std=c++2a DTT.cpp -o DTT

bench:
	time g++ -std=c++2a N3000.cpp -o N3000 -ftemplate-depth=10000

run: all
	./DTT

clean:
	rm -f DTT