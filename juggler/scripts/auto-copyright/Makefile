
CPP_EXTS = java,h.in,h,cxx,cpp
HASH_EXTS = in,pl,sh

default: usage
usage:
	@echo ""
	@echo "Usage:"
	@echo "     make [all|aj|aj-head]"
	@echo ""

# Juggler Umbrella
all:
	auto-copyright.pl -d ../../modules -e $(CPP_EXTS) -c data/vrj-copyright.c.txt  -t config/vrj-copyright-replace.c.pl -a
	auto-copyright.pl -d ../../modules -e $(HASH_EXTS) -c data/vrj-copyright.hash.txt -t config/vrj-copyright-replace.hash.pl -a

# AudioJuggler
aj:
	auto-copyright.pl -d ../../modules/AudioJuggler -e $(CPP_EXTS) -c data/vrj-copyright.c.txt  -t config/vrj-copyright-replace.c.pl -a
	auto-copyright.pl -d ../../modules/AudioJuggler -e $(HASH_EXTS) -c data/vrj-copyright.hash.txt -t config/vrj-copyright-replace.hash.pl -a

aj-head:
	auto-copyright.pl -d ../../modules/AudioJuggler -e $(CPP_EXTS) -c data/aj-heading.c.txt  -t config/aj-heading-replace.c.pl -a
	auto-copyright.pl -d ../../modules/AudioJuggler -e $(HASH_EXTS) -c data/aj-heading.hash.txt -t config/aj-heading-replace.hash.pl -a
