//
//  Created by João Cardoso
//
//

#ifndef __pbrt__projectAmbient__
#define __pbrt__projectAmbient__
#include "pbrt.h"
#include "renderer.h"

class ProjectAmbient : public Renderer {
public:
  ProjectAmbient(const ParamSet&);
  
  void Render(const Scene *scene);
  Spectrum Li(const Scene*, const RayDifferential&, const Sample*, RNG&, MemoryArena&, Intersection*, Spectrum*) const;
  Spectrum Transmittance(const Scene*, const RayDifferential&, const Sample*, RNG&, MemoryArena&) const;
  
private:
  int harmonicLevel;
  FILE* file;
};

#endif
