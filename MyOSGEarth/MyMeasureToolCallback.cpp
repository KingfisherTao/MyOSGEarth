#include "MyMeasureToolCallback.h"

MyMeasureToolCallback::MyMeasureToolCallback(osgEarth::Util::Controls::LabelControl* label) :
	m_label(label)
{
}

void MyMeasureToolCallback::onDistanceChanged(osgEarth::Util::MeasureToolHandler* sender, double distance)
{
	std::stringstream ss;
	ss << "Distance = " << std::setprecision(10) << distance << "m" << std::endl;
	std::string str;
	str = ss.str();
	m_label->setText(str);
}