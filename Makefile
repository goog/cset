TARGET=set_test

$(TARGET):set.c jhash.c hash_func.c
	gcc -o $@ $^


install:
	#cp -f $(TARGET)  /usr/local/bin/   

clean:
	rm -rf $(TARGET) *.o
