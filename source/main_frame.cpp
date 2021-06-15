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

namespace {
    constexpr const char AppTitle[] = APP_TITLE;
    constexpr const char AppVersion[] = APP_VERSION;
}

MainFrame::MainFrame() : TabFrame()
{
    this->setIcon("romfs:/gui_icon.png");
    this->setTitle(AppTitle);

    s64 freeStorage;

    std::string tag = util::getLatestTag(TAGS_INFO);
    
    this->setFooterText(fmt::format("v{} | {:.1f}{}", 
                            (!tag.empty() && tag != AppVersion) ? AppVersion + "menus/main/new_update"_i18n : AppVersion,
                            R_SUCCEEDED(fs::getFreeStorageSD(freeStorage)) ? (float)freeStorage/0x40000000 : -1,
                            "menus/main/GB_available"_i18n)
                        );

    
        this->addTab("menus/main/update_cfw"_i18n, new AmsTab());
        this->addTab("menus/main/download_firmware"_i18n, new ListDownloadTab(archiveType::fw));
        this->addTab("menus/main/download_cheats"_i18n, new ListDownloadTab(archiveType::cheats));
        this->addTab("menus/main/tools"_i18n, new ToolsTab(tag));
        this->addTab("menus/main/about"_i18n, new AboutTab());

    this->registerAction("" , brls::Key::B, [this] { return true; });
}
