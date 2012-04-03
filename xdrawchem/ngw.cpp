// ngw.cpp - definitions for functions of class NewGraphWidget

#include <iostream>

#include <QPainter>
#include <QPrinter>
#include <QPrintDialog>
#include <QPaintEvent>

#include "defs.h"
#include "render2d.h"
#include "ngw.h"
#include "graphdata.h"

NewGraphWidget::NewGraphWidget( QWidget * parent )
    : QWidget( parent )
{
    setMinimumWidth( 500 );
    setMinimumHeight( 200 );
    datatype = 0;
    output = 0;
    PrintSetup();
}

void NewGraphWidget::paintEvent( QPaintEvent *pe )
{
    QPainter p;
    QString s, ft;
    bool cmp2 = false;
    int cx = 10, cy = 10;

    if ( output == 0 )
        p.begin( this );
    else
        p.begin( printer );

    int tick_spacing, t1, peakplace;
    QString n1;

    switch ( datatype ) {
    case DATA_1HNMR:
        tick_spacing = qRound( ( ( double ) width() - 40.0 ) / 12.0 );

        // draw baseline
        p.drawLine( 20, height() - 40, width() - 20, height() - 40 );

        // draw ticks
        for ( int x = 0; x < 13; x++ ) {
            t1 = 20 + x * tick_spacing;
            p.drawLine( t1, height() - 40, t1, height() - 35 );
            n1.setNum( 12 - x );
            p.drawText( t1 - 2, height() - 22, n1 );
        }

        // draw spectrum
        foreach ( tmp_data, peaks ) {
            peakplace = width() - 20 - qRound( tmp_data->value * tick_spacing );
            p.setPen( tmp_data->pcolor );
            p.drawLine( peakplace, height() - 40, peakplace, height() - 40 - ( 15 * tmp_data->intensity ) );
        }
        break;
    case DATA_13CNMR:
        tick_spacing = ( width() - 40 ) / 9;

        // draw baseline
        p.drawLine( 20, height() - 40, width() - 20, height() - 40 );

        // draw ticks
        for ( int x = 0; x < 10; x++ ) {
            t1 = 20 + x * tick_spacing;
            p.drawLine( t1, height() - 40, t1, height() - 35 );
            n1.setNum( ( 8 - x ) * 20 );
            p.drawText( t1 - 2, height() - 22, n1 );
        }

        // draw spectrum
        foreach ( tmp_data, peaks ) {
            peakplace = width() - 20 - tick_spacing - ( ( tmp_data->value / 20.0 ) * tick_spacing );
            p.setPen( tmp_data->pcolor );
            p.drawLine( peakplace, height() - 40, peakplace, height() - 40 - qRound( 15.0 * tmp_data->intensity ) );
        }
        break;
    case DATA_IR:
        tick_spacing = ( width() - 40 ) / 12;

        // draw baseline
        p.drawLine( 20, 40, width() - 20, 40 );

        // draw ticks
        for ( int x = 0; x < 13; x++ ) {
            t1 = 20 + x * tick_spacing;
            p.drawLine( t1, 40, t1, 35 );
            n1.setNum( ( 12 - x ) * 300 );
            p.drawText( t1 - 3, 22, n1 );
        }

        // draw spectrum
        foreach ( tmp_data, peaks ) {
            peakplace = width() - 20 - ( qRound( tmp_data->value / 300.0 ) * tick_spacing );
            p.setPen( tmp_data->pcolor );
            p.drawLine( peakplace, 40, peakplace, 40 + tmp_data->intensity );
        }
        break;
    default:
        break;
    }
}

void NewGraphWidget::PrintSetup()
{
    printer = new QPrinter;
    printer->setFullPage( true );
    printer->setPageSize( QPrinter::Letter );
    printer->setOrientation( QPrinter::Portrait );
    printer->setColorMode( QPrinter::GrayScale );
}

void NewGraphWidget::Print()
{
    qDebug() << "Graph::Print";

    QPrintDialog dialog( printer, this );
    if ( !dialog.exec() ) {
        return;
    }

    output = 1;
    update();
    output = 0;
    update();
}

void NewGraphWidget::AddPeak( double v1, QColor c1, QString l1, QString t1 )
{
    GraphData *g1 = new GraphData;

    g1->value = v1;
    g1->fulltext = t1;
    g1->pcolor = c1;
    if ( l1.isEmpty() == false ) {
        g1->label = l1;
        g1->drawlabel = true;
    }
    // check for duplicate peaks
    foreach ( GraphData * tg, peaks ) {
        if ( tg->value == v1 ) {
            tg->intensity++;
            return;
        }
    }
    peaks.append( g1 );
}

void NewGraphWidget::AddPeak( Peak * tmp_peak, QColor c1 )
{
    GraphData *g1 = new GraphData;

    g1->value = tmp_peak->value;
    g1->fulltext = tmp_peak->comment;
    g1->intensity = tmp_peak->intensity;
    g1->pcolor = c1;
    // check for duplicate peaks
    foreach ( GraphData * tg, peaks ) {
        if ( ( tg->value == g1->value ) && ( tg->pcolor == g1->pcolor ) ) {
            qDebug() << "found existing peak";
            tg->intensity += g1->intensity;
            delete g1;

            return;
        }
    }
    peaks.append( g1 );
}

void NewGraphWidget::AddPeak( double v1, int m1, QColor c1, QString l1, QString t1 )
{
    GraphData *g1 = new GraphData;

    g1->value = v1;
    g1->fulltext = t1;
    g1->pcolor = c1;
    g1->intensity = m1;
    if ( l1.isEmpty() == false ) {
        g1->label = l1;
        g1->drawlabel = true;
    }
    peaks.append( g1 );
}

//cmake#include "ngw.moc"

// kate: tab-width 4; indent-width 4; space-indent on; replace-trailing-space-save on;
