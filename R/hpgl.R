devSVG <- function (file = "Rplots.hpgl", width = 10, height = 8,
                    bg = "white", fg = "black", onefile = TRUE, xmlHeader = TRUE)
{
  dev <- .C("do_HPGL", as.character(file),
            as.character(bg),as.character(fg),
            as.double(width),as.double(height),
            as.logical(FALSE), as.logical(xmlHeader),
            as.logical(onefile),PACKAGE="RHPGL")

  invisible(dev)
}

.onLoad <- function(libname, pkgname)
  library.dynam("RHPGL", pkgname, libname)
