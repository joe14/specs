########################################################################
#
# Rules to build VSIPL++ specifications.
#
########################################################################

########################################################################
# Variables
########################################################################

vsipl_dda_rfc_src:= vsipl_dda_rfc.xml dda.xml

src:=$(vsipl_dda_rfc_src)

css:= ../css/style.css
xsl:= ../xsl

docs:= vsipl_dda_rfc
xhtml_xref:= vsipl_dda_rfc.xhtml.xref
fo_xref:= vsipl_dda_rfc.fo.xref
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

vsipl_dda_rfc.xhtml.xref: $(vsipl_dda_rfc_src)
vsipl_dda_rfc.fo.xref: $(vsipl_dda_rfc_src)

$(xhtml): $(xhtml_xref)
$(fo): $(fo_xref)

vsipl_dda_rfc: vsipl_dda_rfc/index.xhtml
