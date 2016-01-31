MAIN = fp20compiler
CXXFLAGS= -DYY_NO_UNPUT -Wall -g

INCLUDES = $(wildcard *.h) \
	_ps1.0_parser.h \
	_rc1.0_parser.h \
	_ts1.0_parser.h

SRCS = main.cpp \
	nvparse_errors.cpp \
	rc1.0_combiners.cpp \
	rc1.0_final.cpp \
	rc1.0_general.cpp \
	ts1.0_inst.cpp \
	ts1.0_inst_list.cpp \
	_ts1.0_parser.cpp \
	_ts1.0_lexer.cpp \
	_rc1.0_lexer.cpp \
	_rc1.0_parser.cpp \
	_ps1.0_lexer.cpp \
	_ps1.0_parser.cpp \
	ps1.0_program.cpp

OBJS = $(SRCS:.cpp=.o)

$(MAIN): $(OBJS)
	$(CXX) -o '$@' $(OBJS)

%.o: %.cpp ${INCLUDES}
	$(CXX) $(CXXFLAGS) -c -o '$@' '$<'

_ps1.0_parser.cpp _ps1.0_parser.h: ps1.0_grammar.y
	bison -d -o _ps1.0_parser.c -p ps10_ ps1.0_grammar.y
	mv -f _ps1.0_parser.c _ps1.0_parser.cpp

_rc1.0_parser.cpp _rc1.0_parser.h: rc1.0_grammar.y
	bison -d -o _rc1.0_parser.c -p rc10_ rc1.0_grammar.y
	mv -f _rc1.0_parser.c _rc1.0_parser.cpp

_ts1.0_parser.cpp _ts1.0_parser.h: ts1.0_grammar.y
	bison -d -o _ts1.0_parser.c -p ts10_ ts1.0_grammar.y
	mv -f _ts1.0_parser.c _ts1.0_parser.cpp

_ps1.0_lexer.cpp: ps1.0_tokens.l
	flex -Pps10_ -o_ps1.0_lexer.cpp ps1.0_tokens.l

_rc1.0_lexer.cpp: rc1.0_tokens.l
	flex -Prc10_ -o_rc1.0_lexer.cpp rc1.0_tokens.l

_ts1.0_lexer.cpp: ts1.0_tokens.l
	flex -Pts10_ -o_ts1.0_lexer.cpp ts1.0_tokens.l

.PHONY: clean
clean:
	rm -f _ps1.0_parser.cpp _ps1.0_parser.h _ps1.0_lexer.cpp
	rm -f _rc1.0_parser.cpp _rc1.0_parser.h _rc1.0_lexer.cpp
	rm -f _ts1.0_parser.cpp _ts1.0_parser.h _ts1.0_lexer.cpp
	rm -f $(OBJS)

.PHONY: distclean
distclean: clean
	rm -f $(MAIN)
