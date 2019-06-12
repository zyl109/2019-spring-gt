EXEC:=main.out
OBJS:=fakemn
LIBS:=fake-mininet/lib
CXXFLAGS:= -std=c++11
ORIGIN:=origin_graph
RESULT:=result_path

all: $(EXEC)

# executable (using static library)
# need libfakemn.a (compile from the Makefile of project root)
$(EXEC): %.out: %.cc
	g++ -o $@ $< -I$(LIBS) -g3 -L. -l$(OBJS) $(CXXFLAGS)
	
# transform .dot into .png
png:
	dot $(ORIGIN).dot -T png -o $(ORIGIN).png
	dot $(RESULT).dot -T png -o $(RESULT).png

# reset & clean
clean: 
	rm -rf $(EXEC)