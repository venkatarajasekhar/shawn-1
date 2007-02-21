# - Find the curses include file and library
#

FIND_PATH(CURSES_INCLUDE_PATH 
  curses.h
  /usr/local/include 
  /usr/include
)

FIND_LIBRARY(CURSES_LIBRARY 
  NAMES curses ncurses
  PATHS 
    /usr/local/lib 
    /usr/lib 
    /lib
)

FIND_LIBRARY(CURSES_EXTRA_LIBRARY 
  cur_colr
  PATHS 
    /usr/local/lib 
    /usr/lib 
    /lib
)

FIND_LIBRARY(FORM_LIBRARY 
  form
  PATHS 
    /usr/local/lib 
    /usr/lib 
    /lib
)

# Need to provide the *_LIBRARIES
SET(CURSES_LIBRARIES ${CURSES_LIBRARY})

IF(CURSES_EXTRA_LIBRARY)
  SET(CURSES_LIBRARIES ${CURSES_LIBRARIES} ${CURSES_EXTRA_LIBRARY})
ENDIF(CURSES_EXTRA_LIBRARY)

IF(FORM_LIBRARY)
  SET(CURSES_LIBRARIES ${CURSES_LIBRARIES} ${FORM_LIBRARY})
ENDIF(FORM_LIBRARY)

# Proper name is *_INCLUDE_DIR
SET(CURSES_INCLUDE_DIR ${CURSES_INCLUDE_PATH})

MARK_AS_ADVANCED(
  CURSES_INCLUDE_PATH
  CURSES_LIBRARY
  CURSES_EXTRA_LIBRARY
  FORM_LIBRARY
  CURSES_LIBRARIES
  CURSES_INCLUDE_DIR
  )

