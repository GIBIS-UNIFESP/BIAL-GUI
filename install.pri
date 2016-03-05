unix{
  #VARIABLES
  isEmpty(PREFIX) {
    PREFIX = /usr/local
  }
  BINDIR = $$PREFIX/bin
  DATADIR =$$PREFIX/share

  DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"

  #MAKE INSTALL
  INSTALLS += target desktop icon26 icon32 icon48 icon64 icon128

  target.path =$$BINDIR

  desktop.path = $$DATADIR/applications
  desktop.files += $${PWD}/qrs/bial.desktop
  desktop.extra += desktop-file-install $${PWD}/qrs/bial.desktop --dir=$${DATADIR}/applications &&
  desktop.extra += xdg-mime install --mode system $${PWD}/qrs/bial-nii.xml

  icon128.path = $$DATADIR/icons/hicolor/128x128/apps
  icon128.files += $${PWD}/qrs/128x128/bial.png

  icon64.path = $$DATADIR/icons/hicolor/64x64/apps
  icon64.files += $${PWD}/qrs/64x64/bial.png

  icon48.path = $$DATADIR/icons/hicolor/48x48/apps
  icon48.files += $${PWD}/qrs/48x48/bial.png

  icon32.path = $$DATADIR/icons/hicolor/32x32/apps
  icon32.files += $${PWD}/qrs/32x32/bial.png

  icon26.path = $$DATADIR/icons/hicolor/26x26/apps
  icon26.files += $${PWD}/qrs/26x26/bial.png

}
