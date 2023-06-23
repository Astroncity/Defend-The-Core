#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "raylib.h"


typedef struct Particle{
    Vector2 initPos;
    Vector2 initVel;
    double life;
    double maxLife;
    Color color;
    int size;
    int index;
    Rectangle rect;
    int radius;
    Vector2 acceleration;
}Particle;


typedef enum ParticleType{
    RECT,
    CIRCLE
}ParticleType;


typedef struct ParticleSystem{
    Vector2 initPos;
    Vector2 initVel;
    double rate;
    double life;
    double maxLife;
    Color color;
    int size;
    double time;
    int particleCount;
    Particle* particles;
    ParticleType type;
    Vector2 acceleration;
}ParticleSystem;



void handleParticleSystem(ParticleSystem* system);
void emitParticles(ParticleSystem* system);
void initParticleSystem(ParticleSystem* system, Vector2 initPos, Vector2 initVel, Vector2 acceleration, double lifeSpan, float rate, float size, Color color, ParticleType type);
void handleParticleRect(ParticleSystem* sys, Particle* particle);
void cleanParticleSystem(ParticleSystem* system);
Particle createEmptyParticle();








#endif // PARTICLE_SYSTEM_H