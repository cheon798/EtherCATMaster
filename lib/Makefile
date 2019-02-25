######## Name and directory configuration #########################################################
PROJ_NAME			:= $(shell cd ../; basename "$$PWD")
XENO_DIR			?= /opt/xenomai-2.6.3
PROJ_DIR			?= $(XENO_DIR)/app/$(PROJ_NAME)
INC_DIR				?= $(PROJ_DIR)/include

######## Library configuration #################################################################### 
LIB_ERR_SRCS		?= errors.c
LIB_ERR_OBJS		?= $(LIB_ERR_SRCS:%.c=%.o)
LIB_ERR_NAME		?= $(LIB_ERR_SRCS:%.c=%)
LIB_ERR_FULL_NAME	?= lib$(LIB_ERR_NAME).a

LIB_FLT_SRCS 		?= filters.c
LIB_FLT_OBJS 		?= $(LIB_FLT_SRCS:%.c=%.o)
LIB_FLT_NAME 		?= $(LIB_FLT_SRCS:%.c=%)
LIB_FLT_FULL_NAME 	?= lib$(LIB_FLT_NAME).a

######## General configuration stuff ##############################################################
LR = ar
RANLIB = ranlib

######## Start compiling ##########################################################################
all :: $(LIB_ERR_FULL_NAME) $(LIB_FLT_FULL_NAME)

$(LIB_ERR_FULL_NAME) : $(LIB_ERR_OBJS)
	$(AR) rcv $@ $<
	$(RANLIB) $@

$(LIB_FLT_FULL_NAME) : $(LIB_FLT_OBJS)
	$(AR) rcv $@ $<
	$(RANLIB) $@

#$(LIB_FULL_NAMES) : $(LIB_OBJS)
#	$(AR) rcv $@ $(LIB_OBJS) 
#	$(RANLIB) $@

clean :
	rm -rf *.a *.o

#clean :
#	rm -rf $(LIB_FULL_NAMES) $(LIB_OBJS)

######## Suffix Rule ##############################################################################
.c.o:
	$(CC) -c -o $@ $< -I$(INC_DIR)
