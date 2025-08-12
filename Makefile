CC = gcc
CFLAGS = -Wall -g -Iinclude
LDFLAGS = -llua5.4 -lreadline -lm -ldl
SRCDIR = src
OBJDIR = build
BINDIR = build
OBJS = $(OBJDIR)/shell.o $(OBJDIR)/lua_integration.o $(OBJDIR)/helpers.o $(OBJDIR)/main.o
TARGET = $(BINDIR)/lushell

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJDIR)/*.o $(TARGET)

run: $(TARGET)
	$(TARGET)

install: $(TARGET)
	install -d /usr/local/bin
	install $(TARGET) /usr/local/bin/lushell

uninstall:
	rm -f /usr/local/bin/lushell
	
.PHONY: clean run all install uninstall
