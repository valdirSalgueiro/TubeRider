#pragma once
class WidgetController
{
public:
	WidgetController();
	void Setup(UUserWidget* widget, UWorld* World);
	void Teardown(UUserWidget* widget, UWorld* World);
};

