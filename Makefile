.PHONY: install example

example: install
	R -e 'library(rhpgl); hpgl("/tmp/iris.hpgl"); plot(iris); dev.off()'

install:
	R -e 'library(devtools); install()'
