
CC = clang
LIBS = -lm
FLAGS = -O2
EXEC = decision_tree
SRCFILES = ssv.c utils.c entropy.c decision_tree.c
OBJFILES = ssv.o utils.o entropy.o decision_tree.o

all: $(EXEC)
	@echo ""
	@echo "Compilation Done"
	@echo "Pruning: " $(POST_PRUNING)
	@echo ""

clean:
	rm -f *.o *~ $(EXEC)

$(EXEC): $(OBJFILES)
	$(CC) $(POST_PRUNING) -g -o $(EXEC) $(OBJFILES) $(LIBS)

%.o: %.c
	$(CC) -c $(POST_PRUNING) -g -o $(*F).o $(FLAGS) $(*F).c
