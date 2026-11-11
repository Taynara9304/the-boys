# makefile The Boys
# Carlos Maziero - DINF/UFPR, 2024/2

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -g -std=c99
LDLIBS  = -lm
MAIN    = theboys
ENTREGA = $(MAIN)

# ajuste para os cabeçalhos que existem no seu projeto
HDR = entidades.h universo.h simulacao.h eventos.h fila.h fprio.h conjunto.h

# conjunto.o fica fora para o "make clean" não apagá-lo
OBJS = $(MAIN).o universo.o simulacao.o eventos.o fila.o fprio.o

$(MAIN): $(OBJS) conjunto.o
	$(CC) $(OBJS) conjunto.o $(LDLIBS) -o $(MAIN)

# todo objeto é refeito se algum cabeçalho mudar
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