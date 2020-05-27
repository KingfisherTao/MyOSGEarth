#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Depth>

#include <osgEarth/Map>
#include <osgEarth/MapNode>
#include <osgEarthUtil/Sky>
#include <osgEarth/Registry>

#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/ExampleResources>

#include "MyMeasureToolCallback.h"
#include "PickEvent.h"

int main(int argc, char** argv)
{
	osgViewer::CompositeViewer viewer;
	viewer.setThreadingModel(viewer.SingleThreaded);

	osgViewer::View* view = new osgViewer::View();

	view->setUpViewInWindow(420, 180, 1080, 720);
	view->setCameraManipulator(new osgEarth::Util::EarthManipulator());
	view->getDatabasePager()->setUnrefImageDataAfterApplyPolicy(true, false);
	osgEarth::Util::MapNodeHelper().configureView(view);

	osg::ref_ptr<osg::Group> m_world = new osg::Group();
	osg::ref_ptr<osgEarth::MapNode> m_mapNode = osgEarth::MapNode::findMapNode(osgDB::readNodeFile("World.earth"));
	m_world->addChild(m_mapNode.get());

	// 近地面自动裁剪
	view->getCamera()->addCullCallback(new osgEarth::Util::AutoClipPlaneCullCallback(m_mapNode.get()));

	// 星空
	osg::ref_ptr<osgEarth::Util::SkyNode> skyNode = osgEarth::Util::SkyNode::create();
	osg::ref_ptr<osgEarth::Util::Ephemeris> ephemeris = new osgEarth::Util::Ephemeris;
	skyNode->setEphemeris(ephemeris);
	skyNode->setName("skyNode");
	skyNode->setDateTime(osgEarth::DateTime());
	skyNode->attach(view, 0);
	skyNode->setLighting(true);
	skyNode->addChild(m_mapNode);
	m_world->addChild(skyNode);

	// 绘制线的组
	osg::ref_ptr<osg::Group> _losGroup = new osg::Group();
	_losGroup->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	_losGroup->getOrCreateStateSet()->setAttributeAndModes(new osg::Depth(osg::Depth::Function::ALWAYS));
	m_mapNode->addChild(_losGroup.get());

	// 左上角的UI
	osgEarth::Util::Controls::ControlCanvas* canvas = new osgEarth::Util::Controls::ControlCanvas();
	m_world->addChild(canvas);
	canvas->setNodeMask(0x1 << 1);

	osgEarth::Util::Controls::Grid* grid = new osgEarth::Util::Controls::Grid();
	grid->setBackColor(0, 0, 0, 0.2);
	grid->setAbsorbEvents(true);
	grid->setVertAlign(osgEarth::Util::Controls::Control::ALIGN_TOP);

	// 左上角 UI 中的 lab
	auto mouseLabel = new osgEarth::Util::Controls::LabelControl();
	grid->setControl(0, 0, new osgEarth::Util::Controls::LabelControl("Mouse:"));
	grid->setControl(1, 0, mouseLabel);

	//grid->setControl(0, 1, new osgEarth::Util::Controls::LabelControl("Distance:"));
	//auto labelDistance = new osgEarth::Util::Controls::LabelControl();
	//labelDistance->setFont(osgEarth::Registry::instance()->getDefaultFont());
	//labelDistance->setFontSize(24.0f);
	//labelDistance->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_LEFT);
	//labelDistance->setText("click to measure");
	//grid->setControl(1, 1, labelDistance);

	//Create the MeasureToolHandler
	//auto measureTool = new osgEarth::Util::MeasureToolHandler(m_mapNode);
	//measureTool->setIntersectionMask(0x2);
	//measureTool->addEventHandler(new MyMeasureToolCallback(labelDistance));

	//osgEarth::Symbology::Style style = measureTool->getLineStyle();
	//style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = osgEarth::Symbology::Color::Red;
	//style.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = 2.0f;
	//measureTool->setLineStyle(style);

	//view->addEventHandler(measureTool);

	canvas->addControl(grid);
	view->addEventHandler(new osgEarth::Util::MouseCoordsTool(m_mapNode, mouseLabel));

	PickEvent* _pickEvent = new PickEvent(m_mapNode.get(), _losGroup);
	_pickEvent->setActionEvent(EnumActionEvent::ViewshedAnalysis);
	view->addEventHandler(_pickEvent);

	view->setSceneData(m_world);

	osg::ref_ptr<osgEarth::Util::EarthManipulator> em = dynamic_cast<osgEarth::Util::EarthManipulator*>(view->getCameraManipulator());
	em->setViewpoint(osgEarth::Viewpoint(NULL, 87.43, 27.18, 2060.66, -2.5, -10, 20000), 2);

	viewer.addView(view);
	return viewer.run();
}