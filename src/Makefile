######## Name and directory configuration #########################################################
PROJ_NAME		:= $(shell cd ../; basename "$$PWD")
XENO_DIR		?= /opt/xenomai-2.6.3
PROJ_DIR		?= $(XENO_DIR)/app/$(PROJ_NAME)
INC_DIR			?= $(PROJ_DIR)/include
LIB_DIR			?= $(PROJ_DIR)/lib

######## Linux domain configuration ############################################################### 
USR_SRC0		?= manager.c
USR_OBJ0		?= $(USR_SRC0:%.c=%.o)
USR_APP0		?= $(USR_SRC0:%.c=%)

USR_SRC1		?= display.c
USR_OBJ1		?= $(USR_SRC1:%.c=%.o)
USR_APP1		?= $(USR_SRC1:%.c=%)

USR_SRC2		?= record.c
USR_OBJ2		?= $(USR_SRC2:%.c=%.o)
USR_APP2		?= $(USR_SRC2:%.c=%)

USR_CFLAGS 		?= -W -Wall 
USR_CPPFLAGS	?= -I$(INC_DIR)
USR_LDFLAGS		?= -L$(LIB_DIR)
USR_LDLIBS		?= -lpthread
USR_LDLIBS		+= -lerrors -lfilters

######## Xenomai user-domain configuration ########################################################
XENO_SRC		?= operator.c
XENO_OBJ		?= $(XENO_SRC:%.c=%.o)
XENO_APP		?= $(XENO_SRC:%.c=%)

XENO_CONFIG		?= $(XENO_DIR)/bin/xeno-config
XENO_CFLAGS		?= -D_GNU_SOURCE -D_REENTRANT -D__XENO__
XENO_CPPFLAGS	?= -I$(XENO_DIR)/include
XENO_LDFLAGS	?= -L$(XENO_DIR)/lib 
XENO_LDLIBS		?= -lnative -lrtdm -lxenomai -lpthread -lrt -lerrors
XENO_LOADLIBS	+= -Xlinker -rpath -Xlinker $(XENO_DIR)/lib

#XENO_CONFIG	= $(shell PATH=$(XENO_DIR):$(XENO_DIR)/bin:$(PATH) which xeno-config 2>/dev/null)
#XENO_CPPFLAGS	= $(shell $(XENO_CONFIG) --skin=native --cflags) $(MY_CFLAGS)
#XENO_LDFLAGS	= $(MY_LDFLAGS)
#XENO_LDLIBS	= $(shell $(XENO_CONFIG) --skin=native --ldflags)
#XENO_LOADLIBS	+= -Xlinker -rpath -Xlinker $(shell $(XENO_CONFIG) --libdir)

######## General configuration stuff ##############################################################
CC				= $(shell $(XENO_CONFIG) --cc)
RM				= rm

######## Start compiling ##########################################################################
all :: $(USR_APP0) $(USR_APP1) $(USR_APP2) $(XENO_APP)

$(USR_APP0) : $(USR_OBJ0)
	$(CC) $(USR_CFLAGS) $(XENO_CFLAGS) -o $@ $< $(USR_LDFLAGS) $(USR_LDLIBS)

$(USR_APP1) : $(USR_OBJ1)
	$(CC) $(USR_CFLAGS) $(XENO_CFLAGS) -o $@ $< $(USR_LDFLAGS) $(USR_LDLIBS)
	
$(USR_APP2) : $(USR_OBJ2)
	$(CC) $(USR_CFLAGS) $(XENO_CFLAGS) -o $@ $< $(USR_LDFLAGS) $(USR_LDLIBS)

$(XENO_APP) : $(XENO_OBJ)
	$(CC) $(XENO_CFLAGS) -o $@ $< $(XENO_LOADLIBS) $(XENO_CPPFLAGS) $(USR_CPPFLAGS) $(XENO_LDFLAGS)\
		$(USR_LDFLAGS) $(XENO_LDLIBS)

clean :
	$(RM) -f $(USR_APP0) $(USR_OBJ0) $(USR_APP1) $(USR_OBJ1) $(USR_APP2) $(USR_OBJ2) $(XENO_APP) $(XENO_OBJ) pdo.*

######## Suffix Rule ##############################################################################
.c.o:
	$(CC) -c -o $@ $< $(USR_CPPFLAGS) $(XENO_CPPFLAGS)

#gcc 																	CC
#-I/opt/xenomai-2.6.3/include -D_GNU_SOURCE -D_REENTRANT -D__XENO__		CPPFLAGS		
#operator.c																$(APPLICATIONS).c
#-Xlinker -rpath -Xlinker /opt/xenomai-2.6.3/lib/						LOADLIBES
#-lnative -L/opt/xenomai-2.6.3/lib -lxenomai -lpthread -lrt				LDLIBS
#-lrtdm -L/opt/xenomai-2.6.3/lib -lxenomai -lpthread -lrt 
#-o																		-o
#operator																$(APPLICATIONS)
