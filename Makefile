RM=rm -rf
CCw=i686-w64-mingw32-g++	#windows
# don t forget: export PKG_CONFIG_PATH=/opt/gtk+-3.5.4-1/lib/pkgconfig
CC=g++	#linux


WIN		= -mwindows
STD		= -std=c++11
COPT		= -export-dynamic
GTKCFLAGS       = `pkg-config --cflags gtk+-3.0`
GTKLDFLAGS      = `pkg-config --libs gtk+-3.0 gmodule-2.0`

EXECUTABLE = universalConverter
SOURCEDIR = src
BUILDDIR = build

SOURCES = $(wildcard $(SOURCEDIR)/*.cpp)
OBJECTS = $(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))
OBJECTSw = $(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.ow,$(SOURCES))

all: dir $(BUILDDIR)/$(EXECUTABLE)

dir: 
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ -o $@ $(STD) $(GTKCFLAGS) $(GTKLDFLAGS) $(COPT)

$(OBJECTS): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.cpp
	$(CC) $< -c -o $@ $(STD) $(GTKCFLAGS) $(GTKLDFLAGS) $(COPT)

win: dir $(BUILDDIR)/$(EXECUTABLE).exe

$(BUILDDIR)/$(EXECUTABLE).exe: $(OBJECTSw)
	$(CCw) $^ -o $@ $(STD) $(WIN) $(GTKCFLAGS) $(GTKLDFLAGS) $(COPT)
	$(unset $(PKG_CONFIG_PATH))

$(OBJECTSw): $(BUILDDIR)/%.ow : $(SOURCEDIR)/%.cpp
	$(CCw) $< -c -o $@ $(STD) $(GTKCFLAGS) $(GTKLDFLAGS) $(COPT)
	
partclean:
	$(RM) $(BUILDDIR)/*o $(BUILDDIR)/*ow

clean:
	$(RM) $(BUILDDIR)/*o $(BUILDDIR)/*ow $(BUILDDIR)/$(EXECUTABLE) $(BUILDDIR)/$(EXECUTABLE).exe
