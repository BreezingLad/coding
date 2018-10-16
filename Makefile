
ROOTDIR ?= ..
MAKE=make

all:
	$(MAKE) -C ./cycle_queue
	$(MAKE) -C ./hashtable
	

