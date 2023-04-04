
.PHONY: all
all: version pyc

.PHONY : version
version:
	echo "version='$(shell git describe --tags --dirty)'" > python/samweb_client/_version.py

.PHONY : pyc
pyc: clean
	/usr/bin/python -m compileall python/

.PHONY : dist
dist: all
	tar -czf dist.tar.gz LICENSE RELEASE_NOTES bin python util ups
	@if ! git diff-index --quiet HEAD; then echo "Warning! Uncommitted changes in working directory"; false; \
	elif ! git describe --tags --exact-match >& /dev/null; then echo "Warning! Tarball represents untagged version"; false; fi;

.PHONY: test
test:
	@echo Running tests with requests handler
	@SAMWEB_HTTP_HANDLER=requests test/testsuite.py
	@echo Running tests with urllib handler
	@SAMWEB_HTTP_HANDLER=urllib test/testsuite.py

.PHONY: clean
clean:
	find python -depth -name "*.pyc" -o -name "__pycache__" | xargs -r rm -r
	rm -f dist.tar.gz
