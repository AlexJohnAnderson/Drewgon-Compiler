TESTFILES := $(wildcard **/*.dg) $(wildcard *.dg)
TESTS := $(TESTFILES:.dg=.test)

.PHONY: all

all: $(TESTS)

%.test:
	@echo "Testing $*.dg"
	@touch $*.err #The @ means don't show the command being invoked
	@../dgc $*.dg -c 2> $*.err ;\
	PROG_EXIT_CODE=$$?;\
	echo "diff error...";\
	diff $*.err $*.err.expected;\
	ERR_EXIT_CODE=$$?;\
	exit $$ERR_EXIT_CODE

clean:
	rm -f *.out *.err
