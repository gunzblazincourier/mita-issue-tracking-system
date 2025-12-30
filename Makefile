

# Top-level Makefile: delegate to subdirectory Makefiles

SUBDIRS := userServer issueServer

.PHONY: all $(SUBDIRS) clean help servers style coverage docs

servers: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ server

style:
	@for d in $(SUBDIRS); do \
		$(MAKE) -C $$d style || exit $$?; \
	done

coverage:
	@for d in $(SUBDIRS); do \
		$(MAKE) -C $$d coverage || exit $$?; \
	done
	@mkdir -p coverage
	@if [ -d userServer/coverage ]; then \
		cp -r userServer/coverage/* coverage/ || true; \
	fi
	@if [ -d issueServer/coverage ]; then \
		cp -r issueServer/coverage/* coverage/ || true; \
	fi
	@test -f coverage/index.html || echo "No coverage data (subprojects)" > coverage/index.html

.PHONY: docs
docs:
	@for d in $(SUBDIRS); do \
		$(MAKE) -C $$d docs || exit $$?; \
	done
	@mkdir -p docs/code/html
	@if [ -d userServer/docs/code/html ]; then \
		cp -r userServer/docs/code/html/* docs/code/html/ || true; \
	fi
	@if [ -d issueServer/docs/code/html ]; then \
		cp -r issueServer/docs/code/html/* docs/code/html/ || true; \
	fi
	@test -f docs/code/html/index.html || echo "<html><body><h1>Docs</h1><p>No doxygen output from subprojects.</p></body></html>" > docs/code/html/index.html

clean:
	@for d in $(SUBDIRS); do \
		$(MAKE) -C $$d clean || exit $$?; \
	done

help:
	@echo "Available targets:"; \
	echo "  clean   - run 'make clean' in each subproject"; \
	echo "  servers - alias for all"
