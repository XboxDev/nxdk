SHADEROBJ = \
	$(CURDIR)/vs.inl \
	$(CURDIR)/ps.inl

INCLUDES = $(wildcard $(CURDIR)/*.h)
SRCS = $(wildcard $(CURDIR)/*.c)
NXDK_DIR = $(CURDIR)/../..
include $(NXDK_DIR)/Makefile

$(CURDIR)/graphics.obj : $(SHADEROBJ)