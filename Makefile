OBJDIR=objs
SRCDIR=src
INCDIR=$(SRCDIR)/inc

CXX=g++

SRCS=\
	$(SRCDIR)/pokerbot.cpp

OBJS=$(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

CXXFLAGS+=-std=c++0x -O2 -I$(INCDIR)
LDFLAGS+=

all:	pokerbot

run:	pokerbot
	./pokerbot

pokerbot: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

$(OBJS): | $(OBJDIR)
$(OBJDIR):
	mkdir -p $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INCDIR)/*.h Makefile
	$(CXX) $(CXXFLAGS) $< -c -o $@

clean:
	rm -rf $(OBJDIR) pokerbot
