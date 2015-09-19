#include "displayformat.h"

DisplayFormat::DisplayFormat( Modality modality ) : modality( modality ) {
  switch( modality ) {
      case Modality::BW: {
      currentLayout = Layout::GRID;
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
      showPpmViews = true;
      showOrientation = false;
      showPpmChannels = true;
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
      showNiftiAxis = false;
      showOrientation = true;
      showPpmViews = false;
      showPpmChannels = false;
      break;
    }
  }
}
