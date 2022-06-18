EJECUTABLE = $(DIR_BIN)/bin
DIR_INC  = ./src/headers
DIR_OBJ = ./temp
DIR_BIN = ./bin
DIR_MAIN = ./src/sources
OBJETOS = $(DIR_OBJ)/main.o \
		  $(DIR_OBJ)/http.o \
		  $(DIR_OBJ)/veridic.o

CPPFLAGS = -std=c++20 -lcurl -I$(DIR_INC)  -I/opt/ssl/include/ -L/opt/ssl/lib/ -lcrypto -pthread
COMPILER = g++


$(EJECUTABLE) : $(OBJETOS)
	@mkdir -p $(DIR_BIN)
	@$(COMPILER) $(OBJETOS)  $(CPPFLAGS) -o $(EJECUTABLE)


$(DIR_OBJ)/%.o : $(DIR_MAIN)/**/%.cpp
	@mkdir -p $(DIR_OBJ)
	@$(COMPILER) -c -MD $(CPPFLAGS) $< -o $@
-include $(DIR_OBJ)/*.d


.PHONY: clean install
clean:
	@rm -r $(DIR_BIN) $(DIR_OBJ)

install:
	apt-get install openssl -y
	apt-get install libcurl4-openssl-dev -y