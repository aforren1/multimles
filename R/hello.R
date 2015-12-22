# Hello, world!
#
# This is an example function named 'hello'
# which prints 'Hello, world!'.
#
# You can learn more about package authoring with RStudio at:
#
#   http://r-pkgs.had.co.nz/
#
# Some useful keyboard shortcuts for package authoring:
#
#   Build and Reload Package:  'Ctrl + Shift + B'
#   Check Package:             'Ctrl + Shift + E'
#   Test Package:              'Ctrl + Shift + T'

#' Testing roxygen2 documentation. This is a title.
#'
#'Add a space to separate title from description body.
#' Rebuild documentation with devtools::document(). See http://r-pkgs.had.co.nz/man.html
#'@return A string.

hello <- function() {
  print("Hello, world!")
}

