CC = g++
PCH = pch.h
EXECUTABLE = vtHyperNEAT
IDIR = -I/usr/include -Iinclude
LDIR = -L/usr/lib -Llib
CFLAGSD = -c -std=c++14 -ggdb -O0 -Wall $(IDIR)
CFLAGSR = -c -std=c++14 -O3 $(IDIR)
CFLAGS = $(CFLAGSR)
LDFLAGS = $(LDIR)

OBJDIR = bin
EXEC = $(OBJDIR)/$(EXECUTABLE)
PCHC = $(OBJDIR)/$(PCH).gch

SRCDIR = source
OBJECTS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(wildcard $(SRCDIR)/*.cpp)))



.PHONY: all
all: $(OBJECTS) $(EXEC)

.PHONY: clean
clean:
	- rm $(EXEC) $(OBJECTS) $(PCHC)
	- rmdir $(OBJDIR)

.PHONY: print
print:
	@echo $(SRCDIR)
	@echo $(OBJECTS)



DIRGRD = @mkdir -p $(@D)

$(EXEC): $(OBJECTS) $(PCHC)
	$(DIRGRD)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(PCHC)
	$(DIRGRD)
	$(CC) $(CFLAGS) $< -o $@

$(PCHC): $(SRCDIR)/$(PCH)
	$(DIRGRD)
	$(CC) $(CFLAGS) $< -o $@
