
DIA?=   dia
RM?=  rm -f
SVGRASTER?= /home/vr/Juggler/docbook/batik-1.5/svgrasterizer


EPS_FIGURES = 

PNG_FIGURES = ClusterPlugin.png #\
	      BasicClusterJuggler.png

default: diagrams

diagrams: ${EPS_FIGURES} ${PNG_FIGURES}


# Image conversions -----------------------------------------------------------

%.eps: %.dia
	$(DIA) --export $@ $<

%.png: %.svg
	$(SVGRASTER) $< -d $@ 

#%.jpg %.jpeg: %.svg
#	$(RASTERIZER) -m jpg $<

clean:
	$(RM) ${EPS_FIGURES} ${PNG_FIGURES}

