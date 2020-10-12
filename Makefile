include makefile.conf

all: $(TARGET)

$(TARGET): $(JAVLTREE_OBJS)
	$(AR) $@ $^

clean:
	$(RM) $(JAVLTREE_OBJS)
	$(RM) $(TARGET)

