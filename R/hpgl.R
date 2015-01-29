hpgl <- function (file = "Rplots.hpgl", width = 10, height = 8,
                  bg = "white", fg = "black") {
                 #units = c('mm', 'in', 'plotter-units')) {
  plotter.units <- 1
  millimeters <- 1/40
  inches <- 1/1016
  dev <- .C("do_HPGL", as.character(file),
            as.character(bg),as.character(fg),
            as.double(width),as.double(height),
            as.double(mm),PACKAGE="rhpgl")

  invisible(dev)
}

.onLoad <- function(libname, pkgname)
  library.dynam("rhpgl", pkgname, libname)
