#include "painterThread.h"

#include <QtGui/QPainter>
#include <QDebug>
#include <time.h>

#include "../legend/legendAttributes.h"

extern bool QUIET_MODE;

using namespace TerraMEObserver;

PainterThread::PainterThread(QObject *parent)
    : QThread(parent)
{
    restart = false;
    abort = false;
    reconfigMaxMin = false;
    // defaultPen = QPen(Qt::NoPen);		// n�o desenha a grade

    //posicionar randomicamente os agentes na c�lula
    // para que seja poss�vel visualizar mais agentes
    // dentro da mesma c�lular
    //qsrand(time(NULL));
    qsrand(1);
}

PainterThread::~PainterThread()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

//void PainterThread::render(double scaleFactor, QSize resultSize)
//{
//	QMutexLocker locker(&mutex);
//
//	this->resultSize = resultSize;
//
//	if (!isRunning()) {
//		start(LowPriority);
//	} else {
//		restart = true;
//		condition.wakeOne();
//	}
//}

void PainterThread::run()
{
    // qDebug() << "PainterThread::run()";

    //forever {

    //    //locker.unlock();

    //    // mutex.lock();
    //    //---- Atribui��es locais
    //    //QPainter *p = this->p;
    //    //Attributes *attrib = this->attrib;
    //    //QImage *img = this->img;
    //    //----
    //    // mutex.unlock();


    //    mutex.lock();
    //    if (! restart)
    //        condition.wait(&mutex);
    //    restart = false;
    //    //printf("===>> dormiu\n");
    //    mutex.unlock();

    //}
    exec();
}

void PainterThread::drawAttrib(QPainter *p, Attributes *attrib)
{
    if (attrib->getType() == TObsAgent)
        return;

    //---- Desenha o atributo
    p->begin(attrib->getImage());

    p->setPen(Qt::NoPen); //defaultPen);

    if (attrib->getDataType() == TObsNumber)
    {
        QColor color(Qt::white);
        QVector<double> *values = attrib->getNumericValues();
        QVector<ObsLegend> *vecLegend = attrib->getLegend();

        double x = -1.0, y = -1.0, v = 0.0;

#ifdef DEBUG_OBSERVER		
        // if (attrib->getType() != TObsCell)
        {
            qDebug() << "\n----TObsNumber\nattrib->getXsValue()->size(): " << attrib->getXsValue()->size();
            qDebug() << "values->size(): " << values->size() << "\n----\n";
        }
#endif

        int vSize = values->size();
        int xSize = attrib->getXsValue()->size();
        int ySize = attrib->getYsValue()->size();

        for(int pos = 0; (pos < vSize && pos < xSize && pos < ySize); pos++)
        {
            v = values->at(pos);

            // Corrige o bug gerando quando um agente morre
            if (attrib->getXsValue()->isEmpty() || attrib->getXsValue()->size() == pos)
                break;

            x = attrib->getXsValue()->at(pos);
            y = attrib->getYsValue()->at(pos);

            if (vecLegend->isEmpty())
            {
                v = v - attrib->getMinValue();

                double c = v * attrib->getVal2Color();
                if ((c >= 0) && (c <= 255))
                {
                    color.setRgb(c, c, c);
                }
                else
                {
                    if (! reconfigMaxMin)
                    {
                        if (! QUIET_MODE )
                            qWarning("Warning: Invalid color. You need to reconfigure the "
                                     "maximum and the minimum values of the attribute \"%s\".",
                                     qPrintable(attrib->getName()) );

                        reconfigMaxMin = true;
                    }
                    color.setRgb(255, 255, 255);
                }
                p->setBrush(color);
            }
            else
            {
                for(int j = 0; j < vecLegend->size(); j++)
                {
                    p->setBrush(Qt::white);

                    const ObsLegend &leg = vecLegend->at(j);
                    if (attrib->getGroupMode() == TObsUniqueValue) // valor �nico 3
                    {
                        if (v == leg.getToNumber())
                        {
                            p->setBrush(leg.getColor());
                            break;
                        }
                    }
                    else
                    {
                        if ((leg.getFromNumber() <= v) && (v < leg.getToNumber()))
                        {
                            p->setBrush(leg.getColor());
                            break;
                        }
                    }
                }
            }
            if ((x >= 0) && ( y >= 0))
                draw(p, attrib->getType(), x, y);
        }
    }
    else if (attrib->getDataType() == TObsText)
    {
        QVector<QString> *values = attrib->getTextValues();
        QVector<ObsLegend> *vecLegend = attrib->getLegend();

#ifdef DEBUG_OBSERVER		
        if (attrib->getType() != TObsCell)
        {
            qDebug() << "\n----TObsNumber\nattrib->getXsValue()->size(): " << attrib->getXsValue()->size();
            qDebug() << "values->size(): " << values->size() << "\n----\n";
        }
#endif

        int random = rand() % 256;
        double x = -1.0, y = -1.0;

        int vSize = values->size();
        int xSize = attrib->getXsValue()->size();
        int ySize = attrib->getYsValue()->size();

        for (int pos = 0; (pos < vSize && pos < xSize && pos < ySize); pos++)
        {
            const QString & v = values->at(pos);

            // Corrige o bug gerando quando um agente morre
            if (attrib->getXsValue()->isEmpty() || attrib->getXsValue()->size() == pos)
                break;

            x = attrib->getXsValue()->at(pos);
            y = attrib->getYsValue()->at(pos);

            if (vecLegend->isEmpty())
            {
                p->setBrush(QColor(random, random, random));
            }
            else
            {
                p->setBrush(Qt::white);
                for(int j = 0; j < vecLegend->size(); j++)
                {
                    const ObsLegend &leg = vecLegend->at(j);
                    if (v == leg.getFrom())
                    {
                        p->setBrush(leg.getColor());
                        break;
                    }
                }
            }

            if ((x >= 0) && ( y >= 0))
                draw(p, attrib->getType(), x, y);
        }
    }
    p->end();
}


//void PainterThread::setVectorPos(QVector<double> *xs, QVector<double> *ys)
//{
//QMutexLocker locker(&mutex);

//this->xs = xs;
//this->ys = ys;

// a thread pode n�o ter os dados dos atributos
//if (!isRunning() && (this->p)) {
//	start(LowPriority);
//}// else {
//	restart = true;
//	condition.wakeOne();
//	//printf("--->>  acordou\n");
//}
//}

void PainterThread::draw(QPainter *p, TypesOfSubjects type, double &x, double &y)
{
    switch (type)
    {
        case TObsAutomaton:
            p->drawRect(SIZE_CELL * x, SIZE_CELL * y, SIZE_AUTOMATON, SIZE_AUTOMATON);
            break;

        case TObsAgent:
        {
            //double rx = qrand() % (SIZE_CELL - SIZE_AGENT);
            //double ry = qrand() % (SIZE_CELL - SIZE_AGENT);
            //p->setPen(Qt::SolidLine);
            //p->drawEllipse(SIZE_CELL * x + rx, SIZE_CELL * y + ry, SIZE_AGENT, SIZE_AGENT);
            break;
        }
        default:
            p->drawRect(SIZE_CELL * x, SIZE_CELL * y, SIZE_CELL, SIZE_CELL);
    }
}


//void PainterThread::gridOn(bool on)
//{
//    if (on)
//        defaultPen = QPen(Qt::darkGray);// habilita desenhar a grade
//    else
//        defaultPen = QPen(Qt::NoPen);		// n�o desenha a grade
//
//}

void PainterThread::drawGrid(QImage &imgResult, double &width, double &height)
{
    mutex.lock();

    QPainter p(&imgResult);
    p.setPen(Qt::darkGray);

    for(int j = 0; j < imgResult.height(); j++)
    {
        for(int i = 0; i < imgResult.width(); i++)
            p.drawRect(i * width, j * height, width, height);
    }

    mutex.unlock();
}
