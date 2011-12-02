# - Try to find the GeoIP C Library from (http://www.maxmind.com)
# Once done this will define
#
#  GEOIP_FOUND         - system has GeoIP
#  GEOIP_LIBRARIES     - Link these to use GeoIP
#  GEOIP_INCLUDE_DIR   - Include directory for using GeoIP
#  GEOIP_DEFINITIONS   - Add Simple -DHAVE_GEOIP definition
#  GEOIP_DATABASE_PATH - Path to GeoIP Datbases Default:/usr/share/GeoIP/

# $GEOIPDIR is an environment variable that would
# correspond to the ./configure --prefix=$GEOIPDIR
# used in building GeoIP.
#
# (c) 2007-2012 Juergen Heinemann (Undefined) http://www.hjcms.de
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

MESSAGE (STATUS "Looking for GeoIP")

FIND_PATH(GEOIP_INCLUDE_DIR GeoIP.h
  HINTS
  $ENV{GEOIPDIR}
  PATH_SUFFIXES include/GeoIP include/libGeoIP include/geoip include/libgeoip include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MaxMind\\GeoIP]"
)

FIND_LIBRARY(GEOIP_LIBRARIES
  NAMES GeoIP libGeoIP libGeoIP32 libGeoIP64
  HINTS
  $ENV{GEOIPDIR}
  PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MaxMind\\GeoIP]"
)

FIND_PATH(GEOIP_DATABASE_PATH GeoIP.dat
  HINTS
  $ENV{GEOIPDIR}
  $ENV{GEOIP_DBBASE_DIR}
  PATH_SUFFIXES lib/GeoIP lib/geoip share/GeoIP share/geoip share/misc
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /var
  /var/lib
  /var/spool
  /usr/local
  /usr/local/share
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MaxMind\\GeoIP]"
)

SET (GEOIP_DEFINITIONS "")
SET (GEOIP_FOUND "NO")

IF (GEOIP_LIBRARIES AND GEOIP_INCLUDE_DIR)
  SET (GEOIP_FOUND "YES")
  SET (GEOIP_DEFINITIONS ${GEOIP_DEFINITIONS} -DHAVE_GEOIP)
  MESSAGE (STATUS " Found GeoIP Library - ${GEOIP_LIBRARIES}; includes - ${GEOIP_INCLUDE_DIR}; databases - ${GEOIP_DATABASE_PATH}")
ELSE (GEOIP_LIBRARIES AND GEOIP_INCLUDE_DIR)
  IF (GeoIP_FIND_REQUIRED)
    MESSAGE (FATAL_ERROR " Could NOT find GeoIP Development Library")
  ENDIF (GeoIP_FIND_REQUIRED)
ENDIF(GEOIP_LIBRARIES AND GEOIP_INCLUDE_DIR)

MARK_AS_ADVANCED (
  GEOIP_INCLUDE_DIR
  GEOIP_LIBRARIES
  GEOIP_DATABASE_PATH
  GEOIP_DEFINITIONS
)
