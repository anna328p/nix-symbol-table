.PHONY = all clean

CFLAGS += $(shell pkg-config --cflags --libs nix-expr nix-main nix-store)

all: nix-symbol-table.so

%.so : %.cc
	$(CC) $(CFLAGS) -shared -fPIC $< -o $@

clean:
	rm -f nix-symbol-table.so
