file(GLOB LOCAL_SOURCE "*.h" "*.cpp")
SET(SOURCES
        ${SOURCES}
        ${LOCAL_SOURCE}
        PARENT_SCOPE)