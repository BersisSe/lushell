CC = gcc
CFLAGS = -Wall -g -Iinclude
LDFLAGS = -llua5.4 -lreadline -lm -ldl
SRCDIR = src
OBJDIR = build
BINDIR = build
OBJS = $(OBJDIR)/shell.o $(OBJDIR)/lua_integration.o $(OBJDIR)/helpers.o $(OBJDIR)/main.o
TARGET = $(BINDIR)/lushell

# Kullanıcı HOME'unu bul (sudo etkilenmesin)
USER_HOME := $(shell getent passwd $(shell logname) | cut -d: -f6)

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
	@echo "[*] Copying config to $(USER_HOME)/.lushell"
	mkdir -p $(USER_HOME)/.lushell
	cp -r .lushell/* $(USER_HOME)/.lushell/
	install -d /usr/local/bin
	install $(TARGET) /usr/local/bin/lushell
	@echo "[+] Installation complete!"

uninstall:
	@echo "[*] Removing binary..."
	rm -f /usr/local/bin/lushell
	@echo "[*] Removing config from $(USER_HOME)/.lushell"
	rm -rf $(USER_HOME)/.lushell
	@echo "[+] Uninstallation complete!"

.PHONY: clean run all install uninstall
