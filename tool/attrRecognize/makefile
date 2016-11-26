.PHONY: all clean


# annotation
Release 	=  1
TARGET_PROG	:= main

#projet directory
DEBUG_DIR	:= ./Debug
RELEASE_DIR	:= ./Release
BIN_DIR		:= $(if $(Release), $(RELEASE_DIR), $(DEBUG_DIR))

#shell command
CC		:= gcc
CXX 	:= g++
RM		:= rm -rf
MKDIR	:= mkdir -p
SED		:= sed
MV		:= mv
CP		:= cp -r

#init sources & objects & depends
sources_all	:= $(shell find . -name "*.c" -o -name "*.cpp" -o -name "*.h")
sources_c	:= $(filter %.c, $(sources_all))
sources_cpp	:= $(filter %.cpp, $(sources_all))
sources_h	:= $(filter, %.h, $(sources_all))
objs		:= $(addprefix $(BIN_DIR)/,$(strip $(sources_cpp:.cpp=.o) $(sources_c:.c=.o)))
deps		:= $(addprefix $(BIN_DIR)/,$(strip $(sources_cpp:.cpp=.d) $(sources_c:.c=.d)))

#create directory
$(foreach dirname,$(sort $(dir $(sources_c) $(sources_cpp))),\
	$(shell $(MKDIR) $(BIN_DIR)/$(dirname)))

#compile & link variable
CFLAGS 	:= $(if $(DEBUG),-g -O, -O2)
CFLAGS	+= $(addprefix -I ,$(sort $(dir $(sources_h))))
CXXFLAGS = $(CFLAGS) `pkg-config --cflags opencv`
LDFLAGS	:=
LOADLIBS+= `pkg-config --libs opencv`
LDLIBS	+= #-lpthread -lmysqlclient
PKG_CONFIG_PATH := /usr/local/opencv3/lib/pkgconfig
LD_LIBRARY_PATH := /usr/local/opencv3/lib

#add vpath
vpath %.h $(sort $(dir $(sources_h)))
vpath %.c $(sort $(dir $(sources_c)))
vpath %.cpp $(sort $(dir $(sources_cpp)))

#generate depend files
#actually generate after object generated
ifneq "$(MAKECMDGOALS)" "clean"
	sinclude $(deps)
endif

#make-depend
define make-depend
$(RM) $1;										\
	$4 $(CFLAGS) -MM $2 | 							\
	$(SED) 's,\($(notdir $3)\): ,$3: ,' > $1.tmp;	\
	$(SED) -e 's/#.*//'								\
	-e 's/^[^:]*: *//'						\
	-e 's/ *\\$$//'							\
	-e '/^$$/ d'								\
	-e 's/$$/ :/' < $1.tmp >> $1.tmp;		\
	$(MV) $1.tmp $1;
endef

#rules to generate objects file
$(BIN_DIR)/%.o: %.c
	@$(call make-depend,$(patsubst %.o,%.d,$@),$<,$@,$(CC))
	$(CC) $(CFLAGS) -o $@ -c $<
	
$(BIN_DIR)/%.o: %.cpp
	@$(call make-depend,$(patsubst %.o,%.d,$@),$<,$@,$(CXX))
	$(CXX) $(CXXFLAGS) -o $@ -c $<

#add-target
define add-target
REAL_TARGET	+= $(BIN_DIR)/$1
$(BIN_DIR)/$1: $2
	$3 $(LDFLAGS) $$^ $(LOADLIBS) $(LDLIBS) -o $$@
endef

#call add-target
$(foreach targ,$(TARGET_PROG),$(eval $(call add-target,$(targ),$(objs),$(CXX))))

print:
	@echo system PKG_CONFIG_PATH=$(shell echo $$PKG_CONFIG_PATH)
	@echo make PKG_CONFIG_PATH=$(PKG_CONFIG_PATH)
	@echo system LD_LIBRARY_PATH=$(shell echo $$LD_LIBRARY_PATH)
	@echo make LD_LIBRARY_PATH=$(LD_LIBRARY_PATH)


all:$(REAL_TARGET) $(TARGET_LIBS)	

clean:
	$(RM) $(BIN_DIR)

