//
// Created by Bogdan Madzhuga on 02.12.2023.
//

#include "FireParticle.h"

FireParticle::FireParticle() {
    generateParticles();
    this->placeable = new Placeable(new Renderable(particles));
}

FireParticle::FireParticle(int details) {
    this->details = details;
    generateParticles();
    this->placeable = new Placeable(new Renderable(particles));
}

void FireParticle::generateParticles() {
    for (int i = 0; i < details; i++) {
        auto p = Particle(rng);
        particles.push_back(p);
    }
}

void FireParticle::moveParticles(float dt) {

    for (auto &p: particles) {
        p.velocity += A * dt;

        p.position += p.velocity * dt;

        p.velocity *= exp(-C * dt);

        p.size *= exp(-D * dt);

        p.rotation += p.angularVelocity * dt;

        if (p.position.y > 2) {
            p = Particle(rng);
        }
    }

    if (particles.size() < details) {
        Particle p(rng);
        particles.push_back(p);
    }
}

void FireParticle::moveAndDraw(float dt) {
    moveParticles(dt);
    getPlaceable()->draw();
}

void FireParticle::setPosition(glm::vec3 position) {
    placeable->setPosition(position);
}

void FireParticle::addRotation(glm::vec3 offset) {
    placeable->addRotation(offset);
}

void FireParticle::setRotation(glm::vec3 rotation) {
    placeable->setRotation(rotation);
}

void FireParticle::setScale(glm::vec3 scale) {
    placeable->setScale(scale);
}

Material *FireParticle::getMaterial() {
    return placeable->getMaterial();
}

Placeable *FireParticle::getPlaceable() {
    return placeable;
}

void FireParticle::move(float dt) {
    moveParticles(dt);
    getPlaceable()->GetRenderable()->setParticles(particles);
}
