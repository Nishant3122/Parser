# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall

# Files
PARSER = test.y
LEXER  = lexer.l
EXEC   = compiler

SRC = main.cpp \
      Abstract_syntax_tree.cpp \
      symbol.cpp \
      evaluation.cpp

# Default target
all: $(EXEC)

# Bison
test.tab.c test.tab.h: $(PARSER)
	bison -d $(PARSER)

# Flex
lex.yy.c: $(LEXER) test.tab.h
	flex $(LEXER)

# Build executable
$(EXEC): test.tab.c lex.yy.c $(SRC)
	$(CXX) $(CXXFLAGS) test.tab.c lex.yy.c $(SRC) -o $(EXEC)

# Run program
run: $(EXEC)
	./$(EXEC) < test.txt

# Clean
clean:
	rm -f test.tab.c test.tab.h lex.yy.c $(EXEC)
