hpgl <- function (file = "Rplots.hpgl", width = 10, height = 8,
                  bg = "white", fg = "black",
                  ip = c(0, 8.5, 11)) {
                 #units = c('mm', 'in', 'plotter-units')) {
  plotter.units <- 1
  millimeters <- 1/40
  inches <- 1/1016
  ip <- paste0('IP', paste(ip, sep = ','), ';')
  sc <- paste0('SC', paste(c(0, width, height), sep = ','), ';')
  dev <- .C("do_HPGL", as.character(file),
            as.character(bg),as.character(fg),
            as.double(width),as.double(height),
            as.double(mm),
            as.character(ip), as.character(sc),
            PACKAGE="rhpgl")

  invisible(dev)
}

.onLoad <- function(libname, pkgname)
  library.dynam("rhpgl", pkgname, libname)
