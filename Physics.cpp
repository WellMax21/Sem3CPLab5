#include "Physics.h"
#include <algorithm>

double dot(const Point& lhs, const Point& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

Physics::Physics(double timePerTick) : timePerTick{timePerTick} {}

void Physics::setWorldBox(const Point& topLeft, const Point& bottomRight) {
    this->topLeft = topLeft;
    this->bottomRight = bottomRight;
}

void Physics::update(std::vector<Ball>& balls, std::vector<Dust>& dusts,
                     const size_t ticks) const {

    for (size_t i = 0; i < ticks; ++i) {
        move(balls);
        collideWithBox(balls);
        collideBalls(balls, dusts);

        for (Dust& dust : dusts) {
            dust.update(timePerTick);
        }
        dusts.erase(std::remove_if(dusts.begin(), dusts.end(),
                                   [](const Dust& d) { return d.isExpired(); }),
                    dusts.end());
    }
}

void Physics::collideBalls(std::vector<Ball>& balls,
                           std::vector<Dust>& dusts) const {
    for (auto a = balls.begin(); a != balls.end(); ++a) {
        if (!a->isCollidable()) {
            continue;
        }
        for (auto b = std::next(a); b != balls.end(); ++b) {
            if (!b->isCollidable()) {
                continue;
            }
            const double distanceBetweenCenters2 =
                distance2(a->getCenter(), b->getCenter());
            const double collisionDistance = a->getRadius() + b->getRadius();
            const double collisionDistance2 =
                collisionDistance * collisionDistance;

            if (distanceBetweenCenters2 < collisionDistance2) {
                const Point normal = (b->getCenter() - a->getCenter()) /
                                     std::sqrt(distanceBetweenCenters2);
                const Point hit = a->getCenter() + normal * a->getRadius();
                if (processCollision(*a, *b, distanceBetweenCenters2)) {
                    const Color ca = a->getColor();
                    const Color cb = b->getColor();
                    dusts.emplace_back(
                        hit, Color((ca.red() + cb.red()) / 2,
                                   (ca.green() + cb.green()) / 2,
                                   (ca.blue() + cb.blue()) / 2));
                }
            }
        }
    }
}

void Physics::collideWithBox(std::vector<Ball>& balls) const {
    for (Ball& ball : balls) {
        if (!ball.isCollidable()) {
            continue;
        }
        const Point p = ball.getCenter();
        const double r = ball.getRadius();
        auto isOutOfRange = [](double v, double lo, double hi) {
            return v < lo || v > hi;
        };

        if (isOutOfRange(p.x, topLeft.x + r, bottomRight.x - r)) {
            Point vector = ball.getVelocity().vector();
            vector.x = -vector.x;
            ball.setVelocity(vector);
        } else if (isOutOfRange(p.y, topLeft.y + r, bottomRight.y - r)) {
            Point vector = ball.getVelocity().vector();
            vector.y = -vector.y;
            ball.setVelocity(vector);
        }
    }
}

void Physics::move(std::vector<Ball>& balls) const {
    for (Ball& ball : balls) {
        Point newPos =
            ball.getCenter() + ball.getVelocity().vector() * timePerTick;
        ball.setCenter(newPos);
    }
}

bool Physics::processCollision(Ball& a, Ball& b,
                               double distanceBetweenCenters2) const {
    const Point normal =
        (b.getCenter() - a.getCenter()) / std::sqrt(distanceBetweenCenters2);

    const Point aV = a.getVelocity().vector();
    const Point bV = b.getVelocity().vector();

    const bool approaching = dot(aV - bV, normal) > 0;

    const double p =
        2 * (dot(aV, normal) - dot(bV, normal)) / (a.getMass() + b.getMass());

    a.setVelocity(Velocity(aV - normal * p * a.getMass()));
    b.setVelocity(Velocity(bV + normal * p * b.getMass()));
    return approaching;
}
