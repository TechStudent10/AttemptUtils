#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

#include <fmt/chrono.h>

using namespace geode::prelude;

class $modify(PlayLayer) {
	void updateAttempts() {
		if (!Mod::get()->getSettingValue<bool>("total-attempts")) {
			PlayLayer::updateAttempts();
			return;
		}
		m_attemptLabel->setString(
			fmt::format("Attempt {}", m_level->m_attempts.value() + 1).c_str()
		);
	}

	void updateTimeLabel(int p0, int p1, bool p2) {
		if (!Mod::get()->getSettingValue<bool>("plat-time")) {
			PlayLayer::updateTimeLabel(p0, p1, p2);
			return;
		}
		if (!m_level->isPlatformer()) {
			return;
		}
		auto msStr = std::to_string(p1);
		if (msStr.length() == 1) {
			msStr = fmt::format("0{}", p1);
		}

		// https://stackoverflow.com/a/58697497/20616402
		
		using namespace std::literals::chrono_literals;

		auto duration = std::chrono::duration<int>{p0};
		auto timeString = fmt::format("{:%H:%M:%S}.{}", duration, msStr);
		m_percentageLabel->setString(
			timeString.c_str()
		);
		m_percentageLabel->setPositionX(
			(CCDirector::get()->getWinSize().width / 2) - m_percentageLabel->getContentWidth() / 2
		);
		m_percentageLabel->setAnchorPoint({ -0.5f, 0.5f });
	}
};

class $modify(MyInfoLayer, LevelInfoLayer) {
	void onLevelInfo(CCObject* sender) {
		LevelInfoLayer::onLevelInfo(sender);
		auto btnSprite = ButtonSprite::create("Reset Attempts", "bigFont.fnt", "GJ_button_01.png");
		btnSprite->setScale(0.65f);
		auto btn = CCMenuItemSpriteExtra::create(
			btnSprite, this, menu_selector(MyInfoLayer::onResetAttempts)
		);
		auto menu = CCMenu::create();
		menu->addChild(btn);
		menu->setPositionY(35.f);
		menu->setZOrder(3);
		menu->setTouchPriority(-9999);
		CCDirector::get()->getRunningScene()->getChildByID("FLAlertLayer")->getChildByID("main-layer")->addChild(menu);
	}

	void onResetAttempts(CCObject* sender) {
		geode::createQuickPopup(
			"Reset Attempts",
			"This will <cr>reset your attempts and jumps on this level to 0</c>. Are you sure you want to proceed?",
			"No", "Yes I want to reset",
			[this](auto, bool btn2) {
				if (btn2) {
					m_level->m_attempts = 0;
					m_level->m_jumps = 0;

					FLAlertLayer::create(
						"Success",
						"Attempts and jumps have been reset!",
						"OK"
					)->show();
				}
			}
		);
	}
};
