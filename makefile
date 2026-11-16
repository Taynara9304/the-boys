# makefile The Boys
# Carlos Maziero - DINF/UFPR, 2024/2

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -g -std=c99
LDLIBS  = -lm
MAIN    = theboys
ENTREGA = $(MAIN)

HDR = entidades.h eventos.h universo.h simulacao.h funcoes_auxiliares.h fila.h fprio.h conjunto.h

OBJ = fila.o fprio.o eventos.o universo.o simulacao.o funcoes_auxiliares.o theboys.o

$(MAIN): $(OBJ) conjunto.o

eventos.o: eventos.c $(HDR)
universo.o: universo.c $(HDR)
simulacao.o: simulacao.c $(HDR)
funcoes_auxiliares.o: funcoes_auxiliares.c $(HDR)

$(OBJS): $(HDR)

run: $(MAIN)
	./$(MAIN)

valgrind: $(MAIN)
	valgrind --leak-check=full --track-origins=yes ./$(MAIN)

tgz: clean
	-mkdir -p /tmp/$(USER)/$(ENTREGA)
	chmod 0700 /tmp/$(USER)/$(ENTREGA)
	cp *.c *.h conjunto.o makefile /tmp/$(USER)/$(ENTREGA)
	tar czvf $(ENTREGA).tgz -C /tmp/$(USER) $(ENTREGA)
	rm -rf /tmp/$(USER)
	@echo "Arquivo $(ENTREGA).tgz criado para entrega"

clean:
	rm -f *~ $(OBJS) $(MAIN) $(ENTREGA).tgz