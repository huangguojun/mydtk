// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

#include <dtkFontsExport>

#include <QtCore>
#include <QtGui>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

namespace oi {

  enum DTKFONTS_EXPORT icon {
     alert = 0xf02d,
     arrowdown = 0xf03f,
     arrowleft = 0xf040,
     arrowright = 0xf03e,
     arrowsmalldown = 0xf0a0,
     arrowsmallleft = 0xf0a1,
     arrowsmallright = 0xf071,
     arrowsmallup = 0xf09f,
     arrowup = 0xf03d,
     book = 0xf007,
     bookmark = 0xf07b,
     briefcase = 0xf0d3,
     broadcast = 0xf048,
     browser = 0xf0c5,
     bug = 0xf091,
     calendar = 0xf068,
     check = 0xf03a,
     checklist = 0xf076,
     chevrondown = 0xf0a3,
     chevronleft = 0xf0a4,
     chevronright = 0xf078,
     chevronup = 0xf0a2,
     circleslash = 0xf084,
     circuitboard = 0xf0d6,
     clippy = 0xf035,
     clock = 0xf046,
     clouddownload = 0xf00b,
     cloudupload = 0xf00c,
     code = 0xf05f,
     comment = 0xf02b,
     commentdiscussion = 0xf04f,
     creditcard = 0xf045,
     dash = 0xf0ca,
     dashboard = 0xf07d,
     database = 0xf096,
     devicecamera = 0xf056,
     devicecameravideo = 0xf057,
     devicedesktop = 0xf27c,
     devicemobile = 0xf038,
     diff = 0xf04d,
     diffadded = 0xf06b,
     diffignored = 0xf099,
     diffmodified = 0xf06d,
     diffremoved = 0xf06c,
     diffrenamed = 0xf06e,
     ellipsis = 0xf09a,
     eye = 0xf04e,
     filebinary = 0xf094,
     filecode = 0xf010,
     filedirectory = 0xf016,
     filemedia = 0xf012,
     filepdf = 0xf014,
     filesubmodule = 0xf017,
     filesymlinkdirectory = 0xf0b1,
     filesymlinkfile = 0xf0b0,
     filetext = 0xf011,
     filezip = 0xf013,
     flame = 0xf0d2,
     fold = 0xf0cc,
     gear = 0xf02f,
     gift = 0xf042,
     gist = 0xf00e,
     gistsecret = 0xf08c,
     gitbranch = 0xf020,
     gitcommit = 0xf01f,
     gitcompare = 0xf0ac,
     gitmerge = 0xf023,
     gitpullrequest = 0xf009,
     globe = 0xf0b6,
     graph = 0xf043,
     beaker = 0xf0dd,
     heart = 0x2665,
     history = 0xf07e,
     home = 0xf08d,
     horizontalrule = 0xf070,
     hourglass = 0xf09e,
     hubot = 0xf09d,
     inbox = 0xf0cf,
     info = 0xf059,
     issueclosed = 0xf028,
     issueopened = 0xf026,
     issuereopened = 0xf027,
     jersey = 0xf019,
     key = 0xf049,
     keyboard = 0xf00d,
     law = 0xf0d8,
     lightbulb = 0xf000,
     link = 0xf05c,
     linkexternal = 0xf07f,
     listordered = 0xf062,
     listunordered = 0xf061,
     location = 0xf060,
     lock = 0xf06a,
     logogithub = 0xf092,
     mail = 0xf03b,
     mailread = 0xf03c,
     mailreply = 0xf051,
     markgithub = 0xf00a,
     markdown = 0xf0c9,
     megaphone = 0xf077,
     mention = 0xf0be,
     milestone = 0xf075,
     mirror = 0xf024,
     mortarboard = 0xf0d7,
     mute = 0xf080,
     nonewline = 0xf09c,
     octoface = 0xf008,
     organization = 0xf037,
     package = 0xf0c4,
     paintcan = 0xf0d1,
     pencil = 0xf058,
     person = 0xf018,
     pin = 0xf041,
     plug = 0xf0d4,
     plus = 0xf05d,
     primitivedot = 0xf052,
     primitivesquare = 0xf053,
     pulse = 0xf085,
     puzzle = 0xf0c0,
     question = 0xf02c,
     quote = 0xf063,
     radiotower = 0xf030,
     repo = 0xf001,
     repoclone = 0xf04c,
     repoforcepush = 0xf04a,
     repoforked = 0xf002,
     repopull = 0xf006,
     repopush = 0xf005,
     rocket = 0xf033,
     rss = 0xf034,
     ruby = 0xf047,
     search = 0xf02e,
     server = 0xf097,
     settings = 0xf07c,
     signin = 0xf036,
     signout = 0xf032,
     squirrel = 0xf0b2,
     star = 0xf02a,
     steps = 0xf0c7,
     stop = 0xf08f,
     sync = 0xf087,
     tag = 0xf015,
     telescope = 0xf088,
     terminal = 0xf0c8,
     threebars = 0xf05e,
     thumbsdown = 0xf0db,
     thumbsup = 0xf0da,
     tools = 0xf031,
     trashcan = 0xf0d0,
     triangledown = 0xf05b,
     triangleleft = 0xf044,
     triangleright = 0xf05a,
     triangleup = 0xf0aa,
     unfold = 0xf039,
     unmute = 0xf0ba,
     versions = 0xf064,
     x = 0xf081,
     zap = 0x26A1,
  };
}

// ///////////////////////////////////////////////////////////////////
// dtkFontOcticonsIconPainter
// ///////////////////////////////////////////////////////////////////

class dtkFontOcticonsIconPainter;

// ///////////////////////////////////////////////////////////////////
// dtkFontOcticons
// ///////////////////////////////////////////////////////////////////

class DTKFONTS_EXPORT dtkFontOcticons : public QObject
{
    Q_OBJECT

public:
    static dtkFontOcticons *instance(void);

protected:
    static dtkFontOcticons *s_instance;

private:
    explicit dtkFontOcticons(QObject *parent = 0);
    virtual ~dtkFontOcticons(void);

public:
    void init(const QString& fontname);
    bool initFontOcticons(void);

    void setDefaultOption(const QString& name, const QVariant& value);
    QVariant defaultOption(const QString& name);

    QIcon icon(int character, const QVariantMap& options = QVariantMap());
    QIcon icon(dtkFontOcticonsIconPainter *painter, const QVariantMap& optionMap = QVariantMap());

    void give(const QString& name, dtkFontOcticonsIconPainter *painter);

    QFont font(int size);

    QString fontName(void) { return fontName_ ; }

private:
    QString fontName_;
    QHash<QString, dtkFontOcticonsIconPainter*> painterMap_;
    QVariantMap defaultOptions_;
    dtkFontOcticonsIconPainter* fontIconPainter_;
};

// ///////////////////////////////////////////////////////////////////
// dtkFontOcticonsIconPainter
// ///////////////////////////////////////////////////////////////////

class DTKFONTS_EXPORT dtkFontOcticonsIconPainter
{
public:
    virtual ~dtkFontOcticonsIconPainter(void) {}

public:
    virtual void paint(dtkFontOcticons *awesome, QPainter *painter, const QRect& rect, QIcon::Mode mode, QIcon::State state, const QVariantMap& options) = 0;
};

//
// dtkFontOcticonsIcons.h ends here
