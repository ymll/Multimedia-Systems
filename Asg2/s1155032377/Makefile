all:
	rm -rf temp/
	mkdir temp

	gcc -o temp/lzw.o lzw.c -Wall

	# Tests for compression
	./temp/lzw.o -c temp/test_01_actual.lzw test/test_01_input.txt
	diff temp/test_01_actual.lzw test/test_01_expected.lzw -s
	./temp/lzw.o -c temp/test_02_actual.lzw test/test_02_input.txt
	diff temp/test_02_actual.lzw test/test_02_expected.lzw -s

	# Tests for decompression
	./temp/lzw.o -d test/test_03_input.lzw
	diff temp/test_03_actual.txt test/test_03_expected.txt -s
	diff temp/test_03_actual.txt test/test_01_input.txt -s
	./temp/lzw.o -d test/test_04_input.lzw
	diff temp/test_04_actual.txt test/test_04_expected.txt -s
	diff temp/test_04_actual.txt test/test_02_input.txt -s

	# Test for multi-file compression
	./temp/lzw.o -c temp/test_05_actual.lzw test/test_05_input_a.txt test/test_05_input_b.txt
	diff temp/test_05_actual.lzw test/test_05_expected.lzw -s

	# Test for multi-file decompression
	./temp/lzw.o -d test/test_06_input.lzw
	diff temp/test_06_actual_a.txt test/test_06_expected_a.txt -s
	diff temp/test_06_actual_b.txt test/test_06_expected_b.txt -s

	# Tests for big file compression
	./temp/lzw.o -c temp/test_07_actual.lzw test/test_07_input.bmp
	diff temp/test_07_actual.lzw test/test_07_expected.lzw -s

	# Tests for big file decompression
	./temp/lzw.o -d test/test_08_input.lzw
	diff temp/test_08_actual.bmp test/test_08_expected.bmp -s

	# Tests for big file compression + decompression
	cp test/test_09_input.bmp temp/test_09_actual.bmp
	./temp/lzw.o -c temp/test_09_actual.lzw temp/test_09_actual.bmp
	mv temp/test_09_actual.bmp temp/test_09_input.bmp
	./temp/lzw.o -d temp/test_09_actual.lzw
	diff temp/test_09_actual.bmp temp/test_09_input.bmp -s
	diff temp/test_09_actual.bmp test/test_09_input.bmp -s
