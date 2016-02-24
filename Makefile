.PHONY: install example simple iris

simple: install
	R -e 'library(rhpgl); hpgl("/tmp/simple.hpgl"); plot(0); dev.off()'

iris: install
	R -e 'library(rhpgl); hpgl("/tmp/iris.hpgl"); plot(iris); dev.off()'

install:
	R -e 'library(devtools); install()'
