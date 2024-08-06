CXX = gcc
CXXFLAGS = -std=c++17 -Wall -Werror -Wextra -Wshadow
CXXCOV = --coverage
OS := $(shell uname -s)

LIB_SOURCES = s21_matrix_oop.cc
LIB_OBJECTS = $(LIB_SOURCES:.cc=.o)

TEST_SOURCES = tests/tests.cc

ASAN_FLAGS = -fsanitize=address
LSAN_FLAGS = -fsanitize=leak

all: test

test: clean
	$(CXX) $(CXXFLAGS) $(TEST_SOURCES) $(LIB_SOURCES) -o test -lgtest -lstdc++ -lm
	./test

s21_matrix_oop.a:
	$(CXX) $(CXXFLAGS) -c $(LIB_SOURCES)
	ar rcs s21_matrix_oop.a $(LIB_OBJECTS)

gcov_report: clean
	$(CXX) $(TEST_SOURCES) $(CXXFLAGS) $(CXXCOV) $(LIB_SOURCES) -o test_cov -lgtest -lstdc++ -lm
	./test_cov
ifeq ($(OS), Linux)
	lcov -t "matrix" -o matrix.info --no-external -c -d .
else
	lcov -t "matrix" --ignore-errors mismatch -o matrix.info --no-external -c -d .
endif
	genhtml -o report matrix.info
	open report/index.html

test_asan: clean s21_matrix_oop.a
	$(CXX) $(CXXFLAGS) $(ASAN_FLAGS) $(TEST_SOURCES) s21_matrix_oop.a -o test_asan -lgtest -lstdc++ -lm
	./test_asan

test_lsan: clean s21_matrix_oop.a
	$(CXX) $(CXXFLAGS) $(LSAN_FLAGS) $(TEST_SOURCES) s21_matrix_oop.a -o test_lsan -lgtest -lstdc++ -lm
	./test_lsan

test_leaks: clean s21_matrix_oop.a
	$(CXX) $(CXXFLAGS) $(TEST_SOURCES) s21_matrix_oop.a -o test_leaks -lgtest -lstdc++ -lm
ifeq ($(OS), Linux)
	valgrind --leak-check=full ./test_leaks
else
	leaks -atExit -- ./test_leaks
endif

clean:
	rm -rf test test_cov test_asan test_lsan test_leaks *.o *.gcda *.gcno *.info *.a
	rm -rf report/

rebuild:
	make clean
	make all
