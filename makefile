Run:	USpan.cpp function.cpp
	make Parse && g++ USpan.cpp function.cpp && ./a.out data/UtilityMatrix.txt 50

Clean:
	rm ./a.out ./data/UtilityMatrix.txt

Parse: Parser.py
	python Parser.py ./data/sample.txt
