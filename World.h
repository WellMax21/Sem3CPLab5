#pragma once
#include "Ball.hpp"
#include "Dust.h"
#include "Physics.h"
#include <string>
#include <vector>

class Painter;

class World {
  public:
    World(const std::string& worldFilePath);
    void show(Painter& painter) const;
    void update(double time);

  private:
    
    Point topLeft;
    Point bottomRight;
  
    Physics physics;
    
    std::vector<Ball> balls;
    
    std::vector<Dust> dusts;
    
    
    double restTime = 0.;
};
