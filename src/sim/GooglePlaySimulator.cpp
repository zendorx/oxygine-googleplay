#include "GooglePlaySimulator.h"
#include "googleplay.h"
#include "oxygine/json/json.h"
#include "oxygine/core/file.h"
#include "oxygine/actor/Box9Sprite.h"
#include "oxygine/actor/TextField.h"
#include "oxygine/actor/DebugActor.h"
#include "oxygine/res/Resources.h"
#include "oxygine/actor/Stage.h"
#include "oxygine/core/oxygine.h"

bool googleplay_signedin = false;
string googleplay_token = "token-token-token";
string googleplay_uid = "1111111111111";
string googleplay_displayname = "Full Name";

DECLARE_SMART(Btn, spBtn);
class Btn : public Box9Sprite
{
public:
    Btn()
    {
        _txt = new TextField;
        _txt->setText("OK");
        _txt->setAlign(TextStyle::VALIGN_MIDDLE, TextStyle::HALIGN_MIDDLE);
        addChild(_txt);

        setColor(Color::Green);
        setResAnim(DebugActor::resSystem->getResAnim("btn"));

        addEventListener(TouchEvent::OVER, CLOSURE(this, &Btn::touch));
        addEventListener(TouchEvent::OUT, CLOSURE(this, &Btn::touch));
    }

    void setText(const string& txt)
    {
        _txt->setText(txt);
    }

    void touch(Event* ev)
    {
        if (ev->type == TouchEvent::OVER)
            setColor(Color::GreenYellow);
        if (ev->type == TouchEvent::OUT)
            setColor(Color::Green);
    }

    void sizeChanged(const Vector2& size)
    {
        _txt->setSize(size);
    }

    spTextField _txt;
};

DECLARE_SMART(GooglePlayDialog, spGooglePlayDialog);
class GooglePlayDialog : public Actor
{
public:
    enum
    {
        EVENT_OK = 12323,
        EVENT_CANCEL
    };

    GooglePlayDialog()
    {
        setPriority(9999);

        spActor blocker = new Actor;
        blocker->setPosition(-getStage()->getSize());
        blocker->setSize(getStage()->getSize() * 3);
        addChild(blocker);

        _bg = new Box9Sprite;
        addChild(_bg);

        _title = new TextField;
        _title->setAlign(TextStyle::VALIGN_MIDDLE, TextStyle::HALIGN_MIDDLE);
        _title->setMultiline(true);
        _title->setColor(Color::Black);
        addChild(_title);

        _btnOk = new Btn();
        _btnOk->setSize(70, 30);
        _btnOk->setText("Ok");
        addChild(_btnOk);

        _btnCancel = new Btn();
        _btnCancel->setSize(70, 30);
        _btnCancel->setText("Cancel");
        addChild(_btnCancel);
    }

    void setTitle(const string& txt)
    {
        _title->setText(txt);
    }

    void doRender(const RenderState& rs)
    {
        //Stage::render()
    }

    void sizeChanged(const Vector2& size)
    {
        _bg->setSize(size);

        Vector2 center = core::getDisplaySize().cast<Vector2>() / 2.0f;
		
        center = getStage()->local2parent(center);

        float sx = getStage()->getScaleX();
        setPosition(center - size / sx / 2);

        _btnOk->setPosition(size - _btnOk->getSize() - Vector2(10, 10));
        _btnCancel->setPosition(10, getHeight() - 10 - _btnCancel->getHeight());
        _title->setWidth(getWidth());
        _title->setHeight(_btnCancel->getY());
    }

    spBox9Sprite        _bg;
    spTextField         _title;
    spBtn               _btnOk;
    spBtn               _btnCancel;
};


//Json::Value _googleplay(Json::objectValue);

void googleplaySimulator_SignOut()
{
	googleplay_signedin = false;
}

bool googleplaySimulator_isSignedIn()
{
	return googleplay_signedin;
}

void googleplaySimulator_SignIn()
{
    spGooglePlayDialog dialog = new GooglePlayDialog;
    dialog->setScale(1.0f / getStage()->getScaleX());
    dialog->setSize(500, 300);
    getStage()->addChild(dialog);

    dialog->_btnOk->addClickListener([ = ](Event * e)
    {
        dialog->detach();
        e->removeListener();
		googleplay_signedin = true;

		getStage()->addTween(TweenDummy(), 2500 + rand() % 1000)->addDoneCallback([](Event*ev) {
			googleplay::internal::onSignInResult(0);
		});
        
    });

    dialog->_btnCancel->addClickListener([ = ](Event * e)
    {
        dialog->detach();
        e->removeListener();
    });
}

void googleplaySimulator_RequestToken()
{
	if (googleplay_signedin)
	{
		
		getStage()->addTween(TweenDummy(), 2500 + rand() % 2000)->addDoneCallback([](Event*ev) {
			googleplay::internal::onGetTokenResult(googleplay_uid, googleplay_token);
		});
		
	}
}

string googleplaySimulator_getUserID()
{
	return googleplay_uid;
}


string googleplaySimulator_getUserName()
{
	return googleplay_displayname;
}

void googleplaySimulatorInit()
{
    log::messageln("GooglePlay Simulator Init");


    /*file::buffer bf;
    file::read(".googleplay", bf, ep_ignore_error);

    if (!bf.empty())
    {
        Json::Reader reader;
        reader.parse((char*)&bf.front(), (char*)&bf.front() + bf.size(), _googleplay, false);
    }*/
}
