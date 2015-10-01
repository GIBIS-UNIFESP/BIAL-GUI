GDCM_BASEDIR = $$(GDCM_PATH)
if( exists( $${GDCM_BASEDIR} ) ){
    message(GDCM_BASEDIR found)

    QMAKE_CXXFLAGS += -DLIBGDCM

    INCLUDEPATH +=  $${GDCM_BASEDIR}

    LIBS        += -lgdcmMSFF \
                   -lgdcmDICT \
                   -lsocketxx \
                   -lgdcmCommon \
                   -lgdcmDSED \
                   -lgdcmIOD \
                   -lgdcmMEXD \
                   -lgdcmjpeg8 \
                   -lgdcmjpeg12 \
                   -lgdcmjpeg16 \
    #               -lgdcmzlib \
    #               -lgdcmopenjpeg \
    #               -lgdcmexpat \
    #               -lgdcmgetopt \
    #               -lgdcmcharls

} else{
    message("GDCM library not found!")
}
