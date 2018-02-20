#pragma once

#include "Application.h"
#include "Renderer2D.h"

class Custom_Physics_EngineApp : public aie::Application {
public:

	Custom_Physics_EngineApp();
	virtual ~Custom_Physics_EngineApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
};