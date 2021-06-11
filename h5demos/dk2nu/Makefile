CXX = `root-config --cxx`
CXXFLAGS = `root-config --cflags` -fPIC -g
ROOTLIBS = `root-config --glibs`
SHARED = -shared
SRCS = src/dk2nu.cc src/dkmeta.cc DKDict.cxx
HDRS = include/dkmeta.h include/dk2nu.h include/dflt.h
HDR = include
PROGRAM = libdk.so
#LDFLAGS = "-Wl,-rpath,/Users/bashyala/cern_root/root/obj/lib/"
LDFLAGS = -Wl,-rpath,`root-config --glibs`
all: $(PROGRAM)

DKDict.cxx: $(HDRS) include/LinkDef.h
	@echo "Generating dictionary ..."
	@rootcint -f $@ -c -p $^

$(PROGRAM): $(SRCS)
	@echo "Building $(PROGRAM) ..."
	@rm -f $(PROGRAM)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -I$(ROOTSYS)/include -I$(HDR)  $(SHARED) -o $@ $^ $(ROOTLIBS)
	@echo "done"

clean:; @rm -rf core *~ DKDict* $(PROGRAM)
