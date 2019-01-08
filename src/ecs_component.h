#ifndef COMPONENT_H
#define COMPONENT_H

enum ComponentType {
    RENDER,
}; 

/* For storing a list of components for an entity */
struct ComponentContainer {
    enum ComponentType type;
    void *c;
};

/* Holds a list of all of a specific component type */
struct ComponentManager {
    enum ComponentType type;
    void **c;
};

struct RenderComponent {
    wchar_t ch;
    unsigned char col;
};

#endif
