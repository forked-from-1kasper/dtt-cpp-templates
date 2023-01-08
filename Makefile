FILES=DTT.cpp Check.hpp Conv.hpp Eval.hpp List.hpp Literal.hpp Nat.hpp PP.hpp Val.hpp

all:
	g++ -std=c++2a DTT.cpp -o DTT

run: all
	./DTT

clean:
	rm -f DTT