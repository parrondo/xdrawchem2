// bracket.cpp - Bracket's implementation of functions

#include "render2d.h"
#include "drawable.h"
#include "bracket.h"
#include "render2d.h"
#include "bondedit.h"
#include "defs.h"

Bracket::Bracket( )
{
    highlighted = false;
    style = BRACKET_SQUARE;
    subtext = "";
    fill = false;
    fillColor = QColor( 0, 255, 255 );
}

QString Bracket::ToXML( QString xml_id )
{
    QString s, n1;

    // begin bracket
    s.append( "<bracket id=\"" );
    s.append( xml_id );
    s.append( "\">\n" );

    // write Start
    s.append( "<Start>" );
    n1.setNum( start->x );
    s.append( n1 );
    s.append( " " );
    n1.setNum( start->y );
    s.append( n1 );
    s.append( "</Start>\n" );

    // write End
    s.append( "<End>" );
    n1.setNum( end->x );
    s.append( n1 );
    s.append( " " );
    n1.setNum( end->y );
    s.append( n1 );
    s.append( "</End>\n" );

    // write Style
    s.append( "<style>" );
    n1.setNum( style );
    s.append( n1 );
    s.append( "</style>\n" );

    // write subscript text, if present
    if ( subtext.length() > 0 ) {
        s.append( "<subscript>" );
        s.append( subtext );
        s.append( "</subscript>\n" );
    }
    // write color
    s.append( "<color>" );
    n1.setNum( color.red() );
    s.append( n1 );
    s.append( " " );
    n1.setNum( color.green() );
    s.append( n1 );
    s.append( " " );
    n1.setNum( color.blue() );
    s.append( n1 );
    s.append( "</color>\n" );

    // fill info
    s.append( "<fill>" );
    if ( fill == true )
        s.append( "true" );
    else
        s.append( "false" );
    s.append( "</fill>" );
    s.append( "<fillcolor>" );
    n1.setNum( fillColor.red() );
    s.append( n1 );
    s.append( " " );
    n1.setNum( fillColor.green() );
    s.append( n1 );
    s.append( " " );
    n1.setNum( fillColor.blue() );
    s.append( n1 );
    s.append( "</fillcolor>\n" );

    // end bracket
    s.append( "</bracket>\n" );

    return s;
}

QString Bracket::ToCDXML( QString xml_id )
{
    QString s, n1;

    // begin arrow
    s.append( "<graphic id=\"" );
    s.append( xml_id );
    s.append( "\" BoundingBox=\"" );
    n1.setNum( end->x );
    s.append( n1 );
    s.append( " " );
    n1.setNum( end->y );
    s.append( n1 );
    s.append( " " );
    n1.setNum( start->x );
    s.append( n1 );
    s.append( " " );
    n1.setNum( start->y );
    s.append( n1 );
    s.append( "\" " );
    if ( style == BRACKET_SQUARE )
        s.append( "GraphicType=\"Bracket\" BracketType=\"SquarePair\"" );
    if ( style == BRACKET_CURVE )
        s.append( "GraphicType=\"Bracket\" BracketType=\"RoundPair\"" );
    if ( style == BRACKET_BRACE )
        s.append( "GraphicType=\"Bracket\" BracketType=\"CurlyPair\"" );
    s.append( "/>\n" );

    return s;
}

// set Bracket from XDrawChem-format XML
void Bracket::FromXML( QString xml_tag )
{
    int i1, i2;

    i1 = xml_tag.indexOf( "<Start>" );
    i2 = xml_tag.indexOf( "</Start>" ) + 8;
    start = StartFromXML( xml_tag.mid( i1, i2 - i1 ) );
    i1 = xml_tag.indexOf( "<End>" );
    i2 = xml_tag.indexOf( "</End>" ) + 6;
    end = EndFromXML( xml_tag.mid( i1, i2 - i1 ) );
    i1 = xml_tag.indexOf( "<style>" );
    if ( i1 >= 0 ) {
        i2 = xml_tag.indexOf( "</style>" ) + 8;
        style = xml_tag.mid( i1 + 7, 1 ).toInt();
    }
    i1 = xml_tag.indexOf( "<subscript>" );
    if ( i1 >= 0 ) {
        i2 = xml_tag.indexOf( "</subscript>" ) + 12;
        subtext = xml_tag.mid( i1, i2 - i1 );
        subtext.remove( 0, 11 );
        i2 = subtext.indexOf( "</subscript>" );
        subtext.remove( i2, 999 );
    }
    i1 = xml_tag.indexOf( "<color>" );
    if ( i1 >= 0 ) {
        i2 = xml_tag.indexOf( "</color>" ) + 8;
        SetColorFromXML( xml_tag.mid( i1, i2 - i1 ) );
    }
    i1 = xml_tag.indexOf( "<fill>" );
    if ( i1 >= 0 ) {
        if ( xml_tag.mid( i1 + 6, 4 ) == "true" )
            fill = true;
        else
            fill = false;
    }
    i1 = xml_tag.indexOf( "<fillcolor>" );
    if ( i1 >= 0 ) {
        i2 = xml_tag.indexOf( "</fillcolor>" ) + 12;
        SetFillColorFromXML( xml_tag.mid( i1, i2 - i1 ) );
    }
}

void Bracket::SetFillColorFromXML( QString xml_tag )
{
    qDebug() << "SetFColorFromXML:" << xml_tag;
    int i1, i2;
    int d1, d2, d3;

    i1 = xml_tag.indexOf( "<fillcolor>" );
    i2 = xml_tag.indexOf( "</fillcolor>" );
    xml_tag.remove( i2, 999 );
    xml_tag.remove( i1, 11 );

    QTextStream ts( &xml_tag, QIODevice::ReadOnly );

    ts >> d1 >> d2 >> d3;

    fillColor = QColor( d1, d2, d3 );
}

void Bracket::Render(Render2D * r)
{
    QColor c1, ivc;

    if ( highlighted )
        c1 = QColor( 255, 0, 0 );
    else
        c1 = color;
    // safety check
    if ( ( fill == true ) && ( fillable() == true ) ) {
        ivc = fillColor;
        // otherwise, it's invalid
    }
    r->drawBracket( start->toQPoint(), end->toQPoint(), c1, style, ivc );
    int tx = end->toQPoint().x(), ty = end->toQPoint().y();

    tx += 4;
    if ( subtext.length() > 0 )
        r->drawString( subtext, QPoint( tx, ty ), c1, QFont( "helvetica", 10 ) );
}

void Bracket::Edit(Render2D *r)
{
    BondEditDialog be( r, start, end, PreviewWidget::BRACKET, 0, 0, 0, style, color );

    if ( !be.exec() )
        return;
    qDebug() << "change";
    style = be.Style();
    color = be.Color();
}

bool Bracket::Find( DPoint * target )
{
    if ( start == target )
        return true;
    if ( end == target )
        return true;
    return false;
}

void Bracket::Move (double dx, double dy)
{
    if ( ( highlighted ) && ( start != 0 ) ) {
        start->x += dx;
        start->y += dy;
        }
    if ( ( highlighted ) && ( end != 0 ) ) {
        end->x += dx;
        end->y += dy;
        }
}

void Bracket::Flip( DPoint *origin, int direction )
{
    double delta;

    if ( highlighted == false )
        return;
    if ( start != 0 ) {
        if ( direction == FLIP_H ) {
            delta = start->x - origin->x;
            start->x = start->x - 2 * delta;
        } else {                // direction == FLIP_V
            delta = start->y - origin->y;
            start->y = start->y - 2 * delta;
        }
    }
    if ( end != 0 ) {
        if ( direction == FLIP_H ) {
            delta = end->x - origin->x;
            end->x = end->x - 2 * delta;
        } else {                // direction == FLIP_V
            delta = end->y - origin->y;
            end->y = end->y - 2 * delta;
        }
    }
}

void Bracket::Rotate( DPoint *origin, double angle )
{
    //double dx, dy;

    if ( highlighted == false )
        return;
    if ( start != 0 ) {
        double thisx = start->x - origin->x;
        double thisy = start->y - origin->y;
        double newx = thisx * cos( angle ) + thisy * sin( angle );
        double newy = -thisx * sin( angle ) + thisy * cos( angle );

        start->x = ( newx + origin->x );
        start->y = ( newy + origin->y );
    }
    if ( end != 0 ) {
        double thisx = end->x - origin->x;
        double thisy = end->y - origin->y;
        double newx = thisx * cos( angle ) + thisy * sin( angle );
        double newy = -thisx * sin( angle ) + thisy * cos( angle );

        end->x = ( newx + origin->x );
        end->y = ( newy + origin->y );
    }
}

void Bracket::Resize( DPoint *origin, double scale )
{
    double dx, dy;

    if ( highlighted == false )
        return;
    if ( start != 0 ) {
        dx = start->x - origin->x;
        dy = start->y - origin->y;
        dx *= scale;
        dy *= scale;
        start->x = origin->x + dx;
        start->y = origin->y + dy;
    }
    if ( end != 0 ) {
        dx = end->x - origin->x;
        dy = end->y - origin->y;
        dx *= scale;
        dy *= scale;
        end->x = origin->x + dx;
        end->y = origin->y + dy;
    }
}

// Do not allow connections to this object.
// Simplest way to do this, I think, is to disallow this function
DPoint *Bracket::FindNearestPoint( DPoint * /*target*/, double &dist )
{
    dist = 99999.0;
    return 0;
}

double Bracket::distanceTo ( DPoint * target )
{
    DPoint tl, tr, bl, br;
    double tl_x, tl_y, br_x, br_y, swp, dist1, dist2;

    tl_x = start->x;
    tl_y = start->y;
    br_x = end->x;
    br_y = end->y;
    if ( tl_x < br_x ) {
        swp = tl_x;
        tl_x = br_x;
        br_x = swp;
    }
    if ( tl_y < br_y ) {
        swp = tl_y;
        tl_y = br_y;
        br_y = swp;
    }
    tl.x = tl_x;
    tl.y = tl_y;
    bl.x = tl_x;
    bl.y = br_y;
    tr.x = br_x;
    tr.y = tl_y;
    br.x = br_x;
    br.y = br_y;
    dist1 = DistanceToLine( &tl, &bl, target );
    dist2 = DistanceToLine( &tr, &br, target );
    if ( dist1 < dist2 )
        return dist1;
    else
        return dist2;
}

void Bracket::setPoints( DPoint * s, DPoint * e )
{
    start = s;
    end = e;
}

bool Bracket::isWithinRect( QRect n, bool /*shiftdown*/ )
{
    if ( DPointInRect( start, n ) && DPointInRect( end, n ) )
        highlighted = true;
    else
        highlighted = false;
    return highlighted;
}

const QRect Bracket::BoundingBox() const
{
    if ( highlighted == false )
        return QRect( QPoint( 999, 999 ), QPoint( 0, 0 ) );
    int top, bottom, left, right, swp;

    top = ( int ) start->y;
    left = ( int ) start->x;
    bottom = ( int ) end->y;
    right = ( int ) end->x;
    if ( bottom < top ) {
        swp = top;
        top = bottom;
        bottom = swp;
    }
    if ( right < left ) {
        swp = left;
        left = right;
        right = swp;
    }
    return QRect( QPoint( left, top ), QPoint( right, bottom ) );
}

// kate: tab-width 4; indent-width 4; space-indent on; replace-trailing-space-save on;
