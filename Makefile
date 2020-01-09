CC=gcc

CFLAGS:=${CFLAGS} -Wall -g -Iinclude

TARGET = stm

ODIR = obj

_OBJ = argparse.o stmutil.o stm.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
LIBS = 

all: ${TARGET}

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

clean: 
	rm -f $(OBJS) $(TARGET)
