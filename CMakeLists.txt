cmake_minimum_required ( VERSION 2.6 )

# set APP_NAME and APP_VERSION variables
set ( APP_NAME gSimPIDBench )
set ( APP_VERSION "0.1.1" )

project ( ${APP_NAME} )

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

# name of exacutable APP_NAME from SOURCES
add_executable( ${APP_NAME} ${SOURCES} )

target_link_libraries( ${APP_NAME} ${REQUIRED_LIBRARIES} )

install( TARGETS ${APP_NAME} DESTINATION ${INSTALL_PATH} )