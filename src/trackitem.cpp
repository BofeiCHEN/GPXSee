#include <QApplication>
#include <QCursor>
#include <QPainter>
#include "format.h"
#include "tooltip.h"
#include "trackitem.h"


QString TrackItem::toolTip()
{
	ToolTip tt;

	if (!_name.isEmpty())
		tt.insert(qApp->translate("TrackItem", "Name"), _name);
	if (!_desc.isEmpty())
		tt.insert(qApp->translate("TrackItem", "Description"), _desc);
	tt.insert(qApp->translate("TrackItem", "Distance"),
	  Format::distance(_distance, _units));
	if  (_time > 0)
		tt.insert(qApp->translate("TrackItem", "Time"),
		  Format::timeSpan(_time));
	if (!_date.isNull())
		tt.insert(qApp->translate("TrackItem", "Date"),
		  _date.toString(Qt::SystemLocaleShortDate));

	return tt.toString();
}

TrackItem::TrackItem(const Track &track, QGraphicsItem *parent)
  : PathItem(parent)
{
	QPointF p;
	const Path &path = track.path();
	Q_ASSERT(path.count() >= 2);

	p = path.first().toMercator();
	_path.moveTo(QPointF(p.x(), -p.y()));
	for (int i = 1; i < path.size(); i++) {
		p = path.at(i).toMercator();
		_path.lineTo(QPointF(p.x(), -p.y()));
	}

	updateShape();

	_name = track.name();
	_desc = track.description();
	_date = track.date();
	_distance = track.distance();
	_time = track.time();

	_marker->setPos(_path.pointAtPercent(0));

	setToolTip(toolTip());
}

void TrackItem::setUnits(enum Units units)
{
	PathItem::setUnits(units);
	setToolTip(toolTip());
}
