#include "Geometry/CSCGeometryBuilder/src/CSCGeometryESModule.h"
#include "Geometry/CSCGeometryBuilder/src/CSCGeometryBuilderFromDDD.h"
#include "Geometry/CSCGeometry/interface/CSCChamberSpecs.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/ESProducer.h"

#include <memory>

using namespace edm;

CSCGeometryESModule::CSCGeometryESModule(const edm::ParameterSet & p){

  setWhatProduced(this);

  // Choose Trapezoidal or Radial StripTopology
  // Default is now to use radial strips not trapezoidal

  bool useRadialStrips =       p.getParameter<bool>("useRadialStrips");

  // Choose wire geometry modelling
  // We now _require_ some wire geometry specification in the CSCOrcaSpec.xml file
  // in the DDD Geometry.
  // Default as of transition to CMSSW is to use real values.
  // Alternative is to use pseudo-values which match reasonably closely
  // the calculated geometry values used up to and including ORCA_8_8_1.
  // (This was the default in ORCA.)

  bool useRealWireGeometry =   p.getParameter<bool>("useRealWireGeometry");

  // Suppress strips altogether in ME1a region of ME11?

  bool useOnlyWiresInME1a =    p.getParameter<bool>("useOnlyWiresInME1a");

  // Allow strips in ME1a region of ME11 but gang them?
  // Default is now to treat ME1a with ganged strips (e.g. in clusterizer)

  bool useGangedStripsInME1a = p.getParameter<bool>("useGangedStripsInME1a");

  if ( useGangedStripsInME1a ) useOnlyWiresInME1a = false; // override possible inconsistentcy

  // Feed these value to where I need them
  CSCChamberSpecs::setUseRadialStrips( useRadialStrips );
  CSCChamberSpecs::setUseRealWireGeometry( useRealWireGeometry );
  CSCChamberSpecs::setOnlyWiresInME1a( useOnlyWiresInME1a );
  CSCChamberSpecs::setGangedStripsInME1a( useGangedStripsInME1a );

}


CSCGeometryESModule::~CSCGeometryESModule(){}


boost::shared_ptr<CSCGeometry>
CSCGeometryESModule::produce(const MuonGeometryRecord & record) {
  edm::ESHandle<DDCompactView> cpv;
  record.getRecord<IdealGeometryRecord>().get(cpv);
  CSCGeometryBuilderFromDDD builder;
  return boost::shared_ptr<CSCGeometry>(builder.build(&(*cpv)));
}

DEFINE_FWK_EVENTSETUP_MODULE(CSCGeometryESModule)
