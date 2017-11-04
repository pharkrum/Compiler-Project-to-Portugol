#--------------------------------------------------------------------------------------------------------------
#
# Arquivo   : Makefile 
# Criado    : 2012-06-14
# Modificado: 2017-09-25
#
# Projeto: Analisador lexico de Portugol.
#
# Este Makefile destina-se a ser usado em Linux com 'gcc' e 'make' (padrão em muitas distribuições de Linux).
#
# Antes de compilar e testar seu código, certifique-se que este Makefile está na mesma pasta que:
# - Todo seu código fonte (arquivos .c, .h e quaisquer outros que não façam parte da biblioteca padrão)
# - Três arquivos de testes "teste-01.ptg", "teste-02.ptg" e "teste-03.ptg"
#   Crie esses arquivos com código Portugol de sua escolha para testar seu programa.
#   Na correção serão usados três arquivos criados pelo professor.
#
# Para compilar seu código, digite na linha de comando:
#   make Portugol
#
# Para testar seu código, digite na linha de comando:
#   make teste
#
# Para realizar as duas tarefas (compilar e testar) de uma única vez, digite na linha de comando:
#   make all
#
#--------------------------------------------------------------------------------------------------------------

#----------------------------------------------------------------------

#		**************************************
#		**************************************
#		***                                ***
#		***  NÃO MUDE NADA NESSE MAKEFILE  ***
#		***                                ***
#		**************************************
#		**************************************

#----------------------------------------------------------------------
# Set ANSI_STANDARD to 'yes' to use the "-ansi" flag.
# Set PEDANTIC to 'yes' to use the "-pedantic" flag; 
#   to be used in conjunction with ANSI_STANDARD.
# Set DEBUGGING_INFO  to 'yes' to generate symbolic info for debugging.
#----------------------------------------------------------------------

ANSI_STANDARD  = yes
PEDANTIC       = yes
DEBUGGING_INFO = no

#----------------------------------------------------------------------
# Set the shell, the C compiler and the rm shell command
#----------------------------------------------------------------------

SHELL = /bin/bash
CC = gcc
RM = rm -f

#----------------------------------------------------------------------
# GCC flags
#----------------------------------------------------------------------

ifeq ($(DEBUGGING_INFO), yes)
  DEBUG = -g3
else
  DEBUG = -O2
endif

ifeq ($(ANSI_STANDARD), yes)
  ifeq ($(PEDANTIC), yes)
    ANSI = -ansi -pedantic -std=c99
  else
    ANSI = -ansi -std=c99
  endif
else
  ANSI = -std=c99
endif

WARNINGS = -Wall -Wstrict-prototypes -Wmissing-prototypes -Wnested-externs 
FLAGS = $(DEBUG) $(ANSI) $(WARNINGS)
MATH = -lm

#----------------------------------------------------------------------
# All source and object files in the current directory
#----------------------------------------------------------------------

SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:%.c=%.o)

#----------------------------------------------------------------------
# The main make targets
#----------------------------------------------------------------------

BIN = Portugol
all: clean $(BIN) teste

#----------------------------------------------------------------------
#	Rule: Phony targets (to improve make's performance)
#----------------------------------------------------------------------

.PHONY: all teste clean

#----------------------------------------------------------------------
#	Rule: Default
#----------------------------------------------------------------------

.DEFAULT:
	@echo "Erro: nao ha regra para \"$@\" no Makefile"

#----------------------------------------------------------------------
#	Rule: Disable some suffix rules and other implicit rules
#----------------------------------------------------------------------

.SUFFIXES: .d .h .c .o				# Keep only these suffix rules
									# Disable these suffix rules:
%.o: %.cc %.C						#   C++            Compiler
%.o: %.p							#   Pascal         Compiler
%.o: %.r %.F %.f					#   Fortran/Ratfor Compiler
%.f: %.r %.F						#   Fortran/Ratfor Preprocessor
%.sym: %.def						#   Modula2	   Compiler
%.o: %mod							#   Modula2	   Compiler
%.c: %.y							#   Yacc
%.c: %.l							#   Lex
%.r: %.l							#   Lex
%.nl: %.c							#   Lint
%.dvi: %.tex						#   TeX and Web
%.tex: %.web						#   TeX and Web
%.tex: %.w %.ch						#   TeX and Web
%.p: %.web							#   TeX and Web
%.c: %.w 							#   TeX and Web
%.c: %.w %.ch						#   TeX and Web
%.dvi: %.texinfo %.texi %.txinfo	#   Texinfo and Info
%.info: %.texinfo %.texi %.txinfo	#   Texinfo and Info
%: %,v								#   RCS
%: RCS/%,v							#   RCS
%: s.%								#   SCCS
%: SCCS/s.%							#   SCCS

%: %.c
%: %.o
%: %.cc
%: %.C
%: %.p
%: %.r
%: %.F 
%: %.f
%: %.mod 
%: %.y 
%: %.l 
%: %.w
%: %.sh
%: %.ch

#----------------------------------------------------------------------
#	Rule: Update and include dependency files 
#----------------------------------------------------------------------

include $(SOURCES:.c=.dep)

%.dep: %.c 
	@echo "Atualizando \"$@\" ..."
	@$(SHELL) -ec '$(CC) -MM $< | \
	sed '\''s/$*.o/& $@/g'\'' > $@'

#----------------------------------------------------------------------
#	Rule: Make all object files
#----------------------------------------------------------------------

$(OBJECTS): %.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

#----------------------------------------------------------------------
#	Rule: Make the executable
#----------------------------------------------------------------------

$(BIN): $(OBJECTS)
	$(CC) $(FLAGS) $^ -o $@ $(MATH)

#----------------------------------------------------------------------
#	Rule: Run the executable with test files
#----------------------------------------------------------------------

teste: $(BIN) 
	./$(BIN) teste-01.ptg
	./$(BIN) teste-02.ptg
	./$(BIN) teste-03.ptg

#----------------------------------------------------------------------
#	Rule: Clear some stuff
#----------------------------------------------------------------------

clean:
	$(RM) *.err *tbl *.tok *.mtk *.txt *~ *.dep *.o $(BIN)

