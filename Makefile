all: server client loop loop_forever loop_process server_vm client_vm loop_profiling

objdir=./obj


server: $(objdir)/server.o
	$(CC) -o $@ $<

client: $(objdir)/client.o
	$(CC) -o $@ $<

server_vm: $(objdir)/server_vm.o
	$(CC) -o $@ $<

client_vm: $(objdir)/client_vm.o
	$(CC) -o $@ $<

loop: $(objdir)/loop.o
	$(CC) -o $@ $< -lpthread

loop_profiling: $(objdir)/loop_profiling.o
	$(CC) -o $@ $< -lpthread

loop_forever: $(objdir)/loop_forever.o
	$(CC) -o $@ $<

loop_process: $(objdir)/loop_process.o
	$(CC) -o $@ $<

$(objdir)/%.o: %.c
	if [ ! -d $(objdir) ] ; then mkdir $(objdir); fi
	$(CC) -c -o $@ $<

clean:
	if [ -d $(objdir) ] ; then rm -r $(objdir); fi
	rm server client loop loop_forever loop_process 

plot: plot/*.plt log_time_cdf/*.log
	gnuplot plot/*.plt