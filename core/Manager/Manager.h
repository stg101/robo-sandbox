#pragma once

namespace ManagerType
{
	enum Enum
	{
		Test,
		Gui
	};
}

class Manager
{

public:
	Manager(ManagerType::Enum type);
	~Manager();

	virtual void init();
	virtual void process(const float dt);
	virtual void end();

	virtual void showImGuiWindow(bool *window){};

	ManagerType::Enum getType() const { return m_type; }

private:
	ManagerType::Enum m_type;
};