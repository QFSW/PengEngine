#include "skybox.h"

IMPLEMENT_ENTITY(entities::Skybox);

using namespace entities;

Skybox::Skybox()
	: Entity("Skybox", TickGroup::render)
{ }

void Skybox::tick(float)
{
	// TODO: implement
}
