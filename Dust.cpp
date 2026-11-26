#include "Dust.h"
#include <random>

namespace {
constexpr double kPi = 3.14159265358979323846;
constexpr double kMinSpeed = 200.;
constexpr double kMaxSpeed = 700.;
constexpr double kParticleRadius = 12.;

std::mt19937& rng() {
    static std::mt19937 gen{std::random_device{}()};
    return gen;
}
}

Dust::Dust(const Point& origin, const Color& color, double lifetime,
           int particleCount)
    : color{color}, lifetime{lifetime}, timeLeft{lifetime} {
    std::uniform_real_distribution<double> angle(0., 2 * kPi);
    std::uniform_real_distribution<double> speed(kMinSpeed, kMaxSpeed);
    particles.reserve(particleCount);
    for (int i = 0; i < particleCount; ++i) {
        particles.push_back({origin, Velocity(speed(rng()), angle(rng()))});
    }
}

void Dust::update(double time) {
    for (Particle& p : particles) {
        p.position = p.position + p.velocity.vector() * time;
    }
    timeLeft -= time;
}

void Dust::draw(Painter& painter) const {

    const double radius = kParticleRadius * (timeLeft / lifetime);
    if (radius <= 0.) {
        return;
    }
    for (const Particle& p : particles) {
        painter.draw(p.position, radius, color);
    }
}

bool Dust::isExpired() const {
    return timeLeft <= 0.;
}
