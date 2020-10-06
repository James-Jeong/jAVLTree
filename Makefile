include makefile.conf

all: $(TARGET)

$(TARGET): $(JAVLTREE_OBJS)
	$(AR) $@ $^

clean:
	$(RM) $(jAVLTREE_OBJS)
	$(RM) $(TARGET)

