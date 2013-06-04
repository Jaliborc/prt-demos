//
//  Created by João Cardoso
//
//

#include "projectAmbient.h"
#include "paramset.h"
#include "spectrum.h"
#include "scene.h"
#include "sh.h"

ProjectAmbient::ProjectAmbient(const ParamSet& params) {
  file = fopen(params.FindOneString("filename", "untitled.radiance").c_str(), "w");
  harmonicLevel = params.FindOneInt("harmonicLevel", 4);
}

void ProjectAmbient::Render(const Scene* scene) {
  int numHarmonics = SHTerms(harmonicLevel);
  
  vector<Light*> lights = scene->lights;
  Spectrum* radiance = new Spectrum[numHarmonics];
  Spectrum* harmonics = new Spectrum[numHarmonics];
  BBox world = scene->WorldBound();
  Point center = .5f * world.pMin + .5f * world.pMax;
  RNG random;
  
  for (int i = 0; i < numHarmonics; i++)
    radiance[i] = 0;
  
  for (int l = 0; l < lights.size(); l++) {
    lights[l]->SHProject(center, 5e-4f, harmonicLevel, scene, false, 0, random, harmonics);
    
    for (int i = 0; i < numHarmonics; i++)
      radiance[i] += harmonics[i];
  }
  
  for (int i = 0; i < numHarmonics; i++)
    radiance[i].Write(file);
  
  delete radiance;
  delete harmonics;
  fclose(file);
}

Spectrum ProjectAmbient::Transmittance(const Scene*, const RayDifferential&, const Sample*, RNG&, MemoryArena&) const {return NULL;}
Spectrum ProjectAmbient::Li(const Scene*, const RayDifferential&, const Sample*, RNG&, MemoryArena&, Intersection*, Spectrum*) const {return NULL;}