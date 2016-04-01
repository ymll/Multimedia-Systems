all:
	rm -rf temp/
	mkdir temp

	gcc -o temp/lzw.o lzw.c -Wall
	./temp/lzw.o -c temp/test_01_actual.lzw test/test_01_input.txt
	diff temp/test_01_actual.lzw test/test_01_expected.lzw -s
	./temp/lzw.o -c temp/test_02_actual.lzw test/test_02_input.txt
	diff temp/test_02_actual.lzw test/test_02_expected.lzw -s
