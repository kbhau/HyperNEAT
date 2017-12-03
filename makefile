CC = g++
PCH = pch.h
EXECUTABLE = vtHyperNEAT
LDIR = -L/usr/lib -Llib
LDFLAGS = $(LDIR)

OBJDIR = bin
EXEC = $(OBJDIR)/$(EXECUTABLE)
PCHC = $(OBJDIR)/$(PCH).gch



SOURCES = \
	$(wildcard source/*.cpp) \
	$(wildcard source/*/*.cpp) \
	$(wildcard source/*/*/*.cpp)
OBJECTS = \
	$(patsubst %.cpp, \
	$(OBJDIR)/%.o, \
	$(notdir $(SOURCES)))
VPATH = $(sort $(dir $(SOURCES)))
IDIR = $(addprefix -I, $(VPATH))
CFLAGSD = -c -std=c++14 -O0 -Wall -ggdb $(IDIR)
CFLAGSR = -c -std=c++14 -O3 -Wno-sign-compare $(IDIR)
CFLAGS = $(CFLAGSR)



.PHONY: all
all: $(OBJECTS) $(EXEC)

.PHONY: clean
clean:
	- rm -f $(EXEC) $(OBJECTS) $(PCHC)
	rmdir $(OBJDIR)

.PHONY: print
print:
	@echo $(SOURCES)
	@echo $(OBJECTS)
	@echo $(VPATH)



DIRGRD = @mkdir -p $(@D)

$(EXEC): $(OBJECTS) $(PCHC)
	$(DIRGRD)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp $(PCHC)
	$(DIRGRD)
	$(CC) $(CFLAGS) $< -o $@

$(PCHC): $(PCH)
	$(DIRGRD)
	$(CC) $(CFLAGS) $< -o $@
