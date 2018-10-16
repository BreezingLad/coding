
sem_perf_obj=CycleQueue.o testPerformance.o
cond_perf_obj=./cond/CycleQueue.o testPerformance.o

sem_perf: $(sem_perf_obj)
	g++ -o -O3 perf $(sem_perf_obj) -lpthread

cond_perf: $(cond_perf_obj)
	g++ -o -O3 perf $(cond_perf_obj) -ipthread 

clean:
	rm -f *.o
