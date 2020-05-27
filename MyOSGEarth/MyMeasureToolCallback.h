#pragma once
#include <osgEarthUtil/MeasureTool>			//osgEarth::Util::MeasureToolHandler
#include <osgEarthUtil/MouseCoordsTool>		//osgEarth::Util::Controls

class MyMeasureToolCallback : public osgEarth::Util::MeasureToolHandler::MeasureToolEventHandler
{
public:

	MyMeasureToolCallback(osgEarth::Util::Controls::LabelControl* label);
	virtual void onDistanceChanged(osgEarth::Util::MeasureToolHandler* sender, double distance);


private:

	osgEarth::Util::Controls::LabelControl* m_label;
};
