SOURCE=src/lua2exe.c
TARGET_DIR=build
TARGET=$(TARGET_DIR)/lua2exe
LIBS=-Wl,-static -llua -Wl,-Bdynamic -lm -ldl
#LIBS=-llua

all:
	mkdir -p $(TARGET_DIR)
	gcc $(SOURCE) -o $(TARGET) $(LIBS)
	gcc $(SOURCE) -DLUA2EXE_FILE_SIZE=`stat -c %s $(TARGET)` -o $(TARGET) $(LIBS)

clean:
	rm $(TARGET)

.PHONY: all clean
