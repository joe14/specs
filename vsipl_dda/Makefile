########################################################################
#
# Rules to build VSIPL++ specifications.
#
########################################################################

########################################################################
# Variables
########################################################################

vsipl_dda_src:= vsipl_dda.xml dda.xml

src:=$(vsipl_dda_src)

css:= ../css/style.css
xsl:= ../xsl

docs:= vsipl_dda
xhtml_xref:= vsipl_dda.xhtml.xref
fo_xref:= vsipl_dda.fo.xref
xhtml:= $(docs)
fo:= $(patsubst %, %.fo, $(docs))
pdf:= $(patsubst %, %.pdf, $(docs))

targets:= $(pdf) $(xhtml)

include ../common.mk

all: xhtml pdf

xhtml: $(xhtml)
pdf: $(pdf)

########################################################################
# Dependencies
########################################################################

vsipl_dda.xhtml.xref: $(vsipl_dda_src)
vsipl_dda.fo.xref: $(vsipl_dda_src)

$(xhtml): $(xhtml_xref)
$(fo): $(fo_xref)

vsipl_dda: vsipl_dda/index.xhtml
