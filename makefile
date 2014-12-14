Run:	USpan.cpp function.cpp
	make Convert && g++ USpan.cpp function.cpp && ./a.out data/utilityMatrix.txt 50

Clean:
	rm ./a.out

Convert: converter.cpp
	g++ converter.cpp && ./a.out ./data/sample.txt
