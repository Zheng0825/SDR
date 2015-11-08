SHELL = /bin/sh


# THESE VARIABLES MAY CHANGE BETWEEN PACKAGES

# Set 'package' to the name of your package.
package := hydra

# Set these variables appropriately.
top_builddir := ..
subdir := package

# Require Click prefix settings.
# Generally, you will provide a '--with-click=CLICKPREFIX' option, and set:
include /home/wkim/nfs/click-piggyback/click-build-1.5.0/trunk/../../click-install-1.5.0/share/click/config.mk

# Set 'MAKE_UPACKAGE', 'MAKE_KPACKAGE', and 'MAKE_BPACKAGE' to '0' or '1' 
# to determine whether to build userlevel, linuxmodule, and bsdmodule
# packages, respectively.
MAKE_UPACKAGE = 1
MAKE_KPACKAGE = 0
MAKE_BPACKAGE = 0

# Set 'STRIP_UPACKAGE' to 'strip -g' to strip debugging symbols from the
# userlevel package before installing.
#STRIP_UPACKAGE = $(STRIP) -g

# Set these variables as required.
DEFS = -DPACKAGE_NAME=\"click-hydra\" -DPACKAGE_TARNAME=\"click-hydra\" -DPACKAGE_VERSION=\"1.5.0\" -DPACKAGE_STRING=\"click-hydra\ 1.5.0\" -DPACKAGE_BUGREPORT=\"\"  $(CLICKDEFS)
INCLUDES = -I. -I$(top_builddir) -I$(srcdir) -I$(top_srcdir)/include $(CLICKINCLUDES)

# Set this variable to force 'click-elem2package' to include a header,
# such as your package's '<config.h>', if necessary.
#ELEM2PACKAGE_INCLUDES = -i'<config.h>'

# END OF VARIABLES THAT MAY CHANGE BETWEEN PACKAGES


# Everything below here will probably stay unchanged

srcdir = .
top_srcdir = ..

ifeq ($(CLICK_PACKAGE_MAKING),)
prefix = /home/wkim/nfs/click-piggyback/click-build-1.5.0/trunk/../../click-install-1.5.0
exec_prefix = ${prefix}
bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libdir = ${exec_prefix}/lib
mandir = ${prefix}/man
endif

include $(clickdatadir)/pkg-Makefile
