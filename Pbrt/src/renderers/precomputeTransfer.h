//
//  Created by João Cardoso
//
//

#ifndef pbrt_precomputeTransfer_h
#define pbrt_precomputeTransfer_h
#include "pbrt.h"
#include "renderer.h"
#include "geometry.h"
#include "rng.h"
#include <vector>

using std::vector;
enum PrecomputeTransferMode {
  Transfer,
  Visibility
};

class PrecomputeTransfer : public Renderer {
public:
  PrecomputeTransfer(const ParamSet&);
  ~PrecomputeTransfer();
  
  void Render(const Scene *scene);
  Spectrum Li(const Scene*, const RayDifferential&, const Sample*, RNG&, MemoryArena&, Intersection*, Spectrum*) const;
  Spectrum Transmittance(const Scene*, const RayDifferential&, const Sample*, RNG&, MemoryArena&) const;
  
private:
  friend class TransferTask;
  const Scene* scene;
  
  PrecomputeTransferMode mode;
  RNG rng;
  
  int harmonicLevel, numHarmonics;
  int numSamples;
  long startTime;

  vector<Spectrum*> harmonics;
  vector<Normal> normals;
  vector<Point> points;
  FILE* file;
};

#endif
