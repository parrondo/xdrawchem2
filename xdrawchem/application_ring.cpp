#include <cstdio>
#include <cstdlib>
#include <cerrno>

#include <QBitmap>

#include "application.h"
#include "prefs.h"
#include "defs.h"
#include "chemdata.h"
#include "render2d.h"

extern Preferences preferences;

// canned structure images
#include "ring_xpm.h"
#include "aa_xpm.h"
#include "na_xpm.h"
#include "sugar_xpm.h"

// canned structures
#define RING_3 100
#define RING_4 101
#define RING_5 102
#define RING_6 103
#define RING_BOAT6 104
#define RING_CHAIR6 105
#define RING_IMIDAZOLE 106
#define RING_CYCLOPENTADIENE 107
#define RING_BENZENE 108
#define RING_INDOLE 109
#define RING_PURINE 110
#define RING_PYRIMIDINE 111
#define RING_STEROID 112
#define RING_NAPHTHALENE 113
#define RING_ANTHRACENE 114
#define RING_7 115
#define RING_8 116
#define RING_BIPHENYL 120
#define NA_ADENINE  200
#define NA_CYTOSINE 201
#define NA_GUANINE  202
#define NA_THYMINE  203
#define NA_URACIL   204
#define AA_ALANINE 300
#define AA_ARGININE 301
#define AA_ASPARAGINE 302
#define AA_ASPARTIC_ACID 303
#define AA_CYSTEINE 304
#define AA_GLUTAMIC_ACID 305
#define AA_GLUTAMINE 306
#define AA_GLYCINE 307
#define AA_HISTIDINE 308
#define AA_ISOLEUCINE 309
#define AA_LEUCINE 310
#define AA_LYSINE 311
#define AA_METHIONINE 312
#define AA_PHENYLALANINE 313
#define AA_PROLINE 314
#define AA_SERINE 315
#define AA_THREONINE 316
#define AA_TRYPTOPHAN 317
#define AA_TYROSINE 318
#define AA_VALINE 319
#define AA_NPH 320
#define AA_STATINE 321
#define S_RIBOSE 400
#define S_DEOXYRIBOSE 401
#define S_D_FRUCTOSE 402
#define S_D_GLUCOSE 403
#define FG_FMOC 500
#define FG_BOC 501
#define FG_DABCYL 502
#define FG_DABSYL 503
#define FG_DANSYL 504
#define FG_EDANS 505
#define FG_BIOTIN 506
// end structures

QMenu *ApplicationWindow::BuildNewRingMenu()
{
    QMenu *ringSub = new QMenu( this );

    // make ring list
    QMenu *genericSub = new QMenu( tr( "Rings" ), ringSub );
    ring3Action = genericSub->addAction( QIcon( QPixmap( r_cyclopropane ) ), tr( "[*] Cyclopropane") );
    ring4Action = genericSub->addAction( QIcon( QPixmap( r_cyclobutane ) ), tr( "[*] Cyclobutane") );
    ring5Action = genericSub->addAction( QIcon( QPixmap( r_cyclopentane ) ), tr( "[*] Cyclopentane" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    ringImidazoleAction = genericSub->addAction( QIcon( QPixmap( r_imidazole ) ), tr( "Imidazole" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    ringCyclopentadieneAction = genericSub->addAction( QIcon( QPixmap( r_cyclopentadiene ) ), tr( "[*] Cyclopentadiene" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    ring6Action = genericSub->addAction( QIcon( QPixmap( r_6flat ) ), tr( "[*] Cyclohexane (flat)" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    ring6BoatAction = genericSub->addAction( QIcon( QPixmap( r_6boat ) ), tr( "Cyclohexane (boat)" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    ring6ChairAction = genericSub->addAction( QIcon( QPixmap( r_6chair ) ), tr( "Cyclohexane (chair)" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    ringBenzeneAction = genericSub->addAction( QIcon( QPixmap( r_benzene ) ), tr( "[*] Benzene" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    ringPyrimidineAction = genericSub->addAction( QIcon( QPixmap( r_pyrimidine ) ), tr( "Pyrimidine" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    ring7Action = genericSub->addAction( QIcon( QPixmap( ring7 ) ), tr( "[*] Cycloheptane" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    ring8Action = genericSub->addAction( QIcon( QPixmap( ring8 ) ), tr( "[*] Cyclooctane" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    ringIndoleAction = genericSub->addAction( QIcon( QPixmap( r_indole ) ), tr( "Indole" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    ringPurineAction = genericSub->addAction( QIcon( QPixmap( r_purine ) ), tr( "Purine" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    ringNaphAction = genericSub->addAction( QIcon( QPixmap( naphthalene_xpm ) ), tr( "Naphthalene" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    ringBiphenylAction = genericSub->addAction( QIcon( QPixmap( biphenyl_xpm ) ), tr( "Biphenyl" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    ringAnthraAction = genericSub->addAction( QIcon( QPixmap( anthracene_xpm ) ), tr( "Anthracene" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    ringSteroidAction = genericSub->addAction( QIcon( QPixmap( r_steroid ) ), tr( "Steroid (fused ring template)" ), this, SLOT( FromNewRingMenu( int ) ), 0 );

    ringSub->addMenu( genericSub );

    // make amino acid list
    QMenu *aaSub = new QMenu( tr( "Amino acids" ), this );
    aaSub->addAction( QIcon( QPixmap( aa_ala ) ), tr( "Alanine" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    aaSub->addAction( QIcon( QPixmap( aa_arg ) ), tr( "Arginine" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    aaSub->addAction( QIcon( QPixmap( aa_asn ) ), tr( "Asparagine" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    aaSub->addAction( QIcon( QPixmap( aa_asp ) ), tr( "Aspartic acid" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    aaSub->addAction( QIcon( QPixmap( aa_cys ) ), tr( "Cysteine" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    aaSub->addAction( QIcon( QPixmap( aa_glu ) ), tr( "Glutamic acid" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    aaSub->addAction( QIcon( QPixmap( aa_gln ) ), tr( "Glutamine" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    aaSub->addAction( QIcon( QPixmap( aa_gly ) ), tr( "Glycine" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    aaSub->addAction( QIcon( QPixmap( aa_his ) ), tr( "Histidine" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    aaSub->addAction( QIcon( QPixmap( aa_ile ) ), tr( "Isoleucine" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    aaSub->addAction( QIcon( QPixmap( aa_leu ) ), tr( "Leucine" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    aaSub->addAction( QIcon( QPixmap( aa_lys ) ), tr( "Lysine" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    aaSub->addAction( QIcon( QPixmap( aa_met ) ), tr( "Methionine" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    aaSub->addAction( QIcon( QPixmap( aa_nph ) ), tr( "Nitrophenylalanine" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    aaSub->addAction( QIcon( QPixmap( aa_phe ) ), tr( "Phenylalanine" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    aaSub->addAction( QIcon( QPixmap( aa_pro ) ), tr( "Proline" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    aaSub->addAction( QIcon( QPixmap( aa_ser ) ), tr( "Serine" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    aaSub->addAction( QIcon( QPixmap( aa_statine ) ), tr( "Statine" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    aaSub->addAction( QIcon( QPixmap( aa_thr ) ), tr( "Threonine" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    aaSub->addAction( QIcon( QPixmap( aa_trp ) ), tr( "Tryptophan" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    aaSub->addAction( QIcon( QPixmap( aa_tyr ) ), tr( "Tyrosine" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    aaSub->addAction( QIcon( QPixmap( aa_val ) ), tr( "Valine" ), this, SLOT( FromNewRingMenu( int ) ), 0 );

    ringSub->addMenu( aaSub );

    // make nucleic acid list
    QMenu *naSub = new QMenu( tr( "Nucleic acids" ), this );
    naAdenineAction = naSub->addAction( QIcon( QPixmap( na_adenine ) ), tr( "Adenine" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    naCytosineAction = naSub->addAction( QIcon( QPixmap( na_cytosine ) ), tr( "Cytosine" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    naGuanineAction = naSub->addAction( QIcon( QPixmap( na_guanine ) ), tr( "Guanine" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    naThymineAction = naSub->addAction( QIcon( QPixmap( na_thymine ) ), tr( "Thymine" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    naUracilAction = naSub->addAction( QIcon( QPixmap( na_uracil ) ), tr( "Uracil" ), this, SLOT( FromNewRingMenu( int ) ), 0 );

    ringSub->addMenu( naSub );

    // make sugar list
    QMenu *sugarSub = new QMenu( tr( "Sugars" ), this );
    srAction = sugarSub->addAction( QIcon( QPixmap( s_ribose ) ), tr( "Ribose" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    sdAction = sugarSub->addAction( QIcon( QPixmap( s_deoxyribose ) ), tr( "Deoxyribose" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    sfAction = sugarSub->addAction( QIcon( QPixmap( s_d_fructose ) ), tr( "D-fructose" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    sgAction = sugarSub->addAction( QIcon( QPixmap( s_d_glucose ) ), tr( "D-glucose" ), this, SLOT( FromNewRingMenu( int ) ), 0 );

    ringSub->addMenu( sugarSub );

    // make function group list
    QMenu *fgSub = new QMenu( tr( "Useful groups" ), this );
    fgSub->addAction( tr( "[*] FMOC" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    fgSub->addAction( tr( "[*] BOC" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    fgSub->addAction( tr( "[*] DABCYL" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    fgSub->addAction( tr( "[*] DABSYL" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    fgSub->addAction( tr( "[*] DANSYL" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    fgSub->addAction( tr( "[*] EDANS" ), this, SLOT( FromNewRingMenu( int ) ), 0 );
    fgSub->addAction( tr( "[*] Biotin" ), this, SLOT( FromNewRingMenu( int ) ), 0 );

    ringSub->addMenu( fgSub );

    // make user-defined list
    ringSub->addMenu( BuildCustomRingMenu() );

    return ringSub;
}

QMenu *ApplicationWindow::BuildCustomRingMenu()
{
    QMenu *userDefSub = new QMenu( tr( "User-defined" ), this );
    QDir d( preferences.getCustomRingDir(), "*.png" );

    ringlist = d.entryList();
    for ( int cc = 0; cc < ringlist.count(); cc++ ) {
        QPixmap px1( QString( preferences.getCustomRingDir() + ringlist[cc] ) );
        QBitmap mask1( px1.width(), px1.height() );

        mask1.fill( Qt::color1 );
        px1.setMask( mask1 );
        /*userDefSub->addAction( QPixmap( QString(RingDir + ringlist[cc]) ),
           ringlist[cc],
           this, FromRingMenu(int)), 0, cc ); */
        userDefSub->addAction( QIcon( px1 ), ringlist[cc].left( ringlist[cc].length() - 4 ), this, SLOT( FromRingMenu( int ) ), 0 );
    }
    userDefSub->addAction( tr( "Add new..." ), this, SLOT( saveCustomRing() ), 0 );

    return userDefSub;
}

void ApplicationWindow::FromNewRingMenu( int x )
{
    //c->StartUndo(0,0);
    //c->DeselectAll();
    //c->SetTopLeft(sv->viewportToContents(QPoint(0,0)));
    QString fname;

    if ( x == RING_3 )
        fname = "cyclopropane.cml";
    if ( x == RING_4 )
        fname = "cyclobutane.cml";
    if ( x == RING_5 )
        fname = "cyclopentane.cml";
    if ( x == RING_IMIDAZOLE )
        fname = "imidazole.cml";
    if ( x == RING_CYCLOPENTADIENE )
        fname = "cyclopentadiene.cml";
    if ( x == RING_6 )
        fname = "cyclohexane.cml";
    if ( x == RING_BOAT6 )
        fname = "6ring_boat.cml";
    if ( x == RING_CHAIR6 )
        fname = "6ring_chair.cml";
    if ( x == RING_BENZENE )
        fname = "benzene.cml";
    if ( x == RING_7 )
        fname = "cycloheptane.cml";
    if ( x == RING_8 )
        fname = "cyclooctane.cml";
    if ( x == RING_INDOLE )
        fname = "indole.cml";
    if ( x == RING_STEROID )
        fname = "steroid.cml";
    if ( x == RING_PURINE )
        fname = "purine.cml";
    if ( x == RING_PYRIMIDINE )
        fname = "pyrimidine.cml";
    if ( x == RING_NAPHTHALENE )
        fname = "naphthalene.cml";
    if ( x == RING_ANTHRACENE )
        fname = "anthracene.cml";
    if ( x == RING_BIPHENYL )
        fname = "biphenyl.cml";
    if ( x == NA_ADENINE )
        fname = "adenine.cml";
    if ( x == NA_CYTOSINE )
        fname = "cytosine.cml";
    if ( x == NA_GUANINE )
        fname = "guanine.cml";
    if ( x == NA_THYMINE )
        fname = "thymine.cml";
    if ( x == NA_URACIL )
        fname = "uracil.cml";
    if ( x == AA_ALANINE )
        fname = "alanine.cml";
    if ( x == AA_ARGININE )
        fname = "arginine.cml";
    if ( x == AA_ASPARTIC_ACID )
        fname = "aspartic_acid.cml";
    if ( x == AA_ASPARAGINE )
        fname = "asparagine.cml";
    if ( x == AA_CYSTEINE )
        fname = "cysteine.cml";
    if ( x == AA_GLUTAMIC_ACID )
        fname = "glutamic_acid.cml";
    if ( x == AA_GLUTAMINE )
        fname = "glutamine.cml";
    if ( x == AA_GLYCINE )
        fname = "glycine.cml";
    if ( x == AA_HISTIDINE )
        fname = "histidine.cml";
    if ( x == AA_ISOLEUCINE )
        fname = "isoleucine.cml";
    if ( x == AA_LEUCINE )
        fname = "leucine.cml";
    if ( x == AA_LYSINE )
        fname = "lysine.cml";
    if ( x == AA_METHIONINE )
        fname = "methionine.cml";
    if ( x == AA_NPH )
        fname = "nitrophenylalanine.cml";
    if ( x == AA_PHENYLALANINE )
        fname = "phenylalanine.cml";
    if ( x == AA_PROLINE )
        fname = "proline.cml";
    if ( x == AA_SERINE )
        fname = "serine.cml";
    if ( x == AA_STATINE )
        fname = "statine.cml";
    if ( x == AA_THREONINE )
        fname = "threonine.cml";
    if ( x == AA_TRYPTOPHAN )
        fname = "tryptophan.cml";
    if ( x == AA_TYROSINE )
        fname = "tyrosine.cml";
    if ( x == AA_VALINE )
        fname = "valine.cml";
    if ( x == S_RIBOSE )
        fname = "ribose.cml";
    if ( x == S_DEOXYRIBOSE )
        fname = "deoxyribose.cml";
    if ( x == S_D_FRUCTOSE )
        fname = "d-fructose.cml";
    if ( x == S_D_GLUCOSE )
        fname = "d-glucose.cml";
    if ( x == FG_FMOC )
        fname = "fmoc.cml";
    if ( x == FG_BOC )
        fname = "boc.cml";
    if ( x == FG_DABCYL )
        fname = "dabcyl.cml";
    if ( x == FG_DABSYL )
        fname = "dabsyl.cml";
    if ( x == FG_DANSYL )
        fname = "dansyl.cml";
    if ( x == FG_EDANS )
        fname = "edans.cml";
    if ( x == FG_BIOTIN )
        fname = "biotin.cml";
    //c->load( RingDir + fname );
    //m_renderer->Inserted();
    if ( fname == "cyclopropane.cml" ) {
        m_renderer->setMode_DrawRing( RingDir + fname, fname.left( fname.length() - 4 ), 1 );
        return;
    }
    if ( fname == "cyclobutane.cml" ) {
        m_renderer->setMode_DrawRing( RingDir + fname, fname.left( fname.length() - 4 ), 1 );
        return;
    }
    if ( fname == "cyclopentane.cml" ) {
        m_renderer->setMode_DrawRing( RingDir + fname, fname.left( fname.length() - 4 ), 1 );
        return;
    }
    if ( fname == "cyclopentadiene.cml" ) {
        m_renderer->setMode_DrawRing( RingDir + fname, fname.left( fname.length() - 4 ), 1 );
        return;
    }
    if ( fname == "cyclohexane.cml" ) {
        m_renderer->setMode_DrawRing( RingDir + fname, fname.left( fname.length() - 4 ), 1 );
        return;
    }
    if ( fname == "benzene.cml" ) {
        m_renderer->setMode_DrawRing( RingDir + fname, fname.left( fname.length() - 4 ), 1 );
        return;
    }
    if ( fname == "cycloheptane.cml" ) {
        m_renderer->setMode_DrawRing( RingDir + fname, fname.left( fname.length() - 4 ), 1 );
        return;
    }
    if ( fname == "cyclooctane.cml" ) {
        m_renderer->setMode_DrawRing( RingDir + fname, fname.left( fname.length() - 4 ), 1 );
        return;
    }
    if ( fname == "fmoc.cml" ) {
        m_renderer->setMode_DrawRing( RingDir + fname, fname.left( fname.length() - 4 ), 2 );
        return;
    }
    if ( fname == "boc.cml" ) {
        m_renderer->setMode_DrawRing( RingDir + fname, fname.left( fname.length() - 4 ), 2 );
        return;
    }
    if ( fname == "dabcyl.cml" ) {
        m_renderer->setMode_DrawRing( RingDir + fname, fname.left( fname.length() - 4 ), 2 );
        return;
    }
    if ( fname == "dabsyl.cml" ) {
        m_renderer->setMode_DrawRing( RingDir + fname, fname.left( fname.length() - 4 ), 2 );
        return;
    }
    if ( fname == "dansyl.cml" ) {
        m_renderer->setMode_DrawRing( RingDir + fname, fname.left( fname.length() - 4 ), 2 );
        return;
    }
    if ( fname == "edans.cml" ) {
        m_renderer->setMode_DrawRing( RingDir + fname, fname.left( fname.length() - 4 ), 2 );
        return;
    }
    if ( fname == "biotin.cml" ) {
        m_renderer->setMode_DrawRing( RingDir + fname, fname.left( fname.length() - 4 ), 2 );
        return;
    }
    if ( ( x > 299 ) && ( x < 350 ) ) {
        m_renderer->setMode_DrawRing( RingDir + fname, fname.left( fname.length() - 4 ), 3 );
        return;
    }
    m_renderer->setMode_DrawRing( RingDir + fname, fname.left( fname.length() - 4 ) );
}

void ApplicationWindow::FromNewerRingMenu( QAction * action )
{
  qDebug() << "FromNewerRingMenu() invoked";
  if (action == ring3Action) {
    FromNewRingMenu( RING_3 );
    return;
  }
}

void ApplicationWindow::setRingAction( QAction * action )
{
  qDebug() << "setRingAction() invoked";
  if (action == ring3Action) {
    FromNewRingMenu( RING_3 );
    drawRingButton->setIcon( QIcon( QPixmap( r_cyclopropane ) ) );
    drawRingButton->setDefaultAction( ring3Action );
    return;
  }
  if (action == ring4Action) {
    FromNewRingMenu( RING_4 );
    drawRingButton->setDefaultAction( ring4Action );
    return;
  }
  if (action == ring5Action) {
    FromNewRingMenu( RING_5 );
    drawRingButton->setDefaultAction( ring5Action );
    return;
  }
  if (action == ringImidazoleAction) {
    FromNewRingMenu( RING_IMIDAZOLE );
    drawRingButton->setDefaultAction( ringImidazoleAction );
    return;
  }
  if (action == ringCyclopentadieneAction) {
    FromNewRingMenu( RING_CYCLOPENTADIENE );
    drawRingButton->setDefaultAction( ringCyclopentadieneAction );
    return;
  }
  if (action == ring6Action) {
    FromNewRingMenu( RING_6 );
    drawRingButton->setDefaultAction( ring6Action );
    return;
  }
  if (action == ring6BoatAction) {
    FromNewRingMenu( RING_BOAT6 );
    drawRingButton->setDefaultAction( ring6BoatAction );
    return;
  }
  if (action == ring6ChairAction) {
    FromNewRingMenu( RING_CHAIR6 );
    drawRingButton->setDefaultAction( ring6ChairAction );
    return;
  }
  if (action == ringBenzeneAction) {
    FromNewRingMenu( RING_BENZENE );
    drawRingButton->setDefaultAction( ringBenzeneAction );
    return;
  }
  if (action == ringPyrimidineAction) {
    FromNewRingMenu( RING_PYRIMIDINE );
    drawRingButton->setDefaultAction( ringPyrimidineAction );
    return;
  }
  if (action == ringIndoleAction) {
    FromNewRingMenu( RING_INDOLE );
    drawRingButton->setDefaultAction( ringIndoleAction );
    return;
  }
  if (action == ringNaphAction) {
    FromNewRingMenu( RING_NAPHTHALENE );
    drawRingButton->setDefaultAction( ringNaphAction );
    return;
  }
  if (action == ringBiphenylAction) {
    FromNewRingMenu( RING_BIPHENYL );
    drawRingButton->setDefaultAction( ringBiphenylAction );
    return;
  }
  if (action == ringPurineAction) {
    FromNewRingMenu( RING_PURINE );
    drawRingButton->setDefaultAction( ringPurineAction );
    return;
  }
  if (action == ringAnthraAction) {
    FromNewRingMenu( RING_ANTHRACENE );
    drawRingButton->setDefaultAction( ringAnthraAction );
    return;
  }
  if (action == ringSteroidAction) {
    FromNewRingMenu( RING_STEROID );
    drawRingButton->setDefaultAction( ringSteroidAction );
    return;
  }

  if (action == naAdenineAction) {
    FromNewRingMenu( NA_ADENINE );
    drawRingButton->setDefaultAction( naAdenineAction );
    return;
  }
  if (action == naCytosineAction) {
    FromNewRingMenu( NA_CYTOSINE );
    drawRingButton->setDefaultAction( naCytosineAction );
    return;
  }
  if (action == naGuanineAction) {
    FromNewRingMenu( NA_GUANINE );
    drawRingButton->setDefaultAction( naGuanineAction );
    return;
  }
  if (action == naThymineAction) {
    FromNewRingMenu( NA_THYMINE );
    drawRingButton->setDefaultAction( naThymineAction );
    return;
  }
  if (action == naUracilAction) {
    FromNewRingMenu( NA_URACIL );
    drawRingButton->setDefaultAction( naUracilAction );
    return;
  }

  if (action == srAction) {
    FromNewRingMenu( S_RIBOSE );
    drawRingButton->setDefaultAction( srAction );
    return;
  }
  if (action == sdAction) {
    FromNewRingMenu( S_DEOXYRIBOSE );
    drawRingButton->setDefaultAction( sdAction );
    return;
  }
  if (action == sfAction) {
    FromNewRingMenu( S_D_FRUCTOSE );
    drawRingButton->setDefaultAction( sfAction );
    return;
  }
  if (action == sgAction) {
    FromNewRingMenu( S_D_GLUCOSE );
    drawRingButton->setDefaultAction( sgAction );
    return;
  }
}

// kate: tab-width 4; indent-width 4; space-indent on; replace-trailing-space-save on;
