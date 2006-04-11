
CPP_EXTS = java,h.in,h,cxx,cpp
HASH_EXTS = in,pl,sh

default: usage
usage:
	@echo ""
	@echo "Usage:"
	@echo "     make [all|snx|snx-head]"
	@echo ""

# Juggler Umbrella
all:
	auto-copyright.pl -d ../../modules -e $(CPP_EXTS) -c data/vrj-copyright.c.txt  -t config/vrj-copyright-replace.c.pl -a
	auto-copyright.pl -d ../../modules -e $(HASH_EXTS) -c data/vrj-copyright.hash.txt -t config/vrj-copyright-replace.hash.pl -a

# sonix
snx:
	auto-copyright.pl -d ../../modules/sonix -e $(CPP_EXTS) -c data/vrj-copyright.c.txt  -t config/vrj-copyright-replace.c.pl -a
	auto-copyright.pl -d ../../modules/sonix -e $(HASH_EXTS) -c data/vrj-copyright.hash.txt -t config/vrj-copyright-replace.hash.pl -a

snx-head:
	auto-copyright.pl -d ../../modules/sonix -e $(CPP_EXTS) -c data/snx-heading.c.txt  -t config/snx-heading-replace.c.pl -a
	auto-copyright.pl -d ../../modules/sonix -e $(HASH_EXTS) -c data/snx-heading.hash.txt -t config/snx-heading-replace.hash.pl -a
