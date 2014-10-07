include config.mk

###### STANDARD variables ######
OBJDIR=objs
SRCDIR=src
INCDIR=$(SRCDIR)/inc

CXX=g++

SRCS=$(SRCDIR)/pokerbot.cpp
OBJS=$(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

CXXFLAGS+=-std=c++0x -O2 -I$(INCDIR)
LDFLAGS+=

######## PYTHON variables ########
PYFLAGS+=-I$(PYTHON_DEV_DIR) -lpython$(PYTHON_VERSION)
# Name of the python module
PYLIB=pokerbot

ifeq ($(ARCH), Darwin)
PYFLAGS+=-dynamiclib
else
PYFLAGS+=-shared
endif

PYSRCS=$(SRCDIR)/python_wrap.cpp

###### TARGETS ######

all:	pokerbot

run:	pokerbot
	./pokerbot

# Python stuff is not strictly necessary but if you want it
# to work, edit the config.mk file.
python: $(OBJS)
	$(CXX) $(PYFLAGS) $(CXXFLAGS) $(LDFLAGS) $^ $(PYSRCS) -o $(PYLIB).so

pokerbot: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

$(OBJS): | $(OBJDIR)
$(OBJDIR):
	mkdir -p $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INCDIR)/*.h Makefile
	$(CXX) $(CXXFLAGS) $< -c -o $@

clean:
	rm -rf $(OBJDIR) pokerbot *.so
