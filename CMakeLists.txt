cmake_minimum_required ( VERSION 2.6 )

# set BIN variable value
set ( BIN gSimPIDBench )

project ( ${BIN} )

# source code folder path... all finded source files will be saved in SUORCES variable
set ( SOURCE_ROOT ./src )

aux_source_directory( ${SOURCE_ROOT} SOURCES )

include_directories ( "/usr/include/gtk-3.0"
		      "/usr/include/at-spi2-atk/2.0"
		      "/usr/include/gtk-3.0"
		      "/usr/include/gio-unix-2.0/"
		      "/usr/include/cairo"
		      "/usr/include/pango-1.0"
		      "/usr/include/atk-1.0"
		      "/usr/include/cairo"
		      "/usr/include/pixman-1"
		      "/usr/include/freetype2"
		      "/usr/include/libpng16"
		      "/usr/include/harfbuzz"
		      "/usr/include/freetype2"
		      "/usr/include/harfbuzz"
		      "/usr/include/libdrm"
		      "/usr/include/libpng16"
		      "/usr/include/gdk-pixbuf-2.0"
		      "/usr/include/libpng16"
		      "/usr/include/glib-2.0"
		      "/usr/lib/glib-2.0/include" 
		      "${SOURCE_ROOT}" )

# installation path
set( INSTALL_PATH ~/bin/ )

# auxiliar link directories
# link_directories ( "path_to_libs" )

# used libs
set( REQUIRED_LIBRARIES
		      gtk-3
		      gdk-3
		      pangocairo-1.0
		      pango-1.0
		      atk-1.0
		      cairo-gobject
		      cairo
		      gdk_pixbuf-2.0
		      gio-2.0
		      gobject-2.0
		      glib-2.0
		      m 
		      mgl )

# setting compiler flags
set( CMAKE_CXX_FLAGS "-std=c99 -Wall -pedantic --cflags --libs" )

# name of exacutable BIN from SOURCES
add_executable( ${BIN} ${SOURCES} )

target_link_libraries( ${BIN} ${REQUIRED_LIBRARIES} )

install( TARGETS ${BIN} DESTINATION ${INSTALL_PATH} )