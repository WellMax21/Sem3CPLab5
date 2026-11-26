#pragma once
#include "Color.h"
#include "Painter.h"
#include "Point.h"
#include "Velocity.h"
#include <vector>

/**
 * Визуальный эффект столкновения: облачко мелких частиц,
 * разлетающихся от точки удара и исчезающих через небольшое время.
 * Не участвует в физике (частицы не сталкиваются).
 */
class Dust {
  public:
    /**
     * @param origin точка столкновения
     * @param color цвет частиц
     * @param lifetime время жизни, с
     * @param particleCount количество частиц
     */
    Dust(const Point& origin, const Color& color, double lifetime = 0.6,
         int particleCount = 10);

    void update(double time);
    void draw(Painter& painter) const;
    bool isExpired() const;

  private:
    struct Particle {
        Point position;
        Velocity velocity;
    };
    std::vector<Particle> particles;
    Color color;
    double lifetime;
    double timeLeft;
};
