
//Dependencies

#include "GameObject.hpp"
#include "../Misc/Util.hpp"
#include "../Renderer/Renderer.hpp"
#include "../Renderer/Collider.hpp"
#include "../Renderer/Controller.hpp"

GameObject::GameObject() {

}

GameObject::GameObject(V2 pos, float sc) {

	position = pos;
	isoPosition = Util::twoDToIso(pos);
	scale = sc;
}

GameObject::GameObject(GameObject& in) {

	position = in.position;
	isoPosition = in.isoPosition;
	scale = in.scale;

	if (in.renderer)
		renderer = in.renderer;

	if (in.collider)
		collider = in.collider;

	if (in.controller)
		controller = in.controller;
}

void GameObject::addComponent(Collider* comp)
{
    collider = comp;
    comp->owner = this;
}

void GameObject::addComponent(Renderer* comp)
{
    renderer = comp;
    comp->owner = this;
}

void GameObject::addComponent(Controller* comp)
{
    controller = comp;
    comp->owner = this;
}

template <typename T>
T* GameObject::getComponent()
{
	return nullptr;
}

//TODO: reduce this repetition (variadic templates?)
//set owner at each component get operation
//TODO: we must guarantee it is set before any operation on an object, how?
template <>
Collider* GameObject::getComponent<Collider>()
{
	collider->owner = this;
	return collider;
}

template <>
Renderer* GameObject::getComponent<Renderer>()
{
	renderer->owner = this;
    return renderer;
}


template <>
Controller* GameObject::getComponent<Controller>()
{
	controller->owner = this;
    return controller;
}

//TODO: reduce cache misses - possible performance optimization
void GameObject::update(sf::Time delta)
{
//	for (int i = 0; i < components.size(); i++)
//	{
//        if(components[i])
//		    components[i]->update(delta);
//	}
    if(collider)
        collider->update(delta);
    if(controller)
        controller->update(delta);
}

void GameObject::draw()
{
//	for (int i = 0; i < components.size(); i++)
//	{
//		//components[i]->draw();
//	}
}

V2 GameObject::getPosition() {
	return position;
}

void GameObject::setPosition(V2 pos) {
	position = pos;
}

float GameObject::getScale() {
	return scale;
}

void GameObject::setScale(float sca) {
	scale = sca;
}

GameObject::~GameObject() {

}