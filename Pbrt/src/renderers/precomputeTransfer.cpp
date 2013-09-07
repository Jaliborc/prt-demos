//
//  Created by João Cardoso
//
//

#include "renderers/precomputeTransfer.h"
#include "parallel.h"
#include "paramset.h"
#include "shape.h"
#include "sh.h"
#include <sstream>
#include <time.h>

using std::stringstream;
string append(string text, int number) {
  stringstream stream;
  stream << text << number;
  return stream.str();
}

class TransferTask : public Task {
public:
  TransferTask(PrecomputeTransfer* parent, int start, int end) {
    this->parent = parent;
    this->start = start;
    this->end = end;
  }
  
  void Run() {
    for (int i = start; i < end; i++) {
      Point point = parent->points[i];
      Normal normal = parent->normals[i];
      Spectrum* harmonics = parent->harmonics[i];
      
      if (parent->mode == Visibility)
          SHComputeVisibility(point, 5e-2f, parent->scene, parent->rng, 512, parent->harmonicLevel, harmonics);
      else {
        SHComputeDiffuseTransfer(point, normal, 5e-2f, parent->scene, parent->rng, 512, parent->harmonicLevel, harmonics);
        
        for (int j = 0; j < parent->numHarmonics; j++)
          harmonics[j] /= M_PI;
      }
    }
  }
  
  PrecomputeTransfer* parent;
  int start, end;
};

PrecomputeTransfer::PrecomputeTransfer(const ParamSet& params)
{
  startTime = time(NULL);
  mode = !params.FindOneString("mode", "transfer").compare("visibility") ? Visibility : Transfer;
  file = fopen(params.FindOneString("filename", "untitled.transfer").c_str(), "wb");
  harmonicLevel = params.FindOneInt("harmonicLevel", 4);
  numHarmonics = SHTerms(harmonicLevel);
  
  harmonics = vector<Spectrum*>();
  normals = vector<Normal>();
  points = vector<Point>();
  
  int i = 0, count;
  const Normal* n;
  const Point* p;
  
  while ((p = params.FindPoint(append("P", i), &count)) != NULL) {
    n = params.FindNormal(append("N", i), &count);
    i++;
    
    for (int i = 0; i < count; ++i) {
      harmonics.push_back(new Spectrum[numHarmonics]);
      normals.push_back(n[i]);
      points.push_back(p[i]);
    }
  }
  
  fwrite(&numHarmonics, sizeof(int), 1, file);
}

PrecomputeTransfer::~PrecomputeTransfer() {
  fclose(file);
  
  for (int i = 0; i < harmonics.size(); i++)
    delete harmonics[i];
}

void PrecomputeTransfer::Render(const Scene *scene) {
  printf("Rendering %s with %d coefficients\n", mode == Visibility ? "visibility" : "transfer", numHarmonics);
  vector<Task*> tasks;
  
  int numPoints = points.size();
  int step = numPoints / NumSystemCores() + 1;
  this->scene = scene;
  
  for (int start = 0, end; start < numPoints; start = end) {
    end = start + step;
    end = numPoints <= end ? numPoints : end;
    tasks.push_back(new TransferTask(this, start, end));
  }
  
  EnqueueTasks(tasks);
  WaitForAllTasks();
  
  printf("Writing file\n");
  for (int i = 0; i < harmonics.size(); i++)
    for (int j = 0; j < numHarmonics; j++)
      harmonics[i][j].WriteBinary(file, mode == Visibility ? 1 : 3);
  
  printf("Done, took %ld seconds\n", time(NULL) - startTime);
}

Spectrum PrecomputeTransfer::Transmittance(const Scene*, const RayDifferential&, const Sample*, RNG&, MemoryArena&) const {return NULL;}
Spectrum PrecomputeTransfer::Li(const Scene*, const RayDifferential&, const Sample*, RNG&, MemoryArena&, Intersection*, Spectrum*) const {return NULL;}