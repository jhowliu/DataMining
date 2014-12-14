Run:	USpan.cpp function.cpp
	make Parse && g++ USpan.cpp function.cpp && ./a.out data/UtilityMatrix.txt 50

Clean:
	rm ./a.out ./data/quan_sample.txt ./data/UtilityMatrix.txt

Convert: converter.cpp
	g++ converter.cpp && ./a.out ./data/sample.txt 10 10

Parse: Parser.py
	make Convert && python Parser.py ./data/quan_sample.txt
