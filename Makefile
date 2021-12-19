include config.mk
project_name = clalib
SRC = src
INC = inc
OBJ = obj
BIN = bin
libs = numctl

def: hard-rebuild
	@

options:
	@printf $(project_name)" make menu:\n"
	@printf "make [options]   :: print this menu\n"
	@printf "make build       :: compile "$(project_name)".c with libs files linked\n"
	@printf "make clean       :: removes files in bin/ folder\n"

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

libs: $(OBJ)/$(libs).o
	@echo "Libs files compiled!" $(libs_files)

$(BIN)/shared:
	mkdir -p $@

$(BIN)/shared/%.o: $(SRC)/%.c
	$(CC) -c -fPIC $< -o $@ $(CFLAGS)

$(BIN)/shared/libcla.so: $(libs:%=$(BIN)/shared/%.o)
	$(CC) -shared $^ -o $@ $(CFLAGS)

./libcla.so: $(BIN)/shared/libcla.so
	sudo cp $< /usr/lib/
	mv $< ./

shared: $(BIN)/shared $(BIN)/shared/libcla.so ./libcla.so
	@

build: shared
	$(CC) -o $(project_name) $(SRC)/$(project_name).c $(CFLAGS) -L. -lcla

clean-libs:
	-rm $(BIN)/shared/* 2> /dev/null

libs-recompile: clean-libs shared
	@

hard-rebuild: libs-recompile build
	@

.PHONY: options libs shared-libs shared build clean
