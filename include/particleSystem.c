#include "particleSystem.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <math.h>
#define MAX_PARTICLES 5000

void handleParticleSystem(ParticleSystem* system){
    printf("handling particle system\n");
    system->time += deltaTime;
    emitParticles(system);
    for(int i = 0; i < system->particleCount; i++){
        switch(system->type){
            case RECT:
                handleParticleRect(system, &system->particles[i]);
                break;
            case CIRCLE:
                break;
        }
    }
    printf("particle system handled\n");
}



void emitParticles(ParticleSystem* system){
    if(system->time > system->rate){
        system->time = 0;
        switch(system->type){
            case RECT:
                Particle particle;
                particle.rect = (Rectangle){system->initPos.x, system->initPos.y, system->size, system->size};
                particle.rect.x = system->initPos.x + GetRandomValue(-20, 20);
                particle.rect.y = system->initPos.y + GetRandomValue(-20, 20);
                particle.index = system->particleCount;
                particle.maxLife = system->maxLife;
                particle.life = 0;
                particle.radius = 0;
                particle.color = system->color;
                particle.initVel = system->initVel;
                particle.acceleration = system->acceleration;
                system->particles[system->particleCount] = particle;
                system->particleCount++;
                //system->particles = realloc(system->particles, sizeof(Particle) * system->particleCount);
                break;
            case CIRCLE:
                break;
        }
    }
    printf("particles emitted\n");
}



void initParticleSystem(ParticleSystem* system, Vector2 initPos, Vector2 initVel, Vector2 acceleration, double lifeSpan, float rate, float size, Color color, ParticleType type){
    system->initPos = initPos;
    system->initVel = initVel;
    system->maxLife = lifeSpan;
    system->rate = rate;
    system->size = size;
    system->color = color;
    system->type = type;
    system->time = 0;
    system->particles = malloc(sizeof(Particle) * MAX_PARTICLES);
    system->acceleration = acceleration;
    printf("particle system initialized\n");
}




void handleParticleRect(ParticleSystem* sys, Particle* particle){
    DrawRectangleRec(particle->rect, particle->color);
    particle->rect.x += particle->initVel.x * deltaTime;
    particle->rect.y += particle->initVel.y * deltaTime;
    particle->life += deltaTime;

    particle->initVel.x += particle->acceleration.x * deltaTime;
    particle->initVel.y += particle->acceleration.y * deltaTime;
    printf("step 1\n");
    if(particle->life > particle->maxLife){
        printf("step 1a\n");
        Particle lastPart = sys->particles[sys->particleCount - 1];
        printf("step 2a\n");
        sys->particles[particle->index] = lastPart;
        sys->particles[sys->particleCount - 1] = createEmptyParticle();
        printf("step 3\n");
        sys->particleCount--;
        printf("particle removed\n");
    }
    printf("particle handled\n");

}


void cleanParticleSystem(ParticleSystem* system){
    free(system->particles);
}



Particle createEmptyParticle(){
    Particle particle;
    particle.rect = (Rectangle){0, 0, 0, 0};
    particle.initPos = (Vector2){0, 0};
    particle.initVel = (Vector2){0, 0};
    particle.life = 0;
    particle.maxLife = 0;
    particle.color = (Color){0, 0, 0, 0};
    particle.size = 0;
    particle.index = 0;
    particle.radius = 0;
    particle.acceleration = (Vector2){0,0};
    return particle;
}