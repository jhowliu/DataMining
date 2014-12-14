Run:	USpan.cpp function.cpp
	make Parse && g++ USpan.cpp function.cpp && ./a.out data/UtilityMatrix.txt 10000

Clean:
	rm ./a.out

Parse: Parser.py
	python Parser.py ./data/sample.txt
