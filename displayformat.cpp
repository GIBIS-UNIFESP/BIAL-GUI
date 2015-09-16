#include "displayformat.h"

DisplayFormat::DisplayFormat( Modality modality ) : modality( modality ) {
  switch( modality ) {

      case Modality::BW: {
      currentLayout = Layout::NONE;
      currentViews = Views::SHOW0;
      viewerControls = false;
      enableTools = true;
      rotateAll = false;
      rotateSingle = true;

      showNiftiViews = false;
      showNiftiAxis = false;
      showOrientation = false;
      showPpmViews = false;
      showPpmChannels = false;

//      availbleViewOptions.append( Views::SHOW0 );
      break;
    }
      case Modality::RGB: {
      currentLayout = Layout::GRID;
      currentViews = Views::SHOW0;
      viewerControls = false;
      enableTools = false;
      rotateAll = true;
      rotateSingle = false;

      showNiftiViews = false;
      showNiftiAxis = false;
      showOrientation = true;
      showPpmViews = true;
      showPpmChannels = true;

//      availbleViewOptions = { Views::SHOW0,
//                              Views::SHOW1,
//                              Views::SHOW2,
//                              Views::SHOW3,
//                              Views::SHOW123,
//                              Views::SHOW0123 };
      break;
    }
      case Modality::NIfTI: {
      currentLayout = Layout::GRID;
      currentViews = Views::SHOW012;
      viewerControls = true;
      enableTools = true;
      rotateAll = true;
      rotateSingle = false;

      showNiftiViews = true;
      showNiftiAxis = true;
      showOrientation = true;
      showPpmViews = false;
      showPpmChannels = false;

//      availbleViewOptions = { Views::SHOW0,
//                              Views::SHOW1,
//                              Views::SHOW2,
//                              Views::SHOW3,
//                              Views::SHOW012 };
      break;
    }
      case Modality::NONE: {
      currentLayout = Layout::NONE;
      currentViews = Views::NONE;

      viewerControls = false;
      enableTools = false;
      rotateAll = false;
      rotateSingle = false;

      showNiftiViews = false;
      showNiftiAxis = false;
      showOrientation = false;
      showPpmViews = false;
      showPpmChannels = false;


      break;
    }
  }
}
