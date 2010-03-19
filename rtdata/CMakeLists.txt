
file (GLOB IMAGEFILES "images/*.png")
file (GLOB PROFILEFILES "profiles/*.pp2")
# unfortunatelly GLOB * matches directories too which breas cmake build from
# sources handled by subversion, so better stick with manual list for these
#file(GLOB LANGUAGEFILES "languages/*")
#file(GLOB THEMEFILES "themes/*")
set (LANGUAGEFILES languages/catala
    "languages/chinese simplified"
    "languages/chinese traditional"
    languages/czech
    languages/dansk
    languages/deutsch
    languages/english-uk
    languages/english-us
    languages/espanol
    languages/euskara
    languages/francais
    languages/greek
    languages/hebrew
    languages/italian
    languages/japanese
    languages/latvian
    languages/magyar
    languages/nederlands
    languages/norsk-bm
    languages/polish
    languages/russian
    languages/slovak
    languages/suomi
    languages/swedish
    languages/turkish)
set (THEMEFILES themes/dark
    themes/default
    themes/gray
    themes/light
    themes/test)

install (FILES ${IMAGEFILES} DESTINATION ${DATADIR}/images)
install (FILES ${LANGUAGEFILES} DESTINATION ${DATADIR}/languages)
install (FILES ${PROFILEFILES} DESTINATION ${DATADIR}/profiles)
install (FILES ${THEMEFILES} DESTINATION ${DATADIR}/themes)
