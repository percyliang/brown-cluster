# 1.2: need to make sure opt.o goes in the right order to get the right scope on the command-line arguments
# Use this for Linux
ifeq ($(shell uname),Linux)
	files=$(subst .cc,.o,basic/logging.cc $(shell /bin/ls *.cc) $(shell /bin/ls basic/*.cc | grep -v logging.cc))
else
	files=$(subst .cc,.o,basic/opt.cc $(shell /bin/ls *.cc) $(shell /bin/ls basic/*.cc | grep -v opt.cc))
endif

wcluster: $(files)
	g++ -Wall -g -o wcluster $(files)

%.o: %.cc
	g++ -Wall -g -o $@ -c $<

clean:
	rm wcluster basic/*.o *.o
