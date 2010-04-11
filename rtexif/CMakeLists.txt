
add_library (rtexif STATIC rtexif.cc stdattribs.cc nikonattribs.cc canonattribs.cc
    pentaxattribs.cc fujiattribs.cc sonyminoltaattribs.cc olympusattribs.cc)

set_target_properties (rtexif PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math")
 