hpgl <- function (file = "Rplots.hpgl", width = 10, height = 8,
                  bg = "white", fg = "black",
                  ip = c(0, 8.5, 11),
                  units = c('mm', 'in', 'plotter-units')) {
  # Convert from R units to plotter units.
  ipr <- c('plotter-units'=1, mm=1/40, 'in'=1/1016)[units[1]]
                   
  ip <- paste0('IP', paste(ip, sep = ','), ';')
  sc <- paste0('SC', paste(c(0, width, height), sep = ','), ';')
  dev <- .C("do_HPGL", as.character(file),
            as.character(bg),as.character(fg),
            as.double(width),as.double(height),
            as.double(ipr),
            as.character(ip), as.character(sc),
            PACKAGE="rhpgl")

  invisible(dev)
}

.onLoad <- function(libname, pkgname)
  library.dynam("rhpgl", pkgname, libname)
