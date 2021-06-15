#include "main_frame.hpp"
#include "about_tab.hpp"
#include "list_download_tab.hpp"
#include "ams_tab.hpp"
#include "tools_tab.hpp"
#include <json.hpp>
#include <fstream>
#include "utils.hpp"
#include "fs.hpp"

namespace i18n = brls::i18n;
using namespace i18n::literals;
using json = nlohmann::json;

MainFrame::MainFrame() : TabFrame()
{
    this->setIcon("romfs:/gui_icon.png");
    this->setTitle(std::string(APP_TITLE));

    std::string tag = util::getLatestTag(TAGS_INFO);
    if(!tag.empty() && tag != APP_VERSION)
        this->setFooterText("v" + std::string(APP_VERSION) + "menus/main/new_update"_i18n);
    else
        this->setFooterText("v" + std::string(APP_VERSION));
    
        this->addTab("menus/main/update_cfw"_i18n, new AmsTab());
        this->addTab("menus/main/download_firmware"_i18n, new ListDownloadTab(archiveType::fw));
        this->addTab("menus/main/download_cheats"_i18n, new ListDownloadTab(archiveType::cheats));
        this->addTab("menus/main/tools"_i18n, new ToolsTab(tag));
        this->addTab("menus/main/about"_i18n, new AboutTab());

    this->registerAction("" , brls::Key::B, [this] { return true; });
}
