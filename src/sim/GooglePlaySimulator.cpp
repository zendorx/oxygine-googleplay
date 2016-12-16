#include "GooglePlaySimulator.h"
#include "googleplay.h"
#include "json/json.h"
#include "core/file.h"
#include "Box9Sprite.h"
#include "TextField.h"
#include "DebugActor.h"
#include "res/Resources.h"
#include "Stage.h"
#include "core/oxygine.h"
#include "EventDispatcher.h"

DECLARE_SMART(Btn, spBtn);
class Btn : public Box9Sprite
{
public:
    Btn()
    {
        _txt = new TextField;
        _txt->setText("OK");
        _txt->setAlign(TextStyle::VALIGN_MIDDLE, TextStyle::HALIGN_CENTER);
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
        _title->setAlign(TextStyle::VALIGN_MIDDLE, TextStyle::HALIGN_CENTER);
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
        center = getStage()->global2local(center);

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

bool _googlePlaySimulator_Interstitital_isLoaded = false;

void googlePlaySimulator_Interstitial_Show()
{
	spGooglePlayDialog dialog = new GooglePlayDialog;
	dialog->setScale(1.0f / getStage()->getScaleX());
	dialog->setSize(500, 300);
	getStage()->addChild(dialog);

	dialog->_btnOk->addClickListener([=](Event * e)
	{
		dialog->detach();
		_googlePlaySimulator_Interstitital_isLoaded = false;
		google::interstitial::OnAdClosed ev;
		google::dispatcher()->dispatchEvent(&ev);
	});

	dialog->_btnCancel->setVisible(false);
}

void googlePlaySimulator_Interstitial_Load()
{
	getStage()->addTween(TweenDummy(), 5000 + rand() % 5000)->setDoneCallback([](Event *ev) {
		_googlePlaySimulator_Interstitital_isLoaded = true;
		google::interstitial::OnAdLoaded event;
		google::dispatcher()->dispatchEvent(&event);
	});
}

bool googlePlaySimulator_Interstitial_isLoaded()
{
	return _googlePlaySimulator_Interstitital_isLoaded;
}

bool _googlePlaySimulator_Rewarded_isLoaded = false;

void googlePlaySimulator_Rewarded_Show()
{
	spGooglePlayDialog dialog = new GooglePlayDialog;
	dialog->setScale(1.0f / getStage()->getScaleX());
	dialog->setSize(500, 300);
	getStage()->addChild(dialog);

	dialog->_btnOk->addClickListener([=](Event * e)
	{
		dialog->detach();
		_googlePlaySimulator_Rewarded_isLoaded = false;
		google::rewarded::OnRewarded ev;
		google::dispatcher()->dispatchEvent(&ev);
	});

	dialog->_btnCancel->addClickListener([=](Event * e)
	{
		_googlePlaySimulator_Rewarded_isLoaded = false;
		dialog->detach();
		google::rewarded::OnRewardedVideoAdClosed ev;
		google::dispatcher()->dispatchEvent(&ev);
	});
}

void googlePlaySimulator_Rewarded_Load()
{
	getStage()->addTween(TweenDummy(), 5000 + rand() % 5000)->setDoneCallback([](Event *ev) {
		_googlePlaySimulator_Rewarded_isLoaded = true;
		google::rewarded::OnRewardVideoLoaded event;
		google::dispatcher()->dispatchEvent(&event);
	});
}


//Json::Value _googleplay(Json::objectValue);

/*void googleplaySimulatorLogin()
{
    spGooglePlayDialog dialog = new GooglePlayDialog;
    dialog->setScale(1.0f / getStage()->getScaleX());
    dialog->setSize(500, 300);
    getStage()->addChild(dialog);

    dialog->_btnOk->addClickListener([ = ](Event * e)
    {
        dialog->detach();
        e->removeListener();
        _isLoggedIn = true;

        googleplay::internal::loginResult(true);
    });

    dialog->_btnCancel->addClickListener([ = ](Event * e)
    {
        dialog->detach();
        e->removeListener();
    });
}*/

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
