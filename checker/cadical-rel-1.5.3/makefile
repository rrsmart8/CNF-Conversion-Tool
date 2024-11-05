CADICALBUILD=/Users/rares/Documents/Facultate/Anul 2/AA/tema2/archive-2/cadical-rel-1.5.3/build
all:
	$(MAKE) -C "$(CADICALBUILD)"
clean:
	@if [ -d "$(CADICALBUILD)" ]; \
	then \
	  if [ -f "$(CADICALBUILD)"/makefile ]; \
	  then \
	     touch "$(CADICALBUILD)"/build.hpp; \
	     $(MAKE) -C "$(CADICALBUILD)" clean; \
	  fi; \
	  rm -rf "$(CADICALBUILD)"; \
	fi
	rm -f "/Users/rares/Documents/Facultate/Anul 2/AA/tema2/archive-2/cadical-rel-1.5.3/src/makefile"
	rm -f "/Users/rares/Documents/Facultate/Anul 2/AA/tema2/archive-2/cadical-rel-1.5.3/makefile"
test:
	$(MAKE) -C "$(CADICALBUILD)" test
cadical:
	$(MAKE) -C "$(CADICALBUILD)" cadical
mobical:
	$(MAKE) -C "$(CADICALBUILD)" mobical
update:
	$(MAKE) -C "$(CADICALBUILD)" update
.PHONY: all cadical clean mobical test update
