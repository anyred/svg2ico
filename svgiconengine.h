#ifndef SVGICONENGINE_H
#define SVGICONENGINE_H

#include <QIconEngine>
#include <QtSvg/QSvgRenderer>

class SVGIconEngine : public QIconEngine
{
    QByteArray data;
public:
    SVGIconEngine();
};

#endif // SVGICONENGINE_H
