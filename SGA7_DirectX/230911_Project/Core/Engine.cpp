#include "pch.h"
#include "Engine.h"

Engine::Engine()
{
	context = new Context();
	context->Register_subsystem<Timer>();
	context->Register_subsystem<Graphics>();
	context->Register_subsystem<SceneManager>();
	context->Register_subsystem<Renderer>();

	context->Initialize_subsystems();
}

Engine::~Engine()
{
	SAFE_DELETE(context);
}

void Engine::Update()
{
	context->Update_subsystems();
}

void Engine::Render()
{
	Renderer* renderer = context->GetSubsystem<Renderer>();
	renderer->Update();
}
 